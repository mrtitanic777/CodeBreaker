#ifndef STEPSOUNDGRAVEL_H
#define STEPSOUNDGRAVEL_H

// Auto-generated from StepSoundGravel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSound.h"
#include <string>

class StepSoundGravel : public StepSound {
public:
	StepSoundGravel();
	StepSoundGravel(const std::string& var1, float var2, float var3);
	virtual ~StepSoundGravel();

	virtual const std::string& stepSoundDir();

private:

};

#endif // STEPSOUNDGRAVEL_H