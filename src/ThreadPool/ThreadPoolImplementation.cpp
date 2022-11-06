#include <Windows.h>
#include "ThreadPoolImplementation.h"

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
		//Initialize minimal number of threads
		for (int i = 0; i < MIN_THREAD_NUMBER; ++i) {
			m_poolThreads.push_back(new ThreadInfo());
		}
	}


	ThreadPoolImplementation::~ThreadPoolImplementation() {
		for (auto thread : m_poolThreads) {
			delete thread;
		}
	}
}
