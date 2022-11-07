#include "DynamicThreadManager.h"
#include "../ThreadInfo/ThreadInfo.h"
#include "../ThreadPool/ThreadsMap.h"


namespace LAB {

	DWORD WINAPI ManagerThreadProc(LPVOID param);
	VOID APIENTRY CheckDelayTasks(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue, DWORD dwTimerHighValue);

	DynamicThreadManager::DynamicThreadManager(int maxThreadsNumber, int startThreadsNumber, int localTaskQueueCapacity):
		IThreadManager{maxThreadsNumber, startThreadsNumber, localTaskQueueCapacity}
	{
		m_waitableTimer = CreateWaitableTimer(NULL, FALSE, NULL);
		m_managerThread = CreateThread(NULL, 0, ManagerThreadProc, this, 0, NULL);
	}


	DynamicThreadManager::~DynamicThreadManager() {
		TerminateThread(m_managerThread, 0);
		DeleteObject(m_managerThread);
		DeleteObject(m_waitableTimer);
	}


	void DynamicThreadManager::QueueUserWorkItem(WorkCallback callback, void* params) {
		int suspendedThread = m_suspendedThreadMap->TryGetSuspendedThread();
		if (suspendedThread != -1) {
			if (m_threads[suspendedThread]->TrySetWorkItem(callback, params)) {
				return;
			}
		}
		//If all threads are running (try to add task by cycle)
		int i = m_cycleThreadIndex;
		do {
			if (m_threads[i]->TrySetWorkItem(callback, params)) {
				m_cycleThreadIndex = (i + 1) % m_activeThreadNumber;
				return;
			}
			i = (i + 1) % m_activeThreadNumber;
		} while (i != m_cycleThreadIndex);
		m_cycleThreadIndex = 0;
		//Create new phisical thread if available
		int newThreadIndex = TryAddPhisicalThread();
		if (newThreadIndex != -1) {
			if (m_threads[newThreadIndex]->TrySetWorkItem(callback, params)) {
				return;
			}
		}
		//All threads are busy and max threads count is created (add task to global queue and wait)
		if (m_globalTaskQueue.TryPushTask({ callback, params })) {
			//Set waitable timer to perform this callback task later
			if (!InterlockedCompareExchange(&m_isTimerOn, 1, 0)) {
				FILETIME ft;  SYSTEMTIME st;  LARGE_INTEGER li;
				GetSystemTime(&st);              // Gets the current system time
				SystemTimeToFileTime(&st, &ft);
				ft.dwLowDateTime += 1000000;//add 100 ms - dangerous overflow!!!
				li.LowPart = ft.dwLowDateTime;
				li.HighPart = ft.dwHighDateTime;
				BOOL b = SetWaitableTimer(m_waitableTimer, &li, 0, CheckDelayTasks, this, TRUE);
			}
		}
		else {
			throw Exception(L"All system resourses was used");
		}
	}


	DWORD WINAPI ManagerThreadProc(LPVOID param) {
		DynamicThreadManager* managerObject = (DynamicThreadManager*)param;
		while (true) {
			WaitForSingleObjectEx(managerObject->m_waitableTimer, INFINITE, TRUE);
			//CancelWaitableTimer(managerObject->m_waitableTimer);
			CheckDelayTasks((void*)managerObject, 0, 0);
		}
	}


	VOID APIENTRY CheckDelayTasks(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue, DWORD dwTimerHighValue) {
		DynamicThreadManager* managerObject = (DynamicThreadManager*)lpArgToCompletionRoutine;
		InterlockedCompareExchange(&managerObject->m_isTimerOn, 0, 1);
		TaskInfo task;
		while (managerObject->m_globalTaskQueue.TryPopTask(task)) {
			managerObject->QueueUserWorkItem(task.callbackFunction, task.params);
		}
		
	}
}