#pragma once

#include "stdafx.h"
#include "OptionalReference.h"

namespace Chimp {
	class AnyConstReference;

	class AnyOptionalReference {
	public:
		template <typename T>
	explicit	AnyOptionalReference(T& t) :
			Type(&typeid(T)),
			Ptr(&t) {
		static_assert(!std::is_same_v<std::remove_cvref_t<T>, AnyConstReference>,
			"T must not be AnyConstReference");
		static_assert(!std::is_same_v<std::remove_cvref_t<T>, AnyOptionalReference>,
			"T must not be AnyOptionalReference");
		}

		AnyOptionalReference() :
			Type(nullptr),
			Ptr(nullptr) {
		}

		AnyOptionalReference(TypeInfo typeInfo, void* notNullPtr) :
			Type(typeInfo),
			Ptr(notNullPtr) {
		}

		TypeInfo GetType() const { return Type; };
		void* GetPtr() const { return Ptr; }

	private:
		TypeInfo Type;
		void* Ptr;
	};

	class AnyConstReference {
	public:
		template <typename T>
		explicit AnyConstReference(const T& t) :
			Type(&typeid(T)),
			Ptr(&t) {
			static_assert(!std::is_same_v<std::remove_cvref_t<T>, AnyConstReference>,
				"T must not be AnyConstReference");
			static_assert(!std::is_same_v<std::remove_cvref_t<T>, AnyOptionalReference>,
				"T must not be AnyOptionalReference");
		}

		AnyConstReference(TypeInfo typeInfo, void* notNullPtr) :
			Type(typeInfo),
			Ptr(notNullPtr) {
			assert(notNullPtr);
		}

		TypeInfo GetType() const { return Type; };
		const void* GetPtr() const { return Ptr; }

		bool operator==(const AnyConstReference& other) const {
			return Ptr == other.Ptr;
		}

	private:
		TypeInfo Type;
		const void* Ptr;
	};

	typedef AnyOptionalReference AnyReference;
}