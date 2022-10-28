#pragma once
#include <Windows.h>
#include "../Config.h"

namespace LAB {

	class ThreadPoolImplementation;
	/**
	*Class singelton of thread pool 
	*/
	class ThreadPool {
	public:
		using WorkCallback = void (*)(void* params);
		enum ThreadPoolFlags {NORMAL_THREAD, IO_THREAD};

		ThreadPool();
		~ThreadPool();
		void QueueUserWorkItem(WorkCallback callback, void* params, ThreadPoolFlags threadFlags);

	private:
		ThreadPool(int);	//Init constructor

		static ThreadPoolImplementation* m_threadPool;
		bool m_isThreadOwner = false;
		friend class ConfigThreadPool;
	};
}
