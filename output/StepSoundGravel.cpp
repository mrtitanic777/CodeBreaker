#include "stdafx.h"
// Auto-generated from StepSoundGravel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSoundGravel.h"
#include "Common.h"
#include <string>
#include "StepSound.h"

StepSoundGravel::StepSoundGravel()
{
}

StepSoundGravel::StepSoundGravel(const std::string& var1, float var2, float var3)
	: StepSound(var1, 1.0f, 1.0f)
{
}

StepSoundGravel::~StepSoundGravel() {
	// No dynamic resources
}

const std::string& StepSoundGravel::stepSoundDir() {
		return "step.gravel";
}
