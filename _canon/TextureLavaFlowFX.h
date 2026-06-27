#ifndef TEXTURELAVAFLOWFX_H
#define TEXTURELAVAFLOWFX_H

// Auto-generated from TextureLavaFlowFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TextureFX.h"

class TextureLavaFlowFX : public TextureFX {
public:
	TextureLavaFlowFX();
	virtual ~TextureLavaFlowFX();

	JArray<float > intensity;
	JArray<float > intensityTemp;
	JArray<float > heat;
	JArray<float > heatVelocity;
	int tickCounter;

	virtual void onTick();

	static void initStatics();
};

#endif // TEXTURELAVAFLOWFX_H