#ifndef TEXTUREFLAMESFX_H
#define TEXTUREFLAMESFX_H

// Auto-generated from TextureFlamesFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"

class TextureFlamesFX : public TextureFX {
public:
	TextureFlamesFX();
	TextureFlamesFX(int var1);
	virtual ~TextureFlamesFX();

	virtual void onTick();

private:
	float* m_currentFireFrame;
	float* m_lastFireFrame;

public:  // array-length companions
	int m_currentFireFrame_count;
	int m_lastFireFrame_count;

};

#endif // TEXTUREFLAMESFX_H