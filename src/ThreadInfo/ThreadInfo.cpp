#include "ThreadInfo.h"
#include "../ThreadPool/ThreadsMap.h"
#include "../ThreadPool/TaskQueue.h"
#include "../TaskInfo/TaskInfo.h"

namespace LAB {

	DWORD WINAPI ThreadProc(LPVOID param);

	ThreadInfo::ThreadInfo(ThreadsMap* threadMap, int indexInMap, TaskQueue*& outLocalTaskQueue, int taskQueueCapacity) :
		m_sharedThreadsMap{ threadMap },
		m_indexInThreadsMap{ indexInMap }
	{
		if (threadMap == nullptr) throw ExceptionBadArgs();
		//Auto reset event
		m_threadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		//Local tasks queue
		m_taskQueue = new TaskQueue(taskQueueCapacity);
		outLocalTaskQueue = m_taskQueue;
		//OS thread
		m_isThreadBusy = 0;
		m_threadHandle = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
	}


	bool ThreadInfo::TrySetWorkItem(WorkCallback callbackFunction, void* params) {
		bool isTaskAddedToQueue = m_taskQueue->TryPushTask({ callbackFunction, params });
		if (!isTaskAddedToQueue) {
			return false;
		}
		if (!InterlockedCompareExchange(&m_isThreadBusy, 1, 0)) {	//If thread was suspended
			m_sharedThreadsMap->SetThreadState(m_indexInThreadsMap, false);
			m_currentThreadEvent = ThreadInfo::ThreadEvent::GET_TASK;
			SetEvent(m_threadEvent);
			return true;
		}
		else {	//If thread is running
			return isTaskAddedToQueue;
		}
	}


	ThreadInfo::~ThreadInfo() {
		while (InterlockedCompareExchange(&m_isThreadBusy, 0, 0)) {}
		m_currentThreadEvent = ThreadInfo::ThreadEvent::TERMINATE;
		SetEvent(m_threadEvent);
		WaitForSingleObject(m_threadHandle, INFINITE);
		CloseHandle(m_threadEvent);
		CloseHandle(m_threadHandle);
		delete m_taskQueue;
	}


	DWORD WINAPI ThreadProc(LPVOID param) {
		ThreadInfo* threadInfo = (ThreadInfo*)param;
		HANDLE waitEvent = threadInfo->m_threadEvent;
		TaskInfo currentTask;
		while (true) {
			threadInfo->m_sharedThreadsMap->SetThreadState(threadInfo->m_indexInThreadsMap, true);
			WaitForSingleObject(waitEvent, INFINITE);
			//Choose thread activity
			switch (threadInfo->m_currentThreadEvent) {
			case ThreadInfo::ThreadEvent::GET_TASK:
				while (threadInfo->m_taskQueue->TryPopTask(currentTask)) {
					currentTask.PerformSynchronicTask();
				}
				InterlockedExchange(&threadInfo->m_isThreadBusy, 0);
				while (threadInfo->m_taskQueue->TryPopTask(currentTask)) {
					currentTask.PerformSynchronicTask();
				}
				break;

			case ThreadInfo::ThreadEvent::TERMINATE:
				InterlockedExchange(&threadInfo->m_isThreadBusy, 0);
				return 0;
			}
		}
	}
}