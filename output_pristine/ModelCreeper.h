#ifndef MODELCREEPER_H
#define MODELCREEPER_H

// Auto-generated from ModelCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelCreeper : public ModelBase {
public:
	ModelCreeper();
	virtual ~ModelCreeper();

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

private:
	ModelRenderer* head;
	ModelRenderer* m_unusedCreeperHeadwear;
	ModelRenderer* body;
	ModelRenderer* leg1;
	ModelRenderer* leg2;
	ModelRenderer* leg3;
	ModelRenderer* leg4;

};

#endif // MODELCREEPER_H