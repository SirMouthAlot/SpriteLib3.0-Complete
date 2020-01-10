#ifndef __TIMER_H__
#define __TIMER_H__

#include <ctime>

#define ONE_OVER_CLOCKS_PER_SEC 1.f / (float)CLOCKS_PER_SEC;

class Timer abstract
{
public:
	//Current clock and last clock
	static clock_t currentClock, lastClock;
	//Change in time (time since last frame) and total game time
	static float deltaTime, time;

	//Resets the clock to zero
	static void Reset();
	//Updates the clock using ctime
	static void Update();
};

#endif // !__TIMER_H__
