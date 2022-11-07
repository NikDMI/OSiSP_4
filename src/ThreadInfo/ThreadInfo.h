#pragma once
#include <Windows.h>
#include "../Config.h"
#include "../ThreadPool/ThreadPool.h"
#include "../ThreadPool/TaskQueue.h"
#include "../TaskInfo/TaskInfo.h"

namespace LAB {
	
	class ThreadsMap;
	/*
	* This class represents logical info about thread for thread pool
	*/
	class ThreadInfo {
	public:
		//struct WorkItemData {
		//	ThreadPool::WorkCallback callback;
		//	void* params;
		//};
		bool TrySetWorkItem(WorkCallback, void* params);

	private:
		ThreadInfo(ThreadsMap* threadMap, int indexInMap, TaskQueue*& outLocalTaskQueue, int taskQueueCapacity = 1);
		~ThreadInfo();



		enum class ThreadEvent {GET_TASK, TERMINATE};
		ThreadEvent m_currentThreadEvent;

		HANDLE m_threadEvent;
		HANDLE m_threadHandle;
		LONG m_isThreadBusy;

		//WorkItemData m_itemData;

		ThreadsMap* m_sharedThreadsMap;
		int m_indexInThreadsMap;

		TaskQueue* m_taskQueue;

		
		friend DWORD WINAPI ThreadProc(LPVOID param);
		friend class IThreadManager;
	};
	
}
