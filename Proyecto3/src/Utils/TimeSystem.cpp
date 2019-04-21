#include "TimeSystem.h"
#include "Windows.h" //platform based timing

float TimeSystem::stepTime = 0.0f;
double TimeSystem::PCFreq = 0.0;
__int64 TimeSystem::CounterStart = 0;
__int64 TimeSystem::CounterLast = 0;

void TimeSystem::StartCounter() {
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart) /*/ 1000.0*/;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	CounterLast = CounterStart;
}

double TimeSystem::GetCounter() {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t = double(li.QuadPart - CounterLast) / PCFreq;
	CounterLast = li.QuadPart;
	return t;
}
