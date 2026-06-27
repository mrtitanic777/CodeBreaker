#ifndef STEPSOUND_H
#define STEPSOUND_H

// Auto-generated from StepSound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class StepSound {
public:
	StepSound(const JavaString& var1, float var2, float var3);
	virtual ~StepSound();

	JavaString stepSoundName;
	float volume;
	float pitch;

	virtual float getVolume();
	virtual float getPitch();
	virtual JavaString getBreakSound();
	virtual JavaString getStepSound();

	static void initStatics();
};

#endif // STEPSOUND_H