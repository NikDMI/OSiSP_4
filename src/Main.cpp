#include <Windows.h>
#include "ThreadPool/ThreadPool.h"
#include "FileParser/FileParser.h"
#include <fstream>

using namespace LAB;
using namespace std;

HANDLE waitEvent;
HANDLE waitOs;
void callbackFunction1(void* params);
void callbackFunction2(void* params);
struct Data {
	ofstream* of;
	int num;
};
LONG n = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
	ThreadPool* threadPool = new ThreadPool();
	ofstream* ostream = new ofstream();
	ostream->open(L"FileOut.txt", ios_base::out | ios_base::trunc);
	waitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	waitOs = CreateEvent(NULL, FALSE, TRUE, NULL);
	/*
	threadPool->QueueUserWorkItem(callbackFunction2, ostream, ThreadPool::NORMAL_THREAD);
	for (int i = 0; i < 500; i++) {
		Data* d = new Data{ ostream, i };
		threadPool->QueueUserWorkItem(callbackFunction1, d, ThreadPool::NORMAL_THREAD);
	}
	WaitForSingleObject(waitEvent, INFINITE);
	(*ostream) << "TOTAL " << n;
	*/
	//delete threadPool;
	SortTextFile(L"Test.txt", L"Out.txt");
	return 0;
}

void callbackFunction1(void* params) {
	Data* data = (Data*)params;
	for (int i = 0; i < 1; i++) {
		//WaitForSingleObject(waitOs, INFINITE);
		//(*data->of) <<"Function - "<< data->num << " " << i << endl;
		//SetEvent(waitOs);
		Sleep(10);
	}
	WaitForSingleObject(waitOs, INFINITE);
	(*data->of) << "*** F"<<data->num<<"end * **"<<endl;
	SetEvent(waitOs);
	InterlockedAdd(&n, 1);
}

void callbackFunction2(void* params) {
	ofstream* os = (ofstream*)params;
	for (int i = 0; i < 30; i++) {
		WaitForSingleObject(waitOs, INFINITE);
		(*os) << "F____: " << i << endl;
		SetEvent(waitOs);
		Sleep(150);
	}
	WaitForSingleObject(waitOs, INFINITE);
	(*os) << "*** ALL TASK END ***"<<endl;
	SetEvent(waitOs);
	SetEvent(waitEvent);
}