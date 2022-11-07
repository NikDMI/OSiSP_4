#pragma once
#include <Windows.h>
#include <queue>
#include "ThreadPool.h"
#include "../TaskInfo/TaskInfo.h"

namespace LAB {

	class TaskQueue {
	public:
		static const UINT MAX_TASKS_IN_QUEUE = MAXUINT32;

		TaskQueue(UINT maxQueueCapacity = MAX_TASKS_IN_QUEUE);
		~TaskQueue();

		bool TryPushTask(const TaskInfo& task);
		bool TryPopTask(TaskInfo& task);
	private:
		CRITICAL_SECTION m_lockObject;
		UINT m_queueCapacity;
		std::queue<TaskInfo> m_tasksQueue;
	};
}