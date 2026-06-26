#ifndef MODELBIPED_H
#define MODELBIPED_H

// Auto-generated from ModelBiped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelBiped : public ModelBase {
public:
	ModelBiped();
	ModelBiped(float var1);
	virtual ~ModelBiped();

	ModelRenderer* m_a;
	ModelRenderer* m_b;
	ModelRenderer* m_c;
	ModelRenderer* m_d;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

public: // was private
	ModelRenderer* m_e;
	ModelRenderer* m_f;
	ModelRenderer* m_g;

};

#endif // MODELBIPED_H