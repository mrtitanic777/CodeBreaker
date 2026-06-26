#ifndef RENDERLIST_H
#define RENDERLIST_H

// Auto-generated from RenderList.java
// Target: Xbox 360 / C++03 / DirectX 9

class RenderList {
public:
	RenderList();
	virtual ~RenderList();

	virtual void func_861_a(int var1, int var2, int var3, double var4, double var6, double var8);
	virtual bool func_862_a(int var1, int var2, int var3);
	virtual void func_858_a(int var1);
	virtual void func_860_a();
	virtual void func_859_b();

public: // was private
	int m_field_1242_a;
	int m_field_1241_b;
	int m_field_1240_c;
	float m_field_1239_d;
	float m_field_1238_e;
	float m_field_1237_f;
	int* /* PORT: was IntBuffer */ m_field_1236_g;
	bool m_field_1235_h;
	bool m_field_1234_i;

};

#endif // RENDERLIST_H