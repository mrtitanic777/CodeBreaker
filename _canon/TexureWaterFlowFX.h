#ifndef TEXUREWATERFLOWFX_H
#define TEXUREWATERFLOWFX_H

// Auto-generated from TexureWaterFlowFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TextureFX.h"

class TexureWaterFlowFX : public TextureFX {
public:
	TexureWaterFlowFX();
	virtual ~TexureWaterFlowFX();

	JArray<float > field;
	JArray<float > tempField;
	JArray<float > heightField;
	JArray<float > soundField;
	int tickCounter;

	virtual void onTick();

	static void initStatics();
};

#endif // TEXUREWATERFLOWFX_H