#ifndef TEXTUREGEARSFX_H
#define TEXTUREGEARSFX_H

// Auto-generated from TextureGearsFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"

class TextureGearsFX : public TextureFX {
public:
	TextureGearsFX();
	TextureGearsFX(int var1);
	virtual ~TextureGearsFX();

	virtual void onTick();

private:
	int m_gearRotation;
	int* m_gearColor;
	int* m_gearMiddleColor;
	int m_gearRotationDir;

};

#endif // TEXTUREGEARSFX_H