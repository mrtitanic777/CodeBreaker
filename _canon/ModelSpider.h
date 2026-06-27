#ifndef MODELSPIDER_H
#define MODELSPIDER_H

// Auto-generated from ModelSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ModelBase.h"

class ModelRenderer;

class ModelSpider : public ModelBase {
public:
	ModelSpider();
	virtual ~ModelSpider();

	ModelRenderer* spiderHead;
	ModelRenderer* spiderNeck;
	ModelRenderer* spiderBody;
	ModelRenderer* spiderLeg1;
	ModelRenderer* spiderLeg2;
	ModelRenderer* spiderLeg3;
	ModelRenderer* spiderLeg4;
	ModelRenderer* spiderLeg5;
	ModelRenderer* spiderLeg6;
	ModelRenderer* spiderLeg7;
	ModelRenderer* spiderLeg8;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ModelSpider(const ModelSpider&);
	ModelSpider& operator=(const ModelSpider&);
};

#endif // MODELSPIDER_H