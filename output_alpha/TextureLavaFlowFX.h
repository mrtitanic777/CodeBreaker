#ifndef TEXTURELAVAFLOWFX_H
#define TEXTURELAVAFLOWFX_H

// Auto-generated from TextureLavaFlowFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"

class TextureLavaFlowFX : public TextureFX {
public:
	TextureLavaFlowFX();
	virtual ~TextureLavaFlowFX();

	virtual void func_783_a();

public: // was protected
	float* m_field_1143_g;
	float* m_field_1142_h;
	float* m_field_1141_i;
	float* m_field_1140_j;

public: // was private
	int m_field_1139_k;

public:  // array-length companions
	int m_field_1143_g_count;
	int m_field_1142_h_count;
	int m_field_1141_i_count;
	int m_field_1140_j_count;

};

#endif // TEXTURELAVAFLOWFX_H