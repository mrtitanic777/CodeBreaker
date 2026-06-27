#ifndef MODELCHICKEN_H
#define MODELCHICKEN_H

// Auto-generated from ModelChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ModelBase.h"

class ModelRenderer;

class ModelChicken : public ModelBase {
public:
	ModelChicken();
	virtual ~ModelChicken();

	ModelRenderer* head;
	ModelRenderer* body;
	ModelRenderer* rightLeg;
	ModelRenderer* leftLeg;
	ModelRenderer* rightWing;
	ModelRenderer* leftWing;
	ModelRenderer* bill;
	ModelRenderer* chin;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ModelChicken(const ModelChicken&);
	ModelChicken& operator=(const ModelChicken&);
};

#endif // MODELCHICKEN_H