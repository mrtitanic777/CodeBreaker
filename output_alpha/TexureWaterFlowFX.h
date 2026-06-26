#ifndef TEXUREWATERFLOWFX_H
#define TEXUREWATERFLOWFX_H

// Auto-generated from TexureWaterFlowFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"

class TexureWaterFlowFX : public TextureFX {
public:
	TexureWaterFlowFX();
	virtual ~TexureWaterFlowFX();

	virtual void func_783_a();

public: // was protected
	float* m_field_1138_g;
	float* m_field_1137_h;
	float* m_field_1136_i;
	float* m_field_1135_j;

public: // was private
	int m_field_1134_k;

public:  // array-length companions
	int m_field_1138_g_count;
	int m_field_1137_h_count;
	int m_field_1136_i_count;
	int m_field_1135_j_count;

};

#endif // TEXUREWATERFLOWFX_H