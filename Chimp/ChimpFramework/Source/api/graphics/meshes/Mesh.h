#pragma once

#include "../shaders/IShader.h"
#include "../buffers/IElementArray.h"
#include "api/resources/ResourceDependency.h"
#include "api/utils/OptionalReference.h"

namespace Chimp {
	// A mesh consists of several Mesh::Section objects, each with an element array and a shader.
	// This allows different parts of a mesh to have different shaders, materials, etc.
	class Mesh {
		DISABLE_COPY(Mesh);
	public:
		// Section of a mesh
		struct Section {
			std::unique_ptr<IElementArray> ElementArray;
			std::optional<TextureDependency> Texture = std::nullopt;
		};

		// Builder
		class Builder {
			friend class Mesh;
		public:
			Builder();

			// Add a section to the mesh
			Builder& AddSection(
				std::unique_ptr<IElementArray> elementArray,
				std::optional<TextureDependency> texture = std::nullopt
			);
			Builder& AddSection(std::unique_ptr<Section> section);

			// Build the mesh, making the mesh builder unusable
			std::unique_ptr<Mesh> Build();
		private:
			// Check if the mesh has already been built, returns true if it has been built.
			bool CheckIfBuilt() const;
		private:
			std::vector<std::unique_ptr<Section>> m_Sections;
			bool m_IsBuilt = false;
		};
		friend class Builder;

	private:
		Mesh(Builder& builder);

	public:
		// Const iterator
		class ConstIterator {
		public:
			explicit ConstIterator(std::vector<std::unique_ptr<Section>>::const_iterator it) : m_Iterator(it) {}

			const Section& operator*() const;
			const Section* operator->() const;

			ConstIterator& operator++();
			ConstIterator operator++(int);

			bool operator==(const ConstIterator& other) const;
			bool operator!=(const ConstIterator& other) const;

		private:
			std::vector<std::unique_ptr<Section>>::const_iterator m_Iterator;
		};

		ConstIterator begin() const; 
		ConstIterator end() const;

	private:
		std::vector<std::unique_ptr<Section>> m_Sections;
	};
}
