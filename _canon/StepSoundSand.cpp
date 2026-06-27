#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from StepSoundSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSoundSand.h"
#include "StepSound.h"

StepSoundSand::StepSoundSand(const JavaString& var1, float var2, float var3)
	: StepSound(var1, var2, var3)
{
}

StepSoundSand::~StepSoundSand()
{
}

JavaString StepSoundSand::getBreakSound()
{
	return "step.gravel";
}

void StepSoundSand::initStatics()
{
}
