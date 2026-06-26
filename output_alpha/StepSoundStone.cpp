#include "stdafx.h"
// Auto-generated from StepSoundStone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSoundStone.h"
#include "Common.h"
#include <string>
#include "StepSound.h"

StepSoundStone::StepSoundStone()
{
}

StepSoundStone::StepSoundStone(const std::string& var1, float var2, float var3)
	: StepSound(var1, var2, var3)
{
}

StepSoundStone::~StepSoundStone() {
	// No dynamic resources
}

std::string StepSoundStone::func_1146_a() {
		static const std::string _ret = "random.glass"; return _ret;
}
