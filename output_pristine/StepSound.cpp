#include "stdafx.h"
// Auto-generated from StepSound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSound.h"
#include "Common.h"
#include <string>

StepSound::StepSound()
	: m_soundVolume(0)
	, m_soundPitch(0)
{
}

StepSound::StepSound(const std::string& var1, float var2, float var3)
	: m_soundVolume(0)
	, m_soundPitch(0)
{
		m_sound = var1;
		m_soundVolume = var2;
		m_soundPitch = var3;
}

StepSound::~StepSound() {
	// No dynamic resources
}

const std::string& StepSound::stepSoundDir() {
		return "step." + m_sound;
}

const std::string& StepSound::stepSoundDir2() {
		return "step." + m_sound;
}
