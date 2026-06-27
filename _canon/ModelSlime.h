#ifndef MODELSLIME_H
#define MODELSLIME_H

// Auto-generated from ModelSlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ModelBase.h"

class ModelRenderer;

class ModelSlime : public ModelBase {
public:
	ModelSlime(int var1);
	virtual ~ModelSlime();

	ModelRenderer* body;
	ModelRenderer* rightEye;
	ModelRenderer* leftEye;
	ModelRenderer* mouth;

	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ModelSlime(const ModelSlime&);
	ModelSlime& operator=(const ModelSlime&);
};

#endif // MODELSLIME_H