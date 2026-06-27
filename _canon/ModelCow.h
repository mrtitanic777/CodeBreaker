#ifndef MODELCOW_H
#define MODELCOW_H

// Auto-generated from ModelCow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ModelQuadraped.h"

class ModelRenderer;

class ModelCow : public ModelQuadraped {
public:
	ModelCow();
	virtual ~ModelCow();

	ModelRenderer* udders;
	ModelRenderer* horn1;
	ModelRenderer* horn2;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ModelCow(const ModelCow&);
	ModelCow& operator=(const ModelCow&);
};

#endif // MODELCOW_H