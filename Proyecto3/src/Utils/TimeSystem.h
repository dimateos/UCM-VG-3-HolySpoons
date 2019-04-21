#ifndef TIME_SYSTEM_H_
#define TIME_SYSTEM_H_

// System that allows to check time passed
// partially based on fixed step by physx example blueprints
// lets try a static class
class TimeSystem
{
public:
	static void StartCounter();
	static double GetCounter();

private:
	static float stepTime;

	static double PCFreq;
	static __int64 CounterStart;
	static __int64 CounterLast;
};

#endif /* TIME_SYSTEM_H_ */

