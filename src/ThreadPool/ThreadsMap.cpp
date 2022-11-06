#include "ThreadsMap.h"
#include "../Config.h"


namespace LAB {

	ThreadsMap::ThreadsMap(int totalThreadsCount, int suspendThreadsCount) {
		if (totalThreadsCount <= 0 || totalThreadsCount > ThreadPoolImplementation::MAX_THREAD_NUMBER || suspendThreadsCount > totalThreadsCount) 
			throw ExceptionBadArgs();

	}

}