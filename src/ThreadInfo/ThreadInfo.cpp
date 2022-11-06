#include "ThreadInfo.h"
#include "../ThreadPool/ThreadsMap.h"

namespace LAB {

	DWORD WINAPI ThreadProc(LPVOID param);

	ThreadInfo::ThreadInfo(ThreadsMap* threadMap, int indexInMap) : m_sharedThreadsMap{ threadMap }, m_indexInThreadsMap{ indexInMap } {
		if (threadMap == nullptr) throw ExceptionBadArgs();
		//Auto reset event
		m_threadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_isThreadBusy = 0;
		m_threadHandle = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
	}


	bool ThreadInfo::TrySetWorkItem(ThreadPool::WorkCallback callbackFunction, void* params) {
		if (!InterlockedCompareExchange(&m_isThreadBusy, 1, 0)) {
			m_sharedThreadsMap->SetThreadState(m_indexInThreadsMap, false);
			m_currentThreadEvent = ThreadInfo::ThreadEvent::GET_TASK;
			m_itemData.callback = callbackFunction;
			m_itemData.params = params;
			SetEvent(m_threadEvent);
			return true;
		}
		else {
			return false;
		}
	}


	ThreadInfo::~ThreadInfo() {
		m_currentThreadEvent = ThreadInfo::ThreadEvent::TERMINATE;
		SetEvent(m_threadEvent);
		WaitForSingleObject(m_threadHandle, INFINITE);
		CloseHandle(m_threadEvent);
		CloseHandle(m_threadHandle);
	}


	DWORD WINAPI ThreadProc(LPVOID param) {
		ThreadInfo* threadInfo = (ThreadInfo*)param;
		HANDLE waitEvent = threadInfo->m_threadEvent;
		while (true) {
			threadInfo->m_sharedThreadsMap->SetThreadState(threadInfo->m_indexInThreadsMap, true);
			WaitForSingleObject(waitEvent, INFINITE);
			ThreadPool::WorkCallback callbackFunction;
			//Choose thread activity
			switch (threadInfo->m_currentThreadEvent) {
			case ThreadInfo::ThreadEvent::GET_TASK:
				callbackFunction = threadInfo->m_itemData.callback;
				callbackFunction(threadInfo->m_itemData.params);
				InterlockedExchange(&threadInfo->m_isThreadBusy, 0);
				break;

			case ThreadInfo::ThreadEvent::TERMINATE:
				InterlockedExchange(&threadInfo->m_isThreadBusy, 0);
				return 0;
			}
		}
	}
}