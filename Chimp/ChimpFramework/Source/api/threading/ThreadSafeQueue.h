#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"

namespace Chimp {
	// Thread safe wrapper for a queue
	template<typename T>
	class ThreadSafeQueue
	{
	public:
		[[nodiscard]] OptionalReference<T> Front() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			return OptionalReference<T>(m_Queue.empty() ? nullptr : &m_Queue.front());
		}

		void Pop() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Queue.pop();
		}

		// Pop the front element and return it
		[[nodiscard]] std::unique_ptr<T> PeekAndPop() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			if (m_Queue.empty()) {
				return nullptr;
			}

			auto val = std::make_unique<T>(std::move(m_Queue.front()));
			m_Queue.pop();
			return val;
		}

		// Pop all elements from the queue and call the callback with each element
		void PopAll(std::function<void(const T&)> callback) {
			std::lock_guard<std::mutex> lock(m_Mutex);
			while (!m_Queue.empty()) {
				callback(m_Queue.front());
				m_Queue.pop();
			}
		}

		void Push(const T& value) {
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Queue.push(value);
		}

		template <typename... Args>
		void EmplaceBack(Args&&... args) {
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Queue.emplace(std::forward<Args>(args)...);
		}

		// Transfer all elements from the source queue to the back of this queue, maintaining the order
		void TransferAll(std::queue<T>& source) {
			std::lock_guard<std::mutex> lock(m_Mutex);
			while (!source.empty()) {
				m_Queue.push(std::move(source.front()));
				source.pop();
			}
		}

		bool Empty() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_Queue.empty();
		}

		size_t Size() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_Queue.size();
		}

		void Clear() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			while (!m_Queue.empty()) {
				m_Queue.pop();
			}
		}

	private:
		std::queue<T> m_Queue;
		std::mutex m_Mutex;
	};
}