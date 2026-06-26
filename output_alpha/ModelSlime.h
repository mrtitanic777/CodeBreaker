#ifndef MODELSLIME_H
#define MODELSLIME_H

// Auto-generated from ModelSlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelSlime : public ModelBase {
public:
	ModelSlime();
	ModelSlime(int var1);
	virtual ~ModelSlime();

	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);

public: // was private
	ModelRenderer* m_field_1258_a;
	ModelRenderer* m_field_1257_b;
	ModelRenderer* m_field_1260_c;
	ModelRenderer* m_field_1259_d;

};

#endif // MODELSLIME_H