#pragma once

#include "stdafx.h"
#include "OptionalReference.h"

namespace Chimp {
	class AnyOptionalReference {
	public:
		template <typename T>
		AnyOptionalReference(T& t) :
			Type(&typeid(T)),
			Ptr(&t) {
		}

		AnyOptionalReference() :
			Type(nullptr),
			Ptr(nullptr) {
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
		AnyConstReference(const T& t) :
			Type(&typeid(T)),
			Ptr(&t) {
		}

		AnyConstReference(TypeInfo typeInfo, void* notNullPtr) :
			Type(typeInfo),
			Ptr(notNullPtr) {
			assert(notNullPtr);
		}

		TypeInfo GetType() const { return Type; };
		void* GetPtr() const { return Ptr; }

		bool operator==(const AnyConstReference& other) const {
			return Ptr == other.Ptr;
		}

	private:
		TypeInfo Type;
		void* Ptr;
	};
}