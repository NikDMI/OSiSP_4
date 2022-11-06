#include "ThreadsMap.h"
#include "../Config.h"


namespace LAB {

	ThreadsMap::ThreadsMap(int totalThreadsCount, int suspendThreadsCount) {
		if (totalThreadsCount <= 0 || totalThreadsCount > ThreadPoolImplementation::MAX_THREAD_NUMBER || suspendThreadsCount > totalThreadsCount) 
			throw ExceptionBadArgs();
		for (auto& x : m_threadsSuspendedInfo) x = 0;
		int currentVarIndex = 0;//var in m_threadsSuspendedInfo array
		while (suspendThreadsCount > 0) {
			if (suspendThreadsCount >= MAX_THREAD_STATE_IN_VAR) {
				m_threadsSuspendedInfo[currentVarIndex] = (state_t)~0;
				currentVarIndex++;
				suspendThreadsCount -= MAX_THREAD_STATE_IN_VAR;
			}
			else {
				state_t state = (state_t)~0;
				state >>= MAX_THREAD_STATE_IN_VAR - suspendThreadsCount;
				suspendThreadsCount = 0;
			}
		}
	}


	int ThreadsMap::TryGetSuspendedThread() {
		int startIndex = 0;
		for (auto states : m_threadsSuspendedInfo) {
			//All threads are active
			if (states == 0) {
				startIndex += MAX_THREAD_STATE_IN_VAR;
				continue;
			}
			state_t checkNumber = 0x01;
			while (!(checkNumber & states)) {
				checkNumber <<= 1;
				startIndex++;
			}
			return startIndex;
		}
		return -1;
	}


	void ThreadsMap::SetThreadState(unsigned int threadNumber, bool isSuspended) {
		int varIndex = threadNumber / MAX_THREAD_STATE_IN_VAR;
		if (varIndex >= sizeof(m_threadsSuspendedInfo) / sizeof(state_t)) {
			throw Exception(L"threadNumber out of range");
		}
		threadNumber -= varIndex * MAX_THREAD_STATE_IN_VAR;
		state_t bitMask = ((state_t)1) << threadNumber;
		state_t currentThreadState = m_threadsSuspendedInfo[varIndex] & bitMask;
		if (isSuspended && !currentThreadState) {
			m_threadsSuspendedInfo[varIndex] |= bitMask;
		}
		else if (!isSuspended && currentThreadState) {
			bitMask = ~bitMask;
			m_threadsSuspendedInfo[varIndex] &= bitMask;
		}
	}

}