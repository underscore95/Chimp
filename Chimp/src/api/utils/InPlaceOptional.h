#pragma once

namespace Chimp {
	// An optional where the value is stored at the same place as the optional
	// useful if you don't want to follow a pointer
	// has some restrictions like there is always a value technically, it just may be marked as empty
	// must be default constructible
	template<typename T>
	class InPlaceOptional
	{
	public:
		using ValueType = T;
	public:
		InPlaceOptional() : m_Value(), m_HasValue(false) {}
		InPlaceOptional(T value) : m_Value(value), m_HasValue(true) {}

		InPlaceOptional<T>& operator=(T value) {
			m_Value = value;
			m_HasValue = true;
			return *this;
		}

		InPlaceOptional<T>& operator=(InPlaceOptional<T> other) {
			m_Value = other.m_Value;
			m_HasValue = other.m_HasValue;
			return *this;
		}

		T* operator->() {
			assert(m_HasValue);
			return m_Value;
		}

		[[nodiscard]] bool HasValue() const {
			return m_HasValue;
		}

		[[nodiscard]]  bool Empty() const {
			return !m_HasValue;
		}

		[[nodiscard]] T Value() const {
			assert(m_HasValue);
			return m_Value;
		}

		// Returns pointer to value, even if value is empty
		[[nodiscard]] T& UnsafeGet() {
			return m_Value;
		}

		void Reset() {
			m_HasValue = false;
		}

	private:
		T m_Value;
		bool m_HasValue;
	};
}