#pragma once
#include "ThreadPool.h"
#include "../ThreadInfo/ThreadInfo.h"
#include <vector>
#include <queue>

namespace LAB {
	/***
	* This class has only one object in the programm
	*/
	class ThreadPoolImplementation {
	public:
		void QueueUserWorkItem(ThreadPool::WorkCallback callback, void* params, ThreadPool::ThreadPoolFlags threadFlags);

	private:
		ThreadPoolImplementation();

		//Section of threads information
		int m_maxThreadNumber;
		HANDLE m_threadsCountSemaphore;
		std::vector<ThreadInfo*> m_poolThreads;		//Threads of OS, that ThreadPool was created
		//std::queue<ThreadInfo*> m_freeThreads;		//Queue of free threads in thread pool

		friend class ThreadPool;
	};
}
