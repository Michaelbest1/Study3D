#include "Util.h"

#include <windows.h>
#include <windowsx.h>

DWORD GameTime::lastTime = 0;
DWORD GameTime::currTime = 0;

void GameTime::init()
{
	lastTime = 0;
	currTime = timeGetTime();
}

void GameTime::update()
{
	lastTime = currTime;
	currTime = timeGetTime();
}

DWORD GameTime::getCurrTime()
{
	return currTime;
}

DWORD GameTime::getTimeElapsed()
{
	return currTime - lastTime;
}