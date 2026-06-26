#ifndef TEXTUREWATERFLOWFX_H
#define TEXTUREWATERFLOWFX_H

// Auto-generated from TextureWaterFlowFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"

class TextureWaterFlowFX : public TextureFX {
public:
	TextureWaterFlowFX();
	virtual ~TextureWaterFlowFX();

	virtual void onTick();

private:
	float* m_red;
	float* m_green;
	float* m_blue;
	float* m_alpha;
	int m_tickCounter;

public:  // array-length companions
	int m_red_count;
	int m_green_count;
	int m_blue_count;
	int m_alpha_count;

};

#endif // TEXTUREWATERFLOWFX_H