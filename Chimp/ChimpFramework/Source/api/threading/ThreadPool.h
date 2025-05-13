#pragma once

#include "stdafx.h"
#include "api/threading/ThreadSafeQueue.h"

namespace Chimp {
	// Create a thread pool that can execute tasks asynchronously
	// wip tasks will be discarded upon destruction of the thread pool
	class ThreadPool {
	public:
		typedef std::function<void()> Task;
		struct TaskDescription {
			DISABLE_COPY(TaskDescription);
			TaskDescription(const Task& task, std::unique_ptr<std::lock_guard<std::mutex>> lock);
			TaskDescription(TaskDescription&& other) noexcept;

			Task TaskFunction; // This is the actual task
			std::unique_ptr<std::lock_guard<std::mutex>> Lock; // Lock a mutex while the task is running (may be nullptr)
		};
	public:
		ThreadPool(size_t numThreads);
		~ThreadPool();

		// Run a task asynchronously
		// If a mutex is provided, the task will lock the mutex until it is finished
		void RunTask(const std::function<void()>& task);
		void RunTask(const std::function<void()>& task, std::mutex& mtx);

		// Run a vector of tasks asynchronously and but block until all tasks are finished
		void WaitUntilTasksExecuted(const std::vector<std::function<void()>>& tasks);

		[[nodiscard]] size_t GetNumThreads() const { return m_NumThreads; }

	private:
		void WorkerThreadLogic();

	private:
		size_t m_NumThreads;
		std::vector<std::thread> m_Threads;
		ThreadSafeQueue<TaskDescription> m_Tasks;
		bool m_IsBeingDestroyed = false;
		std::mutex m_Mutex;
		std::condition_variable m_CV;
	};
}