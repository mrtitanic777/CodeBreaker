#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from StepSoundStone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSoundStone.h"
#include "StepSound.h"

StepSoundStone::StepSoundStone(const JavaString& var1, float var2, float var3)
	: StepSound(var1, var2, var3)
{
}

StepSoundStone::~StepSoundStone()
{
}

JavaString StepSoundStone::getBreakSound()
{
	return "random.glass";
}

void StepSoundStone::initStatics()
{
}
