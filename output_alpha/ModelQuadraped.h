#ifndef MODELQUADRAPED_H
#define MODELQUADRAPED_H

// Auto-generated from ModelQuadraped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelQuadraped : public ModelBase {
public:
	ModelQuadraped();
	ModelQuadraped(int var1, float var2);
	virtual ~ModelQuadraped();

	ModelRenderer* head;
	ModelRenderer* body;
	ModelRenderer* leg1;
	ModelRenderer* leg2;
	ModelRenderer* leg3;
	ModelRenderer* leg4;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

};

#endif // MODELQUADRAPED_H