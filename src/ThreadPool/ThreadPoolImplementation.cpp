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
		//Initialize minimal number of threads
		m_threadsMap = new ThreadsMap(m_maxThreadNumber, MIN_THREAD_NUMBER);
		//for (int i = 0; i < MIN_THREAD_NUMBER; ++i) {
		//	m_poolThreads[i] = new ThreadInfo(m_threadsMap, i);
		//}
	}


	ThreadPoolImplementation::~ThreadPoolImplementation() {
		//for (auto thread : m_poolThreads) {
		//	delete thread;
		//}
		delete m_threadsMap;
	}
}
