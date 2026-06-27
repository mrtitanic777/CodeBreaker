#ifndef STEPSOUNDSTONE_H
#define STEPSOUNDSTONE_H

// Auto-generated from StepSoundStone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "StepSound.h"

class StepSoundStone : public StepSound {
public:
	StepSoundStone(const JavaString& var1, float var2, float var3);
	virtual ~StepSoundStone();

	virtual JavaString getBreakSound();

	static void initStatics();
};

#endif // STEPSOUNDSTONE_H