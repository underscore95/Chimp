#include "ModelImporter.h"
#include "api/graphics/IRenderingManager.h"
#include "api/utils/RawArray.h"
#include "api/utils/FilePath.h"
#include "api/resources/ResourceManager.h"
#include "Loggers.h"

namespace Chimp {
	std::unique_ptr<Mesh> ModelImporter::LoadModel(const std::string& path, const Settings& settings)
	{
		const GraphicsType INDEX_TYPE = GraphicsType::UNSIGNED_INT;
		constexpr size_t VERTICES_PER_TRIANGLE = 3;

		unsigned int importFlags = 0;
		importFlags |= aiProcess_Triangulate;
		importFlags |= aiProcess_FlipUVs && settings.FlipUVs;
		importFlags |= aiProcess_JoinIdenticalVertices;
		importFlags |= aiProcess_FixInfacingNormals;
		importFlags |= aiProcess_FindInvalidData;
		auto model = m_Importer.ReadFile(path, importFlags);

		if (!model || model->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !model->mRootNode) {
			Loggers::Resources().Error("Failed to load model: " + path + " with error: " + m_Importer.GetErrorString());
			return nullptr;
		}

		const float FLOATS_PER_VERTEX = 3 + // Position
			(settings.IncludeNormals ? 3 : 0) +
			(settings.IncludeTextureCoordinates ? 2 : 0);

		Mesh::Builder builder;
		for (size_t meshIndex = 0; meshIndex < model->mNumMeshes; ++meshIndex) {
			auto modelMesh = model->mMeshes[meshIndex];

			if (settings.IncludeTextureCoordinates) {
				if (!modelMesh->HasTextureCoords(0)) {
					Loggers::Resources().Error("Mesh section " + std::to_string(meshIndex) + " does not have texture coordinates: " + path);
					return nullptr;
				}
			}
			if (settings.IncludeNormals) {
				if (!modelMesh->HasNormals()) {
					Loggers::Resources().Error("Mesh section " + std::to_string(meshIndex) + " does not have normals: " + path);
					return nullptr;
				}
			}

			// Vertex buffer
			RawArray vertexData;
			vertexData.NumberElements = modelMesh->mNumVertices * FLOATS_PER_VERTEX;
			vertexData.Size = sizeof(float) * vertexData.NumberElements;
			float* vertexDataPtr = new float[vertexData.NumberElements];
			vertexData.Data = vertexDataPtr;

			// Populate our raw array
			size_t vertexDataIndex = 0;
			for (size_t vertexIndex = 0; vertexIndex < modelMesh->mNumVertices; ++vertexIndex) {
				// Position
				vertexDataPtr[vertexDataIndex++] = modelMesh->mVertices[vertexIndex].x;
				vertexDataPtr[vertexDataIndex++] = modelMesh->mVertices[vertexIndex].y;
				vertexDataPtr[vertexDataIndex++] = modelMesh->mVertices[vertexIndex].z;

				// Normal
				if (settings.IncludeNormals) {
					vertexDataPtr[vertexDataIndex++] = modelMesh->mNormals[vertexIndex].x;
					vertexDataPtr[vertexDataIndex++] = modelMesh->mNormals[vertexIndex].y;
					vertexDataPtr[vertexDataIndex++] = modelMesh->mNormals[vertexIndex].z;
				}

				// Texture coordinates
				if (settings.IncludeTextureCoordinates) {
					vertexDataPtr[vertexDataIndex++] = modelMesh->mTextureCoords[0][vertexIndex].x;
					vertexDataPtr[vertexDataIndex++] = modelMesh->mTextureCoords[0][vertexIndex].y;
				}

				assert(vertexDataIndex <= vertexData.NumberElements);
			}

			// Create the vertex buffer
			std::unique_ptr<IBuffer> vertexBuffer = m_RenderingManager.CreateBuffer(
				{
					Usage::UpdateFrequency::VERY_OCCASIONAL,
					Usage::Access::CPU_WRITE
				},
				BindTarget::VERTEX_BUFFER
			);
			vertexBuffer->SetData(vertexData);

			// Index buffer
			RawArray indexData;
			indexData.NumberElements = modelMesh->mNumFaces * VERTICES_PER_TRIANGLE;
			indexData.Size = INDEX_TYPE.Size * indexData.NumberElements;
			assert(sizeof(uint32_t) == INDEX_TYPE.Size);
			uint32_t* indexDataPtr = new uint32_t[indexData.NumberElements];
			indexData.Data = indexDataPtr;

			// Populate our raw array
			size_t indexDataIndex = 0;
			for (size_t faceIndex = 0; faceIndex < modelMesh->mNumFaces; ++faceIndex) {
				const auto& face = modelMesh->mFaces[faceIndex];
				assert(face.mNumIndices == VERTICES_PER_TRIANGLE);
				for (size_t vertexIndex = 0; vertexIndex < VERTICES_PER_TRIANGLE; ++vertexIndex) {
					indexDataPtr[indexDataIndex++] = face.mIndices[vertexIndex];
				}
			}

			// Create the index buffer
			auto indexBuffer = m_RenderingManager.CreateBuffer(
				{
					Usage::UpdateFrequency::VERY_OCCASIONAL,
					Usage::Access::CPU_WRITE
				},
				BindTarget::INDEX_BUFFER
			);
			indexBuffer->SetData(indexData);

			// Element array layout
			std::vector<ElementComponentLayout> layoutComponents;
			layoutComponents.push_back({ GraphicsType::FLOAT, 3, false }); // position
			if (settings.IncludeNormals) {
				layoutComponents.push_back({ GraphicsType::FLOAT, 3, false }); // normal
			}
			if (settings.IncludeTextureCoordinates) {
				layoutComponents.push_back({ GraphicsType::FLOAT, 2, false }); // texture coordinates
			}
			std::unique_ptr<IElementArrayLayout> elementLayout = m_RenderingManager.CreateElementArrayLayout(
				PrimitiveType::TRIANGLES,
				layoutComponents
			);
			assert(elementLayout->GetStride() == FLOATS_PER_VERTEX * GraphicsType::FLOAT.Size);

			// Create the element array
			auto elementArray = m_RenderingManager.CreateElementArray(
				std::move(vertexBuffer),
				std::move(indexBuffer),
				INDEX_TYPE,
				std::move(elementLayout)
			);

			// Get the texture
			std::optional<TextureDependency> texture = std::nullopt;
			if (settings.IncludeTextureCoordinates) {
				const aiMaterial* material = model->mMaterials[modelMesh->mMaterialIndex];
				unsigned int textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
				if (textureCount < 1) {
					Loggers::Resources().Error("Mesh section " + std::to_string(meshIndex) + " does not have a texture, if including texture coordinates, all sections must have 1 texture: " + path);
					return nullptr;
				}
				else if (textureCount > 1) {
					Loggers::Resources().Warning("Mesh section " + std::to_string(meshIndex) + " has more than 1 texture, only the first will be used: " + path);
				}
				aiString texturePath; // assumed to be relative to the model file
				material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
				// this is actual path, may be relative to working dir, or absolute, doesn't matter to us though
				std::string correctedTexturePath = GetPathRelativeToFile(texturePath.C_Str(), path);
				texture.emplace(m_ResourceManager.GetTextures(), correctedTexturePath);
			}

			builder.AddSection(std::move(elementArray), std::move(texture));
		}

		return builder.Build();
	}
}