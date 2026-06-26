#ifndef MODELQUADRUPED_H
#define MODELQUADRUPED_H

// Auto-generated from ModelQuadruped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelQuadruped : public ModelBase {
public:
	ModelQuadruped();
	ModelQuadruped(int var1, float var2);
	virtual ~ModelQuadruped();

	ModelRenderer* head;
	ModelRenderer* body;
	ModelRenderer* leg1;
	ModelRenderer* leg2;
	ModelRenderer* leg3;
	ModelRenderer* leg4;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

};

#endif // MODELQUADRUPED_H