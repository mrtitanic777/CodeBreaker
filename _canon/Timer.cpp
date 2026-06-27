#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Timer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Timer.h"

Timer::Timer(float var1)
	: ticksPerSecond(0)
	, lastHRTime(0)
	, elapsedTicks(0)
	, renderPartialTicks(0)
	, timerSpeed(0)
	, elapsedPartialTicks(0)
	, lastSyncSysClock(0)
	, lastSyncHRClock(0)
	, timeSyncAdjustment(0)
{
	this->timerSpeed = 1.0F;
	this->elapsedPartialTicks = 0.0F;
	this->timeSyncAdjustment = 1.0;
	this->ticksPerSecond = var1;
	this->lastSyncSysClock = System::currentTimeMillis();
	this->lastSyncHRClock = System::nanoTime() / 1000000LL;
}

Timer::~Timer()
{
}

void Timer::updateTimer()
{
	long long var1 = System::currentTimeMillis();
	long long var3 = var1 - this->lastSyncSysClock;
	long long var5 = System::nanoTime() / 1000000LL;
	double var9;
	if(var3 > 1000LL) {
	long long var7 = var5 - this->lastSyncHRClock;
	var9 = (double)var3 / (double)var7;
	this->timeSyncAdjustment += (var9 - this->timeSyncAdjustment) * (double)0.2F;
	this->lastSyncSysClock = var1;
	this->lastSyncHRClock = var5;
	}
	if(var3 < 0LL) {
	this->lastSyncSysClock = var1;
	this->lastSyncHRClock = var5;
	}
	double var11 = (double)var5 / 1000.0;
	var9 = (var11 - this->lastHRTime) * this->timeSyncAdjustment;
	this->lastHRTime = var11;
	if(var9 < 0.0) {
	var9 = 0.0;
	}
	if(var9 > 1.0) {
	var9 = 1.0;
	}
	this->elapsedPartialTicks = (float)((double)this->elapsedPartialTicks + var9 * (double)this->timerSpeed * (double)this->ticksPerSecond);
	this->elapsedTicks = (int)this->elapsedPartialTicks;
	this->elapsedPartialTicks -= (float)this->elapsedTicks;
	if(this->elapsedTicks > 10) {
	this->elapsedTicks = 10;
	}
	this->renderPartialTicks = this->elapsedPartialTicks;
}

void Timer::initStatics()
{
}
