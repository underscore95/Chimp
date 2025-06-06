#pragma once

#include "stdafx.h"
#include "OptionalReference.h"
#include "TypeInfo.h"

namespace Chimp {
	class AnyConstReference;

	class AnyOptionalReference {
	public:
		template <typename T>
		explicit AnyOptionalReference(T& t) :
			m_Type(&typeid(T)),
			m_Ptr(&t) {
			static_assert(!std::is_same_v<std::remove_cvref_t<T>, AnyConstReference>,
				"T must not be AnyConstReference");
			static_assert(!std::is_same_v<std::remove_cvref_t<T>, AnyOptionalReference>,
				"T must not be AnyOptionalReference");
		}

		AnyOptionalReference() :
			m_Type(nullptr),
			m_Ptr(nullptr) {
		}

		AnyOptionalReference(TypeInfo typeInfo, void* notNullm_Ptr) :
			m_Type(typeInfo),
			m_Ptr(notNullm_Ptr) {
		}

		TypeInfo GetType() const { return m_Type; };
		void* GetPtr() const { return m_Ptr; }

		operator AnyConstReference() const;

	private:
		TypeInfo m_Type;
		void* m_Ptr;
	};

	class AnyConstReference {
	public:
		template <typename T>
		explicit AnyConstReference(const T& t) :
			m_Type(&typeid(T)),
			m_Ptr(&t) {
			static_assert(!std::is_same_v<std::remove_cvref_t<T>, AnyConstReference>,
				"T must not be AnyConstReference");
			static_assert(!std::is_same_v<std::remove_cvref_t<T>, AnyOptionalReference>,
				"T must not be AnyOptionalReference");
		}

		AnyConstReference(TypeInfo typeInfo, void* notNullm_Ptr) :
			m_Type(typeInfo),
			m_Ptr(notNullm_Ptr) {
			assert(notNullm_Ptr);
		}

		TypeInfo GetType() const { return m_Type; };
		const void* GetPtr() const { return m_Ptr; }

		bool operator==(const AnyConstReference& other) const {
			return m_Ptr == other.m_Ptr;
		}

	private:
		TypeInfo m_Type;
		const void* m_Ptr;
	};

	typedef AnyOptionalReference AnyReference;
}