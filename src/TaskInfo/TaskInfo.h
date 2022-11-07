#pragma once


namespace LAB {

	using WorkCallback = void (*)(void* params);

	class TaskInfo {
	public:
		WorkCallback callbackFunction;
		void* params;
	};
}
