#ifndef TEXTUREWATERFX_H
#define TEXTUREWATERFX_H

// Auto-generated from TextureWaterFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"

class TextureWaterFX : public TextureFX {
public:
	TextureWaterFX();
	virtual ~TextureWaterFX();

	virtual void onTick();

private:
	float* m_red;
	float* m_green;
	float* m_blue;
	float* m_alpha;
	int m_tickCounter;

};

#endif // TEXTUREWATERFX_H