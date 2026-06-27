#ifndef STEPSOUNDSAND_H
#define STEPSOUNDSAND_H

// Auto-generated from StepSoundSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "StepSound.h"

class StepSoundSand : public StepSound {
public:
	StepSoundSand(const JavaString& var1, float var2, float var3);
	virtual ~StepSoundSand();

	virtual JavaString getBreakSound();

	static void initStatics();
};

#endif // STEPSOUNDSAND_H