#pragma once

#include <Windows.h>

class GameTime
{
	static DWORD lastTime;
	static DWORD currTime;
public:
	//GameTime() {}
	static void init();
	static void update();
	static DWORD getCurrTime();
	static DWORD getTimeElapsed();
};

inline float Lerp(float a, float b, float t)
{
	return a - (a * t) + (b * t);
}
