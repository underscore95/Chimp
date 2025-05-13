#include "api/utils/AnyReference.h"

namespace Chimp {
	AnyOptionalReference::operator AnyConstReference() const
	{
		assert(m_Ptr != nullptr);
		return AnyConstReference{ m_Type, m_Ptr };
	}
}