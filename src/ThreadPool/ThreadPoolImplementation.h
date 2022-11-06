#pragma once
#include "ThreadPool.h"
#include "../ThreadInfo/ThreadInfo.h"
#include <vector>

namespace LAB {

	class ThreadsMap;

	/***
	* This class has only one object in the programm
	* The main responsibility of this class - is to navigate tasks to thread (thread manager)
	*/
	class ThreadPoolImplementation {
	public:
		void QueueUserWorkItem(ThreadPool::WorkCallback callback, void* params, ThreadPool::ThreadPoolFlags threadFlags);
		/*
		* Get resourses for minThreadNumber threads
		* if minThreadNumber == 0 -> get max available threads
		*/
		void SetMinimalActiveThreadsNumber(int minThreadNumber = 0) noexcept;

		const static int MAX_THREAD_NUMBER = 30;
		const static int MIN_THREAD_NUMBER = 4;
	private:
		
		ThreadPoolImplementation();
		~ThreadPoolImplementation();

		//Section of threads information
		int m_maxThreadNumber;
		std::vector<ThreadInfo*> m_poolThreads { MAX_THREAD_NUMBER };		//Threads of OS, that ThreadPool was created
		ThreadsMap* m_threadsMap;

		friend class ThreadPool;
	};
}
