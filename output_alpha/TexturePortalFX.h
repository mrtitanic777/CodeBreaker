#ifndef TEXTUREPORTALFX_H
#define TEXTUREPORTALFX_H

// Auto-generated from TexturePortalFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"

class TexturePortalFX : public TextureFX {
public:
	TexturePortalFX();
	virtual ~TexturePortalFX();

	virtual void func_783_a();

public: // was private
	int m_field_4227_g;
	unsigned char** m_field_4226_h;

public:  // array-length companions
	int m_field_4226_h_count;

};

#endif // TEXTUREPORTALFX_H