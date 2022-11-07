#include "ThreadPool.h"
#include <exception>

#include "../ThreadPool/ThreadPoolImplementation.h"

namespace LAB {

	ThreadPoolImplementation* ThreadPool::m_threadPool = nullptr;
	CRITICAL_SECTION ThreadPool::m_lockObject;

	//Private init constructor
	ThreadPool::ThreadPool(int) : m_isThreadOwner{ true } {
		m_threadPool = new ThreadPoolImplementation();
		InitializeCriticalSectionAndSpinCount(&m_lockObject, 2000);
	}


	//Public constructor for users
	ThreadPool::ThreadPool(): m_isThreadOwner{ false } {
		if (m_threadPool == nullptr) throw std::exception("Thread pool is not allocated");
	}


	ThreadPool::~ThreadPool() {
		if (m_isThreadOwner) {
			delete m_threadPool;
			DeleteCriticalSection(&m_lockObject);
		}
	}


	//Function delegation
	void ThreadPool::QueueUserWorkItem(WorkCallback callback, void* params, ThreadPoolFlags threadFlags) {
		EnterCriticalSection(&m_lockObject);
		m_threadPool->QueueUserWorkItem(callback, params, threadFlags);
		LeaveCriticalSection(&m_lockObject);
	}



	///CONFIGS 

	class ConfigThreadPool {
	public:
		ConfigThreadPool() {
		}

		~ConfigThreadPool() {
		}
	private:
		ThreadPool m_mainThreadPool {1};
	};

	//This object is used to initialize thread pool
	ConfigThreadPool configThreadPool;
}