#include "stdafx.h"
// Auto-generated from Timer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Timer.h"
#include "Common.h"

// Xbox 360 equivalent of Java System.nanoTime()
static long long GetNanoTime()
{
	LARGE_INTEGER counter, freq;
	QueryPerformanceCounter(&counter);
	QueryPerformanceFrequency(&freq);
	return (long long)((double)counter.QuadPart * 1000000000.0 / (double)freq.QuadPart);
}



Timer::Timer()
	: m_ticksPerSecond(20.0f)
	, m_lastHRTime(0)
	, m_elapsedTicks(0)
	, m_renderPartialTicks(0)
	, m_timerSpeed(1.0f)
	, m_elapsedPartialTicks(0.0f)
	, m_lastSyncSysClock(GetTickCount())
	, m_lastSyncHRClock(GetNanoTime() / 1000000LL)
	, m_timeSyncAdjustment(1.0f)
{
}

Timer::Timer(float var1)
	: m_ticksPerSecond(20.0f)
	, m_lastHRTime(0)
	, m_elapsedTicks(0)
	, m_renderPartialTicks(0)
	, m_timerSpeed(1.0f)
	, m_elapsedPartialTicks(0.0f)
	, m_lastSyncSysClock(GetTickCount())
	, m_lastSyncHRClock(GetNanoTime() / 1000000LL)
	, m_timeSyncAdjustment(1.0f)
{
}

Timer::~Timer() {
	// No dynamic resources
}

void Timer::updateTimer() {
		long var1 = GetTickCount();
		long var3 = var1 - m_lastSyncSysClock;
		long var5 = GetNanoTime() / 1000000L;
		double var9;
		if(var3 > 1000L) {
			long var7 = var5 - m_lastSyncHRClock;
			var9 = (double)var3 / (double)var7;
			m_timeSyncAdjustment += (var9 - m_timeSyncAdjustment) * (double)0.2F;
			m_lastSyncSysClock = var1;
			m_lastSyncHRClock = var5;
		}

		if(var3 < 0L) {
			m_lastSyncSysClock = var1;
			m_lastSyncHRClock = var5;
		}

		double var11 = (double)var5 / 1000.0f;
		var9 = (var11 - m_lastHRTime) * m_timeSyncAdjustment;
		m_lastHRTime = var11;
		if(var9 < 0.0f) {
			var9 = 0.0f;
		}

		if(var9 > 1.0f) {
			var9 = 1.0f;
		}

		m_elapsedPartialTicks = (float)((double)m_elapsedPartialTicks + var9 * (double)m_timerSpeed * (double)m_ticksPerSecond);
		m_elapsedTicks = (int)m_elapsedPartialTicks;
		m_elapsedPartialTicks -= (float)m_elapsedTicks;
		if(m_elapsedTicks > 10) {
			m_elapsedTicks = 10;
		}

		m_renderPartialTicks = m_elapsedPartialTicks;
}
