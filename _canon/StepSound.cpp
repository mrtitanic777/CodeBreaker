#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from StepSound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSound.h"

StepSound::StepSound(const JavaString& var1, float var2, float var3)
	: volume(0)
	, pitch(0)
{
	this->stepSoundName = var1;
	this->volume = var2;
	this->pitch = var3;
}

StepSound::~StepSound()
{
}

float StepSound::getVolume()
{
	return this->volume;
}

float StepSound::getPitch()
{
	return this->pitch;
}

JavaString StepSound::getBreakSound()
{
	return (JCat() + "step." + this->stepSoundName);
}

JavaString StepSound::getStepSound()
{
	return (JCat() + "step." + this->stepSoundName);
}

void StepSound::initStatics()
{
}
