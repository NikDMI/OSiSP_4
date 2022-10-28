#include <Windows.h>
#include "ThreadPoolImplementation.h"

namespace LAB {

	ThreadPoolImplementation::ThreadPoolImplementation() {
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		m_maxThreadNumber = systemInfo.dwNumberOfProcessors * 3;
		m_threadsCountSemaphore = CreateSemaphore(NULL, 0, m_maxThreadNumber, NULL);
		if (m_threadsCountSemaphore == INVALID_HANDLE_VALUE) throw Exception(L"Can't create semaphore in ThreadPoolImpl");
	}
}
