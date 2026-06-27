#ifndef MODELBIPED_H
#define MODELBIPED_H

// Auto-generated from ModelBiped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ModelBase.h"

class ModelRenderer;

class ModelBiped : public ModelBase {
public:
	ModelBiped();
	ModelBiped(float var1);
	ModelBiped(float var1, float var2);
	virtual ~ModelBiped();

	ModelRenderer* bipedHead;
	ModelRenderer* bipedHeadwear;
	ModelRenderer* bipedBody;
	ModelRenderer* bipedRightArm;
	ModelRenderer* bipedLeftArm;
	ModelRenderer* bipedRightLeg;
	ModelRenderer* bipedLeftLeg;
	bool heldItemLeft;
	bool heldItemRight;
	bool isSneak;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ModelBiped(const ModelBiped&);
	ModelBiped& operator=(const ModelBiped&);
};

#endif // MODELBIPED_H