#include "api/graphics/GraphicsType.h"

namespace Chimp
{
	GraphicsType::GraphicsType(unsigned int size) :
		Size(size),
		m_Id(NumberTypes++)
	{
	}

	bool GraphicsType::operator==(const GraphicsType& other) const
	{
		return other.m_Id == m_Id;
	}

	unsigned int GraphicsType::NumberTypes = 0;

	// if adding more, update translations.h
	const GraphicsType GraphicsType::FLOAT(4);
	const GraphicsType GraphicsType::UNSIGNED_INT(4);
	const GraphicsType GraphicsType::UNSIGNED_SHORT(2);
	const GraphicsType GraphicsType::UNSIGNED_BYTE(1);
}