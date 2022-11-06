#pragma once
#include <numeric>
#include "ThreadPoolImplementation.h"

namespace LAB {

	class ThreadsMap {
	public:
		ThreadsMap(int totalThreadsCount, int suspendThreadsCount);

		//Return number of the first founded suspended thread or -1
		int TryGetSuspendedThread();
		void SetThreadState(unsigned int threadNumber, bool isSuspended);

	private:
		using state_t = int64_t;
		state_t m_threadsInfo[(ThreadPoolImplementation::MAX_THREAD_NUMBER + (sizeof(state_t) * CHAR_BIT) - 1) / (sizeof(state_t) * CHAR_BIT)];
	};
}