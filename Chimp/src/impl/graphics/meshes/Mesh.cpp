#include "api/graphics/meshes/Mesh.h"

namespace Chimp {
	// Mesh::Builder
	Mesh::Builder::Builder()
	{
	}

	Mesh::Builder& Mesh::Builder::AddSection(
		std::unique_ptr<IElementArray> elementArray,
		std::optional<TextureDependency> texture
	)
	{
		return AddSection(std::make_unique<Section>(Section{
			std::move(elementArray),
			std::move(texture)
			}));
	}

	Mesh::Builder& Mesh::Builder::AddSection(std::unique_ptr<Section> section)
	{
		if (!CheckIfBuilt())
		{
			m_Sections.push_back(std::move(section));
		}
		return *this;
	}

	std::unique_ptr<Mesh> Mesh::Builder::Build()
	{
		if (CheckIfBuilt())
		{
			return nullptr;
		}
		m_IsBuilt = true;
		return std::unique_ptr<Mesh>(new Mesh(*this));
	}

	bool Mesh::Builder::CheckIfBuilt() const
	{
		if (m_IsBuilt)
		{
			std::cerr << "Attempted to use a MeshBuilder that has already built a mesh." << std::endl;
			assert(false);
			return true;
		}
		return false;
	}

	// Mesh
	Mesh::Mesh(Builder& builder)
	{
		m_Sections = std::move(builder.m_Sections);
		m_Sections.shrink_to_fit();
	}

	// Const iterator
	const Mesh::Section& Mesh::ConstIterator::operator*() const
	{
		return **m_Iterator;
	}

	const Mesh::Section* Mesh::ConstIterator::operator->() const
	{
		return m_Iterator->get();
	}

	Mesh::ConstIterator& Mesh::ConstIterator::operator++()
	{
		++m_Iterator;
		return *this;
	}

	Mesh::ConstIterator Mesh::ConstIterator::operator++(int)
	{
		ConstIterator tmp = *this;
		++m_Iterator;
		return tmp;
	}

	bool Mesh::ConstIterator::operator==(const ConstIterator& other) const
	{
		return m_Iterator == other.m_Iterator;
	}

	bool Mesh::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return m_Iterator != other.m_Iterator;
	}

	Mesh::ConstIterator Mesh::begin() const
	{
		return ConstIterator(m_Sections.begin());
	}

	Mesh::ConstIterator Mesh::end() const
	{
		return ConstIterator(m_Sections.end());
	}
}
