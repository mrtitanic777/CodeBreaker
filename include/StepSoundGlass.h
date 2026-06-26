#ifndef STEPSOUNDGLASS_H
#define STEPSOUNDGLASS_H

// Auto-generated from StepSoundGlass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSound.h"
#include <string>

class StepSoundGlass : public StepSound {
public:
	StepSoundGlass();
	StepSoundGlass(const std::string& var1, float var2, float var3);
	virtual ~StepSoundGlass();

	virtual const std::string& stepSoundDir();

private:

};

#endif // STEPSOUNDGLASS_H