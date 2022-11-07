#include "IThreadManager.h"
#include "../ThreadPool/ThreadsMap.h"
#include "../ThreadInfo/ThreadInfo.h"

namespace LAB {

	IThreadManager::IThreadManager(int maxThreadsNumber, int startThreadsNumber) : m_maxThreadNumber{ maxThreadsNumber },
		m_activeThreadNumber{startThreadsNumber} {
		//////////////////////
		m_threads.resize(maxThreadsNumber);
		m_localTasksQueue.resize(maxThreadsNumber);
		m_threadMap = new ThreadsMap(maxThreadsNumber, startThreadsNumber);
		for (int i = 0; i < startThreadsNumber; ++i) {
			m_threads[i] = new ThreadInfo(m_threadMap, i, m_localTasksQueue[i]);
		}
	}

	IThreadManager::~IThreadManager() {
		for (int i = 0; i < m_threads.size(); i++) {
			if (m_threads[i] != nullptr) delete m_threads[i];
		}
		delete m_threadMap;
	}
}