#include "api/threading/ThreadPool.h"
#include "Loggers.h"

namespace Chimp {
	ThreadPool::TaskDescription::TaskDescription(TaskDescription&& other) noexcept :
		TaskFunction(std::move(other.TaskFunction)),
		Lock(std::move(other.Lock))
	{
	}

	ThreadPool::TaskDescription::TaskDescription(const Task& task, std::unique_ptr<std::lock_guard<std::mutex>> lock) :
		TaskFunction(task),
		Lock(std::move(lock))
	{
	}

	ThreadPool::ThreadPool(size_t numThreads) :
		m_IsBeingDestroyed(false),
		m_NumThreads(numThreads)
	{
		m_Threads.reserve(numThreads);
		for (size_t i = 0; i < numThreads; ++i) {
			m_Threads.emplace_back([this]() {
				WorkerThreadLogic();
				});
		}
	}

	ThreadPool::~ThreadPool()
	{
		m_IsBeingDestroyed = true;
		m_CV.notify_all();
		for (auto& thread : m_Threads) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}

	void ThreadPool::RunTask(const std::function<void()>& task)
	{
		m_Tasks.EmplaceBack(task, nullptr);
		m_CV.notify_one();
	}

	void ThreadPool::RunTask(const std::function<void()>& task, std::mutex& mtx)
	{
		m_Tasks.EmplaceBack(
			task,
			std::make_unique<std::lock_guard<std::mutex>>(mtx)
		);
		m_CV.notify_one();
	}

	void ThreadPool::WaitUntilTasksExecuted(const std::vector<std::function<void()>>& tasks)
	{
		auto sharedMtx = std::make_shared<std::mutex>();
		auto sharedCv = std::make_shared<std::condition_variable>();
		auto sharedTasksLeft = std::make_shared<size_t>(tasks.size());

		for (const auto& task : tasks) {
			RunTask([task, sharedMtx, sharedCv, sharedTasksLeft]() {
				try {
					task();
				}
				catch (...) {
					Chimp::Loggers::Main().Error("Exception in thread pool task");
				}

				{
					std::lock_guard<std::mutex> lock(*sharedMtx);
					if (--(*sharedTasksLeft) <= 0) {
						sharedCv->notify_one();
					}
				}
				});
		}

		std::unique_lock<std::mutex> lock(*sharedMtx);
		sharedCv->wait(lock, [sharedTasksLeft]() { return *sharedTasksLeft == 0; });
	}


	void ThreadPool::WorkerThreadLogic()
	{
		while (true) {
			std::unique_lock<std::mutex> lock(m_Mutex);
			auto pred = [this]() { return m_IsBeingDestroyed || !m_Tasks.Empty(); };
			if (!pred()) {
				m_CV.wait(lock, pred);
			}

			if (m_IsBeingDestroyed) {
				break;
			}

			std::unique_ptr<TaskDescription> desc = m_Tasks.PeekAndPop();
			if (desc) {
				desc->TaskFunction();
			}
		}
	}
}