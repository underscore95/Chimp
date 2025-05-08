#pragma once

#include "stdafx.h"
#include "OptionalReference.h"

namespace Chimp {
	class TypeInfo {
	public:
		template <typename T>
		TypeInfo(OptionalReference<T> t) : m_TypeInfo(&typeid(T)) {}

		template <typename T>
		TypeInfo() : m_TypeInfo(&typeid(T)) {}

		TypeInfo() : m_TypeInfo(nullptr) {}

		TypeInfo(const type_info& typeInfo) : m_TypeInfo(&typeInfo) {}
		TypeInfo(const type_info* typeInfo) : m_TypeInfo(typeInfo) {}

		inline const char* Name() const {
			assert(m_TypeInfo);
			return m_TypeInfo->name();
		}

		inline size_t Hash() const {
			assert(m_TypeInfo);
			return m_TypeInfo->hash_code();
		}

		bool operator==(const TypeInfo& other) const {
			return Hash() == other.Hash();
		}

	private:
		const type_info* m_TypeInfo;
	};
}

namespace std {
	template<>
	struct hash<Chimp::TypeInfo> {
		std::size_t operator()(const Chimp::TypeInfo& info) const noexcept {
			return info.Hash();
		}
	};
}
