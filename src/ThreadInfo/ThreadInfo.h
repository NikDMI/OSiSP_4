#pragma once
#include <Windows.h>
#include "../Config.h"
#include "../ThreadPool/ThreadPool.h"

namespace LAB {
	
	class ThreadsMap;
	/*
	* This class represents logical info about thread for thread pool
	*/
	class ThreadInfo {
	public:
		struct WorkItemData {
			ThreadPool::WorkCallback callback;
			void* params;
		};
	private:
		ThreadInfo(ThreadsMap* threadMap, int indexInMap);
		~ThreadInfo();

		bool TrySetWorkItem(ThreadPool::WorkCallback, void* params);


		enum class ThreadEvent {GET_TASK, TERMINATE};
		ThreadEvent m_currentThreadEvent;
		HANDLE m_threadEvent;
		HANDLE m_threadHandle;
		WorkItemData m_itemData;
		LONG m_isThreadBusy;
		ThreadsMap* m_sharedThreadsMap;
		int m_indexInThreadsMap;

		
		friend DWORD WINAPI ThreadProc(LPVOID param);
		friend class ThreadPoolImplementation;
	};
	
}
