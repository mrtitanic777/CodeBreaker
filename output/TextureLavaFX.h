#ifndef TEXTURELAVAFX_H
#define TEXTURELAVAFX_H

// Auto-generated from TextureLavaFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"

class TextureLavaFX : public TextureFX {
public:
	TextureLavaFX();
	virtual ~TextureLavaFX();

	virtual void onTick();

private:
	float* m_red;
	float* m_green;
	float* m_blue;
	float* m_alpha;

public:  // array-length companions
	int m_red_count;
	int m_green_count;
	int m_blue_count;
	int m_alpha_count;

};

#endif // TEXTURELAVAFX_H