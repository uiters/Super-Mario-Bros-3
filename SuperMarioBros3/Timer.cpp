#include "Timer.h"


Timer::Timer()
{
	timeStart = -1;
	started = false;
}

void Timer::Start()
{
	timeStart = GetTickCount64();
	started = true;
}

void Timer::Reset()
{
	timeStart = -1;
	started = false;
}

ULONGLONG Timer::ElapsedTime()
{
	if (timeStart == -1) return 0;
	return GetTickCount64() - timeStart;
}