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

	virtual void func_783_a();

public: // was protected
	float* m_field_1133_g;
	float* m_field_1132_h;

public:  // array-length companions
	int m_field_1133_g_count;
	int m_field_1132_h_count;

};

#endif // TEXTUREFLAMESFX_H