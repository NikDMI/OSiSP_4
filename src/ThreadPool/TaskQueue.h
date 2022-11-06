#pragma once
#include <Windows.h>
#include <queue>
#include "ThreadPool.h"

namespace LAB {

	class TaskQueue {
	public:
		struct TaskInfo {
			ThreadPool::WorkCallback callbackFunction;
			void* params;
		};

		TaskQueue();
		~TaskQueue();

		void PushTask(TaskInfo task);
		bool PopTask(TaskInfo& task);
	private:
		CRITICAL_SECTION m_lockObject;
		std::queue<TaskInfo> m_tasks;
	};
}