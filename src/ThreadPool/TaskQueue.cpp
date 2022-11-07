#include "TaskQueue.h"

namespace LAB {

	TaskQueue::TaskQueue(UINT maxQueueCapacity) :m_queueCapacity { maxQueueCapacity } {
		InitializeCriticalSectionAndSpinCount(&m_lockObject, 2000);
	}


	TaskQueue::~TaskQueue() {
		DeleteCriticalSection(&m_lockObject);
	}


	bool TaskQueue::TryPushTask(const TaskInfo& task) {
		EnterCriticalSection(&m_lockObject);
		if (m_tasksQueue.size() == m_queueCapacity) {
			LeaveCriticalSection(&m_lockObject);
			return false;
		}
		m_tasksQueue.push(task);
		LeaveCriticalSection(&m_lockObject);
		return true;
	}


	bool TaskQueue::TryPopTask(TaskInfo& task) {
		EnterCriticalSection(&m_lockObject);
		if (m_tasksQueue.size() == 0) {
			LeaveCriticalSection(&m_lockObject);
			return false;
		}
		task = m_tasksQueue.front();
		m_tasksQueue.pop();
		LeaveCriticalSection(&m_lockObject);
		return true;
	}

}