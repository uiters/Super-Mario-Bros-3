#pragma once

#include <d3d9.h>

class Timer
{
private:
	DWORD timeStart;
	bool started;
public:
	Timer();

	bool IsStarted() { return this->started; }
	void Start();
	void Reset();
	DWORD ElapsedTime();
};

