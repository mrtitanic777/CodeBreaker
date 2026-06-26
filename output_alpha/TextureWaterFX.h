#ifndef TEXTUREWATERFX_H
#define TEXTUREWATERFX_H

// Auto-generated from TextureWaterFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"

class TextureWaterFX : public TextureFX {
public:
	TextureWaterFX();
	virtual ~TextureWaterFX();

	virtual void func_783_a();

public: // was protected
	float* m_field_1158_g;
	float* m_field_1157_h;
	float* m_field_1156_i;
	float* m_field_1155_j;

public: // was private
	int m_tickCounter;

public:  // array-length companions
	int m_field_1158_g_count;
	int m_field_1157_h_count;
	int m_field_1156_i_count;
	int m_field_1155_j_count;

};

#endif // TEXTUREWATERFX_H