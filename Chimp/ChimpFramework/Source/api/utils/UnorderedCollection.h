#pragma once

#include "stdafx.h"
#include "api/ecs/EntityId.h" // Hacky fix because somewhere includes this before entity id and uses serialisation

namespace Chimp {
	// Uses an array when not many elements, then switches to a vector. Removing swaps with last element
	template <typename T, int SmallNumElements = 8>
	class UnorderedCollection {
		static_assert(SmallNumElements < 126, "SmallNumElements must be < 126 to fit in signed char");

	public:
		UnorderedCollection() : m_ActiveIndex(0) {
			new (&m_Array) std::array<T, SmallNumElements>();
		}

		UnorderedCollection(const std::vector<T> vec) : UnorderedCollection() {
			for (const auto& v : vec) {
				Insert(v);
			}
		}

		UnorderedCollection(const UnorderedCollection& other) : m_ActiveIndex(other.m_ActiveIndex) {
			if (other.IsUsingVector()) {
				new (&m_Vector) std::vector<T>(other.m_Vector);
			}
			else {
				new (&m_Array) std::array<T, SmallNumElements>(other.m_Array);
			}
		}

		UnorderedCollection& operator=(const UnorderedCollection& other) {
			if (this == &other) return *this;
			this->~UnorderedCollection();
			m_ActiveIndex = other.m_ActiveIndex;
			if (other.IsUsingVector()) {
				new (&m_Vector) std::vector<T>(other.m_Vector);
			}
			else {
				new (&m_Array) std::array<T, SmallNumElements>(other.m_Array);
			}
			return *this;
		}

		~UnorderedCollection() {
			if (IsUsingVector()) {
				reinterpret_cast<std::vector<T>*>(&m_Vector)->~vector();
			}
			else {
				reinterpret_cast<std::array<T, SmallNumElements>*>(&m_Array)->~array();
			}
		}

		size_t Size() const {
			size_t size = IsUsingVector() ? GetVector().size() : m_ActiveIndex;
			return size;
		}

		void Insert(const T& value) {
			if (IsUsingVector()) {
				GetVector().push_back(value);
				return;
			}

			auto& arr = GetArray();
			for (char i = 0; i < m_ActiveIndex; ++i) {
				if (arr[i] == value) return;
			}

			if (m_ActiveIndex < SmallNumElements) {
				arr[m_ActiveIndex++] = value;
			}
			else {
				// Transition to vector
				std::array<T, SmallNumElements> tmp = arr;
				arr.~array();
				new (&m_Vector) std::vector<T>();
				auto& s = GetVector();
				for (const auto& val : tmp) s.push_back(val);
				s.push_back(value);
				m_ActiveIndex = USE_SET_FLAG;
			}
		}

		bool Contains(const T& value) const {
			if (IsUsingVector()) {
				const auto& vec = GetVector();
				return std::find(vec.begin(), vec.end(), value) != vec.end();
			}
			else {
				return std::find(GetArray().begin(), GetArray().begin() + m_ActiveIndex, value) != GetArray().begin() + m_ActiveIndex;
			}
		}

		void Remove(const T& value) {
			if (IsUsingVector()) {
				auto& vec = GetVector();
				for (size_t i = 0; i < vec.size(); ++i) {
					if (vec[i] == value) {
						vec[i] = vec.back();
						vec.pop_back();
						return;
					}
				}
			}
			else {
				auto& arr = GetArray();
				for (char i = 0; i < m_ActiveIndex; ++i) {
					if (arr[i] == value) {
						arr[i] = arr[--m_ActiveIndex];
						return;
					}
				}
			}
		}

#pragma region Iterators
		template <bool IsConst>
		class Iterator {
			using ArrayIter = typename std::conditional<IsConst, typename std::array<T, SmallNumElements>::const_iterator, typename std::array<T, SmallNumElements>::iterator>::type;
			using VectorIter = typename std::conditional<IsConst, typename std::vector<T>::const_iterator, typename std::vector<T>::iterator>::type;

			enum class Type { Array, Vector };
			union {
				ArrayIter arrayIt;
				VectorIter vectorIt;
			};
			Type type;

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = typename std::conditional<IsConst, const T&, T&>::type;
			using pointer = typename std::conditional<IsConst, const T*, T*>::type;
			using iterator_category = std::forward_iterator_tag;

			Iterator(ArrayIter it) : arrayIt(it), type(Type::Array) {}
			Iterator(VectorIter it) : vectorIt(it), type(Type::Vector) {}
			~Iterator() {}

			Iterator(const Iterator& other) : type(other.type) {
				if (type == Type::Array) new (&arrayIt) ArrayIter(other.arrayIt);
				else new (&vectorIt) VectorIter(other.vectorIt);
			}

			Iterator& operator=(const Iterator& other) {
				if (this == &other) return *this;
				this->~Iterator();
				type = other.type;
				if (type == Type::Array) new (&arrayIt) ArrayIter(other.arrayIt);
				else new (&vectorIt) VectorIter(other.vectorIt);
				return *this;
			}

			reference operator*() const {
				return (type == Type::Array) ? *arrayIt : *vectorIt;
			}
			pointer operator->() const {
				return (type == Type::Array) ? &(*arrayIt) : &(*vectorIt);
			}
			Iterator& operator++() {
				if (type == Type::Array) ++arrayIt;
				else ++vectorIt;
				return *this;
			}
			Iterator operator++(int) {
				Iterator tmp = *this;
				++(*this);
				return tmp;
			}
			bool operator==(const Iterator& other) const {
				if (type != other.type) return false;
				return (type == Type::Array) ? (arrayIt == other.arrayIt) : (vectorIt == other.vectorIt);
			}
			bool operator!=(const Iterator& other) const {
				return !(*this == other);
			}
		};

		using iterator = Iterator<false>;
		using const_iterator = Iterator<true>;

		iterator begin() {
			return IsUsingVector()
				? iterator(GetVector().begin())
				: iterator(GetArray().begin());
		}
		iterator end() {
			return IsUsingVector()
				? iterator(GetVector().end())
				: iterator(GetArray().begin() + m_ActiveIndex);
		}
		const_iterator begin() const {
			return IsUsingVector()
				? const_iterator(GetVector().begin())
				: const_iterator(GetArray().begin());
		}
		const_iterator end() const {
			return IsUsingVector()
				? const_iterator(GetVector().end())
				: const_iterator(GetArray().begin() + m_ActiveIndex);
		}
		const_iterator cbegin() const { return begin(); }
		const_iterator cend() const { return end(); }

		std::vector<T> CopyToVector() const {
			std::vector<T> copy;
			copy.reserve(Size());
			for (const auto& v : *this) {
				copy.push_back(v);
			}
			return copy;
		}
#pragma endregion

	private:
		static constexpr char USE_SET_FLAG = SmallNumElements + 1;
		char m_ActiveIndex;

		union {
			std::array<T, SmallNumElements> m_Array;
			std::vector<T> m_Vector;
		};

		bool IsUsingVector() const { return m_ActiveIndex == USE_SET_FLAG; }
		std::array<T, SmallNumElements>& GetArray() { return *reinterpret_cast<std::array<T, SmallNumElements>*>(&m_Array); }
		const std::array<T, SmallNumElements>& GetArray() const { return *reinterpret_cast<const std::array<T, SmallNumElements>*>(&m_Array); }
		std::vector<T>& GetVector() { return *reinterpret_cast<std::vector<T>*>(&m_Vector); }
		const std::vector<T>& GetVector() const { return *reinterpret_cast<const std::vector<T>*>(&m_Vector); }
	};

	/*
	If you get compile errors here, the issue is your T isn't serialisable or deserialisable.
	Probably you need to include the file that defines T and defines the to_json and from_json overloads for T before including this file.

	T must also have a default and copy constructor.

	The above constraints are only in effect if you wish to serialise and deserialise.
	*/
	template <typename T, int SmallNumElements>
	inline void to_json(Json& j, const UnorderedCollection<T, SmallNumElements>& e) {
		j = Json::array();
		for (const T& t : e) {
			Json json;
			to_json(json, t);
			j.push_back(json);
		}
	}

	template <typename T, int SmallNumElements>
	inline void from_json(const Json& j, UnorderedCollection<T, SmallNumElements>& e) {
		for (const auto& pair : j.items()) {
			T t;
			const Json& itemJson = pair.value();
			from_json(itemJson, t);
			e.Insert(t);
		}
	}
}