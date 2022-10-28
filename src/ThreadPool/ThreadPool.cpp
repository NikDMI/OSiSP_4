#include "ThreadPool.h"
#include <exception>

#include "../ThreadPool/ThreadPoolImplementation.h"

namespace LAB {

	ThreadPoolImplementation* ThreadPool::m_threadPool = nullptr;

	//Private init constructor
	ThreadPool::ThreadPool(int) : m_isThreadOwner{ true } {
		m_threadPool = new ThreadPoolImplementation();
	}


	//Public constructor for users
	ThreadPool::ThreadPool(): m_isThreadOwner{ false } {
		if (m_threadPool == nullptr) throw std::exception("Thread pool is not allocated");
	}


	ThreadPool::~ThreadPool() {
		if (m_isThreadOwner) {
			delete m_threadPool;
		}
	}

	
	class ConfigThreadPool {
	public:
		ConfigThreadPool() {
		}

		~ConfigThreadPool() {
		}
	private:
		ThreadPool m_mainThreadPool {1};
	};

	ConfigThreadPool configThreadPool;
}