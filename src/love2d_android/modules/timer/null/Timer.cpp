/**
* Copyright (c) 2006-2012 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#include "Timer.h"

// LOVE
#include <common/config.h>

#ifdef LOVE_WINDOWS
#	include <windows.h>
#else
#	include <stdlib.h>
#endif

#include <time.h>

namespace love
{
namespace timer
{
namespace null
{

Timer::Timer()
	: m_dt(0)
	, m_lastTime(0)
{
	srand(time(NULL));
}

const char* Timer::getName() const
{
	return "love.timer.null";
}

void Timer::step()
{
	double lCurrentTime = getMicroTime();
	m_dt = (float)(lCurrentTime - m_lastTime);
	m_lastTime = lCurrentTime;
}

void Timer::sleep(double seconds)
{
// 	if (seconds > 0)
// 		love::delay((int) (seconds*1000));
}

double Timer::getDelta() const
{
	return m_dt;
}

int Timer::getFPS() const
{
	return 0;
}

double Timer::getTime() const
{
	return 0;
}

double Timer::getMicroTime() const
{
#ifdef LOVE_WINDOWS
	static __int64 freq = 0;

	if (!freq)
	{
		LARGE_INTEGER temp;
		QueryPerformanceFrequency(&temp);

		freq = (__int64) temp.QuadPart;
	}

	LARGE_INTEGER microTime;
	QueryPerformanceCounter(&microTime);

	// The 64 to 32 bit integer conversion, assuming the fraction part down
	// to microseconds takes 20 bits, should not be a problem unless the
	// system has an uptime of a few decades.
	return (double) microTime.QuadPart / (double) freq;
#else
	timespec lTimeVal;
	clock_gettime(CLOCK_MONOTONIC, &lTimeVal);
	return lTimeVal.tv_sec + (lTimeVal.tv_nsec * 1.0e-9);

// 	timeval lTimeVal;
// 	gettimeofday(&lTimeVal, NULL);
// 	return (lTimeVal.tv_sec * 1000.0) + (lTimeVal.tv_usec / 1000.0);
#endif
}

} // null
} // timer
} // love