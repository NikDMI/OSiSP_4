#include "IThreadManager.h"
#include "../ThreadPool/ThreadsMap.h"
#include "../ThreadInfo/ThreadInfo.h"

namespace LAB {

	IThreadManager::IThreadManager(int maxThreadsNumber, int startThreadsNumber, int localTaskQueueCapacity) :
		m_maxThreadNumber{ maxThreadsNumber },
		m_activeThreadNumber{startThreadsNumber},
		m_localTaskQueueCapacity{localTaskQueueCapacity}
	{
		//////////////////////
		m_threads.resize(maxThreadsNumber);
		m_localTasksQueue.resize(maxThreadsNumber);
		m_suspendedThreadMap = new ThreadsMap(maxThreadsNumber, startThreadsNumber);
		for (int i = 0; i < startThreadsNumber; ++i) {
			m_threads[i] = new ThreadInfo(m_suspendedThreadMap, i, m_localTasksQueue[i], localTaskQueueCapacity);
		}
	}

	IThreadManager::~IThreadManager() {
		for (int i = 0; i < m_threads.size(); i++) {
			if (m_threads[i] != nullptr) delete m_threads[i];
		}
		delete m_suspendedThreadMap;
	}


	int IThreadManager::TryAddPhisicalThread() {
		for (int i = 0; i < m_maxThreadNumber; ++i) {
			if (m_threads[i] == nullptr) {
				m_threads[i] = new ThreadInfo(m_suspendedThreadMap, i, m_localTasksQueue[i], m_localTaskQueueCapacity);
				++m_activeThreadNumber;
				return i;
			}
		}
		return -1;
	}


	bool IThreadManager::TryRemoveFreePhisicalThread() {
		int suspendedThread = m_suspendedThreadMap->TryGetSuspendedThread();
		if (suspendedThread != -1) {
			delete m_threads[suspendedThread];
			m_threads[suspendedThread] = nullptr;
			--m_activeThreadNumber;
			return true;
		}
		return false;
	}
}