#pragma once
#include <numeric>
#include "ThreadPoolImplementation.h"

namespace LAB {

	class ThreadsMap {
	public:
		ThreadsMap(int totalThreadsCount, int suspendThreadsCount);
		~ThreadsMap();

		//Return number of the first founded suspended thread or -1
		int TryGetSuspendedThread();
		void SetThreadState(unsigned int threadNumber, bool isSuspended);

	private:
		using state_t = uint64_t;
		const static int MAX_THREAD_STATE_IN_VAR = sizeof(state_t) * CHAR_BIT;
		//From right to left   <- 000000011 000100011 <-      1 is suspended thread
		state_t m_threadsSuspendedInfo[(ThreadPoolImplementation::MAX_THREAD_NUMBER + (MAX_THREAD_STATE_IN_VAR) - 1) / MAX_THREAD_STATE_IN_VAR];
		CRITICAL_SECTION m_lockCriticalSection;
	};
}