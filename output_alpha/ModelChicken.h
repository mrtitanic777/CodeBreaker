#ifndef MODELCHICKEN_H
#define MODELCHICKEN_H

// Auto-generated from ModelChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelChicken : public ModelBase {
public:
	ModelChicken();
	virtual ~ModelChicken();

	ModelRenderer* m_field_1289_a;
	ModelRenderer* m_field_1288_b;
	ModelRenderer* m_field_1295_c;
	ModelRenderer* m_field_1294_d;
	ModelRenderer* m_field_1293_e;
	ModelRenderer* m_field_1292_f;
	ModelRenderer* m_field_1291_g;
	ModelRenderer* m_field_1290_h;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

};

#endif // MODELCHICKEN_H