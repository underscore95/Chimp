#pragma once

#include "stdafx.h"

namespace Chimp {
	// Essentially a non-owning raw pointer, this class just exists to make it more clear that the pointer isn't owned
	template <typename T>
	class OptionalReference {
	public:
		OptionalReference() : m_Value(nullptr) {}
		OptionalReference(T* value) : m_Value(value) {}

		void operator=(T* value) { m_Value = value; }
		void operator=(T& value) { m_Value = &value; }

		T* operator->() { assert(HasValue()); return m_Value; }
		T& operator*() { assert(HasValue()); return *m_Value; }
		T* operator&() { return m_Value; }

		bool HasValue() const { return m_Value != nullptr; }
		T& Get() { assert(HasValue()); return *m_Value; }
		const T& Get() const { assert(HasValue()); return *m_Value; }

		operator bool() const { return HasValue(); }

	private:
		T* m_Value;
	};

	template <typename T>
	class ConstOptionalReference {
	public:
		ConstOptionalReference() : m_Value(nullptr) {}
		ConstOptionalReference(const T* value) : m_Value(value) {}

		const T* operator->() const {
			assert(HasValue());
			return m_Value;
		}
		const T& operator*() const { return *m_Value; }

		bool HasValue() const { return m_Value != nullptr; }

		operator bool() const { return HasValue(); }

	private:
		const T* m_Value;
	};

	template <typename T>
	using RawPtr = OptionalReference<T>;

	template <typename T>
	using ConstRawPtr = ConstOptionalReference<T>;

	template <typename T>
	using Reference = OptionalReference<T>;

	template <typename T>
	using ConstReference = ConstOptionalReference<T>;
}