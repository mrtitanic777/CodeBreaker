#ifndef STEPSOUND_H
#define STEPSOUND_H

// Auto-generated from StepSound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class StepSound {
public:
	StepSound();
	StepSound(const std::string& var1, float var2, float var3);
	virtual ~StepSound();

	float m_soundVolume;
	float m_soundPitch;

	virtual const std::string& stepSoundDir();
	virtual const std::string& stepSoundDir2();

private:
	std::string m_sound;

};

#endif // STEPSOUND_H