#include "Timer.h"

clock_t Timer::lastClock = 0L;
clock_t Timer::currentClock = 0L;

float Timer::time = 0.f;
float Timer::deltaTime = 0.f;

void Timer::Reset()
{
	//Grabs current clock and stores it in last and current clock
	lastClock = clock();
	currentClock = clock();
	//Resets the total time passed and time since last frame
	time = 0.f;
	deltaTime = 0.f;
}

void Timer::Update()
{
	//Moves current clock to the last clock
	lastClock = currentClock;
	//Creates a new current clock at the current clock time
	currentClock = clock();
	//Gets the difference betweent he last clock and the current clock
	//Then divides it by Clocks per second (via multiplying 1.f/CLOCKS_PER_SEC)
	deltaTime = (currentClock - lastClock) * ONE_OVER_CLOCKS_PER_SEC;
	//Adds delta time to the total time passed (since last reset)
	time += deltaTime;
}
