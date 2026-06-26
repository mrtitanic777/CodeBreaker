#ifndef ENTITYRENDERER_H
#define ENTITYRENDERER_H

// Auto-generated from EntityRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

class Entity;
class ItemRenderer;
class Minecraft;
class Random;

class EntityRenderer {
public:
	EntityRenderer();
	EntityRenderer(Minecraft* var1);
	virtual ~EntityRenderer();

	ItemRenderer* m_field_1395_a;

	virtual void func_911_a();
	virtual void func_910_a(float var1);
	virtual void func_4136_b(float var1);
	virtual void func_4134_c(float var1);
	virtual void func_905_b();

public: // was private
	Minecraft* mc;
	float m_field_1387_i;
	int m_field_1386_j;
	Entity* m_field_1385_k;
	long long m_field_1384_l;
	Random* m_field_1383_m;
	float* /* PORT: was FloatBuffer */ m_field_1392_d;
	float m_field_4270_e;
	float m_field_4269_f;
	float m_field_4268_g;
	float m_field_1382_n;
	float m_field_1381_o;

	float func_914_d(float var1);
	void hurtCameraEffect(float var1);
	void func_917_f(float var1);
	void orientCamera(float var1);
	void setupCameraTransform(float var1, int var2);
	void func_4135_b(float var1, int var2);
	void func_916_c();
	void updateFogColor(float var1);
	void func_4140_a(int var1);
	float* /* PORT: was FloatBuffer */ func_908_a(float var1, float var2, float var3, float var4);

};

#endif // ENTITYRENDERER_H