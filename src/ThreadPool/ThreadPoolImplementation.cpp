#include <Windows.h>
#include "ThreadPoolImplementation.h"
#include "ThreadsMap.h"

namespace LAB {

	ThreadPoolImplementation::ThreadPoolImplementation() {
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		m_maxThreadNumber = systemInfo.dwNumberOfProcessors * 3;
		if (m_maxThreadNumber > MAX_THREAD_NUMBER) {
			m_maxThreadNumber = MAX_THREAD_NUMBER;
		}
		else if (m_maxThreadNumber < MIN_THREAD_NUMBER) {
			m_maxThreadNumber = MIN_THREAD_NUMBER;
		}
		//Init thread managers
		m_dynamicThreadManager = new DynamicThreadManager(m_maxThreadNumber, MIN_THREAD_NUMBER, 2);
	}


	ThreadPoolImplementation::~ThreadPoolImplementation() {
		delete m_dynamicThreadManager;
	}


	void ThreadPoolImplementation::QueueUserWorkItem(WorkCallback callback, void* params, ThreadPool::ThreadPoolFlags threadFlags) {
		switch (threadFlags) {
		case ThreadPool::ThreadPoolFlags::NORMAL_THREAD:
			m_dynamicThreadManager->QueueUserWorkItem(callback, params);
			break;
		}
	}
}
