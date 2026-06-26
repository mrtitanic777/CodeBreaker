#ifndef MODELSPIDER_H
#define MODELSPIDER_H

// Auto-generated from ModelSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelSpider : public ModelBase {
public:
	ModelSpider();
	virtual ~ModelSpider();

	ModelRenderer* m_field_1255_a;
	ModelRenderer* m_field_1254_b;
	ModelRenderer* m_field_1253_c;
	ModelRenderer* m_field_1252_d;
	ModelRenderer* m_field_1251_e;
	ModelRenderer* m_field_1250_f;
	ModelRenderer* m_field_1249_g;
	ModelRenderer* m_field_1248_h;
	ModelRenderer* m_field_1247_i;
	ModelRenderer* m_field_1246_j;
	ModelRenderer* m_field_1245_m;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

};

#endif // MODELSPIDER_H