#include "stdafx.h"
// Auto-generated from StepSoundSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSoundSand.h"
#include "Common.h"
#include <string>
#include "StepSound.h"

StepSoundSand::StepSoundSand()
{
}

StepSoundSand::StepSoundSand(const std::string& var1, float var2, float var3)
	: StepSound(var1, var2, var3)
{
}

StepSoundSand::~StepSoundSand() {
	// No dynamic resources
}

std::string StepSoundSand::func_1146_a() {
		static const std::string _ret = "step.gravel"; return _ret;
}
