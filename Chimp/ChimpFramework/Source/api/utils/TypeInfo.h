#pragma once

#include "stdafx.h"
#include "OptionalReference.h"

namespace Chimp {
	class TypeInfo {
	public:
		template <typename T>
		TypeInfo(OptionalReference<T> t) : m_TypeInfo(&typeid(T)) {}

		template <typename T>
		TypeInfo() : TypeInfo(&typeid(T)) {}

		TypeInfo() : TypeInfo(nullptr) {}

		TypeInfo(const type_info& typeInfo) : TypeInfo(&typeInfo) {}
		TypeInfo(const type_info* typeInfo) : m_TypeInfo(typeInfo) {
#ifndef NDEBUG
			m_NameOfStoredTypeDoNotUse = m_TypeInfo == nullptr ? "Invalid Type" : m_TypeInfo->name();
#endif
		}

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

		bool operator==(const type_info* other) const {
			return Hash() == other->hash_code();
		}

		bool operator==(const type_info& other) const {
			return *this == &other;
		}

	private:
		const type_info* m_TypeInfo;
#ifndef NDEBUG
		std::string m_NameOfStoredTypeDoNotUse;
#endif
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
