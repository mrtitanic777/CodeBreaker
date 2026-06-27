#ifndef TIMER_H
#define TIMER_H

// Auto-generated from Timer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Timer {
public:
	Timer(float var1);
	virtual ~Timer();

	float ticksPerSecond;
	double lastHRTime;
	int elapsedTicks;
	float renderPartialTicks;
	float timerSpeed;
	float elapsedPartialTicks;
	long long lastSyncSysClock;
	long long lastSyncHRClock;
	double timeSyncAdjustment;

	virtual void updateTimer();

	static void initStatics();
};

#endif // TIMER_H