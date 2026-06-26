#ifndef TEXTUREFX_H
#define TEXTUREFX_H

// Auto-generated from TextureFX.java
// Target: Xbox 360 / C++03 / DirectX 9

class RenderEngine;

class TextureFX {
public:
	TextureFX();
	TextureFX(int var1);
	virtual ~TextureFX();

	unsigned char* m_field_1127_a;
	int m_field_1126_b;
	bool m_field_1131_c;
	int m_field_1130_d;
	int m_field_1129_e;
	int m_field_1128_f;

	virtual void func_783_a();
	virtual void func_782_a(RenderEngine* var1);

public:  // array-length companions
	int m_field_1127_a_count;

};

#endif // TEXTUREFX_H