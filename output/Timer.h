#ifndef TIMER_H
#define TIMER_H

// Auto-generated from Timer.java
// Target: Xbox 360 / C++03 / DirectX 9

class Timer {
public:
	Timer();
	Timer(float var1);
	virtual ~Timer();

	int m_elapsedTicks;
	float m_renderPartialTicks;

	virtual void updateTimer();

private:
	float m_ticksPerSecond;
	double m_lastHRTime;
	float m_timerSpeed;
	float m_elapsedPartialTicks;
	long long m_lastSyncSysClock;
	long long m_lastSyncHRClock;
	double m_timeSyncAdjustment;

};

#endif // TIMER_H