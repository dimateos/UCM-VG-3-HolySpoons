#include "Timer.h"

Timer::Timer()
{
}

Timer::Timer(double time) : duration(time)
{
}

Timer::~Timer()
{
}

//Changes the timer's duration
void Timer::setDuration(double time)
{
	duration = time;
}

//Starts the timer with a previously configured timer duration
void Timer::start()
{
	active = true;
	timeLeft = duration;
}

//Starts the timer with a new timer duration(time)
void Timer::start(double time)
{
	active = true;
	timeLeft = duration = time;
}

//Stops the timer
void Timer::stop()
{
	active = false;
}

//Returns true if an active timer has ended, false otherwise
bool Timer::checkEnd()
{
	if (timeLeft <= 0 && active) {
		active = false;
		return true;
	}
	return false;
}

bool Timer::isActive()
{
	return active;
}

//Updates the timer's remaining duration if it's active and checks end
bool Timer::update(double timePassed)
{
	if (active)
		timeLeft -= timePassed;
	return checkEnd();
}


