#include "Timer.h"

napTimer::napTimer()
{
}

napTimer::napTimer(double time) : duration(time)
{
}

napTimer::~napTimer()
{
}

//Changes the timer's duration
void napTimer::setDuration(double time)
{
	duration = time;
}

//Starts the timer with a previously configured timer duration
void napTimer::start()
{
	active = true;
	resetTimeLeft();
}

//Starts the timer with a new timer duration(time)
void napTimer::start(double time)
{
	active = true;
	timeLeft = duration = time;
}

//Stops the timer
void napTimer::stop()
{
	active = false;
}

void napTimer::resetTimeLeft() {
	timeLeft = duration;
}

//Returns true if an active timer has ended, false otherwise
bool napTimer::checkEnd()
{
	if (timeLeft <= 0 && active) {
		active = false;
		return true;
	}
	return false;
}

bool napTimer::isActive()
{
	return active;
}

//Updates the timer's remaining duration if it's active and checks end
bool napTimer::update(double timePassed)
{
	if (active)
		timeLeft -= timePassed;
	return checkEnd();
}


