#ifndef TIMER_H
#define TIMER_H

// Auto-generated from Timer.java
// Target: Xbox 360 / C++03 / DirectX 9

class Timer {
public:
	Timer();
	Timer(float var1);
	virtual ~Timer();

	float m_ticksPerSecond;
	int m_elapsedTicks;
	float m_renderPartialTicks;
	float m_timerSpeed;
	float m_elapsedPartialTicks;

	virtual void updateTimer();

public: // was private
	double m_lastHRTime;
	long long m_lastSyncSysClock;
	long long m_lastSyncHRClock;
	double m_timeSyncAdjustment;

};

#endif // TIMER_H