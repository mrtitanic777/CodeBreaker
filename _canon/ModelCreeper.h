#ifndef MODELCREEPER_H
#define MODELCREEPER_H

// Auto-generated from ModelCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ModelBase.h"

class ModelRenderer;

class ModelCreeper : public ModelBase {
public:
	ModelCreeper();
	virtual ~ModelCreeper();

	ModelRenderer* head;
	ModelRenderer* unusedModel;
	ModelRenderer* body;
	ModelRenderer* leg1;
	ModelRenderer* leg2;
	ModelRenderer* leg3;
	ModelRenderer* leg4;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ModelCreeper(const ModelCreeper&);
	ModelCreeper& operator=(const ModelCreeper&);
};

#endif // MODELCREEPER_H