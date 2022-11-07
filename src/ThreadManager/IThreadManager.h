#pragma once
//#include "../ThreadPool/ThreadPool.h"
#include "../TaskInfo/TaskInfo.h"
#include <vector>

namespace LAB {

	class ThreadsMap;
	class ThreadInfo;
	class TaskQueue;


	/**
	* This class is used to manage task into threads, create new phisical threads and free thread resources
	*/
	class IThreadManager {
	public:
		IThreadManager(int maxThreadsNumber, int startThreadsNumber, int localTaskQueueCapacity);
		virtual ~IThreadManager();
		virtual void QueueUserWorkItem(WorkCallback callback, void* params) = 0;
	protected:
		int m_maxThreadNumber;
		int m_activeThreadNumber;
		ThreadsMap* m_suspendedThreadMap;
		std::vector<ThreadInfo*> m_threads;
		std::vector<TaskQueue*> m_localTasksQueue;
		int m_localTaskQueueCapacity;

		//-1 - if unsuccessful
		int TryAddPhisicalThread();
		bool TryRemoveFreePhisicalThread();
	};

}
