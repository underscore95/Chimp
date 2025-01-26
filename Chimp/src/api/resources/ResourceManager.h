#pragma once

#include "stdafx.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/graphics/textures/ITexture.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/utils/OptionalReference.h"
#include "IModelImporter.h"
#include "MeshStorage.h"
#include "containers/ShaderResourceContainer.h"
#include "containers/TextureResourceContainer.h"
#include "containers/ModelResourceContainer.h"
#include "containers/SpriteResourceContainer.h"
#include "containers/ImageResourceContainer.h"
#include "containers/SoundResourceContainer.h"
#include "containers/SoundEffectResourceContainer.h"

namespace Chimp {
	class Engine;
	class MainLoop;
	class IShader;

	class ResourceManager {
		friend class Engine;
		friend class MainLoop;
	private:
		ResourceManager(Engine& engine);

	public:
		// Stores IShader, this is wrapper classes around shaders in the rendering API
		[[nodiscard]] ResourceContainer<ShaderFilePaths, IShader>& GetShaders();

		// Stores textures, you probably don't need to use this, and can use sprites or models instead
		[[nodiscard]] ResourceContainer<std::string, ITexture>& GetTextures();

		// Stores 3d models
		[[nodiscard]] ModelResourceContainer& GetModels();

		// Stores sprites (textured quads)
		[[nodiscard]] ResourceContainer<TextureResourcePath, Mesh>& GetSprites();

		// Stores images (cpu side) - not used for rendering!
		[[nodiscard]] ResourceContainer<ImageResourcePath, IImageLoader::LoadedImage>& GetImages();

		// Stores audio files, you probably don't need this.
		// For music, use a MusicPlayer
		// For sound effects, use the sound effects container (this allows multiple sounds per effect, random pitch, and more)
		[[nodiscard]] ResourceContainer<SoundResourcePath, ISound>& GetBasicSounds();

		// Stores sound effects
		[[nodiscard]] SoundEffectResourceContainer& GetSoundEffects();

		// Get the mesh storage, used to store meshes/models that weren't loaded from file (see MeshStorage for more information)
		[[nodiscard]] MeshStorage& GetMeshStorage();

		// Unload all Assets with 0 references
		void UnloadUnusedResources();

		// Load required Assets
		void LoadRequiredResources();

	private:
		void InitModelImporter();

		void Update();

	private:
		Engine& m_Engine;
		std::unique_ptr<IModelImporter> m_ModelImporter;
		MeshStorage m_MeshStorage;

		ShaderResourceContainer m_Shaders;
		TextureResourceContainer m_Textures;
		ModelResourceContainer m_Models;
		SpriteResourceContainer m_Sprites;
		ImageResourceContainer m_Images;
		SoundResourceContainer m_Sounds;
		SoundEffectResourceContainer m_SoundEffects;
	};
}