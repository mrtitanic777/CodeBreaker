#ifndef MODELSPIDER_H
#define MODELSPIDER_H

// Auto-generated from ModelSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelSpider : public ModelBase {
public:
	ModelSpider();
	virtual ~ModelSpider();

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

public: // was private
	ModelRenderer* m_spiderHead;
	ModelRenderer* m_spiderNeck;
	ModelRenderer* m_spiderBody;
	ModelRenderer* m_spiderLeg1;
	ModelRenderer* m_spiderLeg2;
	ModelRenderer* m_spiderLeg3;
	ModelRenderer* m_spiderLeg4;
	ModelRenderer* m_spiderLeg5;
	ModelRenderer* m_spiderLeg6;
	ModelRenderer* m_spiderLeg7;
	ModelRenderer* m_spiderLeg8;

};

#endif // MODELSPIDER_H