#include "stdafx.h"
// Auto-generated from StepSoundGlass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSoundGlass.h"
#include "Common.h"
#include <string>
#include "StepSound.h"

StepSoundGlass::StepSoundGlass()
{
}

StepSoundGlass::StepSoundGlass(const std::string& var1, float var2, float var3)
	: StepSound(var1, 1.0f, 1.0f)
{
}

StepSoundGlass::~StepSoundGlass() {
	// No dynamic resources
}

const std::string& StepSoundGlass::stepSoundDir() {
		return "random->glass";
}
