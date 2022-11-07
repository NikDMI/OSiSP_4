#pragma once
#include "IThreadManager.h"
#include "../ThreadPool/TaskQueue.h"
#include <vector>


namespace LAB{

	class DynamicThreadManager final: public IThreadManager{
	public:
		DynamicThreadManager(int maxThreadsNumber, int startThreadsNumber, int localTaskQueueCapacity);
		~DynamicThreadManager() override;
		void QueueUserWorkItem(WorkCallback callback, void* params) override;

	private:
		int m_cycleThreadIndex = 0;
		TaskQueue m_globalTaskQueue{};

		HANDLE m_managerThread;
		HANDLE m_waitableTimer;
		LONG m_isTimerOn = 0;

		friend DWORD WINAPI ManagerThreadProc(LPVOID param);
		friend VOID APIENTRY CheckDelayTasks(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue, DWORD dwTimerHighValue);
	};
}
