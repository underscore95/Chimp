#pragma once

#include "stdafx.h"

namespace Chimp {
	class Engine;
	class TaskScheduler {
	private:
		struct Task {
			float RunTimeStamp;
			std::function<void()> Function;
		};
		struct TaskComparator {
			bool operator()(const Task& t1, const Task& t2) {
				return t1.RunTimeStamp > t2.RunTimeStamp;
			}
		};
		
	public:
		TaskScheduler(Chimp::Engine& engine);

		// Add a task to the scheduler
		// delay - The time in seconds to wait before running the task
		// function - The function to run
		void RunDelayedTask(float delay, std::function<void()> function);

		// Run all tasks that are due
		void Update();

	private:
		Chimp::Engine& m_Engine;
		std::priority_queue<Task, std::vector<Task>, TaskComparator> m_Tasks;
	};
}