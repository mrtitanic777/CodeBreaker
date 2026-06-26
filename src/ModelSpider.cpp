#include "stdafx.h"
// Auto-generated from ModelSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSpider.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelSpider::ModelSpider()
	: m_spiderHead(new ModelRenderer(32, 4))
	, m_spiderNeck(NULL)
	, m_spiderBody(NULL)
	, m_spiderLeg1(NULL)
	, m_spiderLeg2(NULL)
	, m_spiderLeg3(NULL)
	, m_spiderLeg4(NULL)
	, m_spiderLeg5(NULL)
	, m_spiderLeg6(NULL)
	, m_spiderLeg7(NULL)
	, m_spiderLeg8(NULL)
{
		m_spiderHead->addBox(-4.0F, -4.0F, -8.0F, 8, 8, 8, 0.0F);
		m_spiderHead->setRotationPoint(0.0F, 15.0F, -3.0F);
		m_spiderNeck = new ModelRenderer(0, 0);
		m_spiderNeck->addBox(-3.0F, -3.0F, -3.0F, 6, 6, 6, 0.0F);
		m_spiderNeck->setRotationPoint(0.0F, 15.0F, 0.0F);
		m_spiderBody = new ModelRenderer(0, 12);
		m_spiderBody->addBox(-5.0F, -4.0F, -6.0F, 10, 8, 12, 0.0F);
		m_spiderBody->setRotationPoint(0.0F, 15.0F, 9.0F);
		m_spiderLeg1 = new ModelRenderer(18, 0);
		m_spiderLeg1->addBox(-15.0F, -1.0F, -1.0F, 16, 2, 2, 0.0F);
		m_spiderLeg1->setRotationPoint(-4.0F, 15.0F, 2.0F);
		m_spiderLeg2 = new ModelRenderer(18, 0);
		m_spiderLeg2->addBox(-1.0F, -1.0F, -1.0F, 16, 2, 2, 0.0F);
		m_spiderLeg2->setRotationPoint(4.0F, 15.0F, 2.0F);
		m_spiderLeg3 = new ModelRenderer(18, 0);
		m_spiderLeg3->addBox(-15.0F, -1.0F, -1.0F, 16, 2, 2, 0.0F);
		m_spiderLeg3->setRotationPoint(-4.0F, 15.0F, 1.0F);
		m_spiderLeg4 = new ModelRenderer(18, 0);
		m_spiderLeg4->addBox(-1.0F, -1.0F, -1.0F, 16, 2, 2, 0.0F);
		m_spiderLeg4->setRotationPoint(4.0F, 15.0F, 1.0F);
		m_spiderLeg5 = new ModelRenderer(18, 0);
		m_spiderLeg5->addBox(-15.0F, -1.0F, -1.0F, 16, 2, 2, 0.0F);
		m_spiderLeg5->setRotationPoint(-4.0F, 15.0F, 0.0F);
		m_spiderLeg6 = new ModelRenderer(18, 0);
		m_spiderLeg6->addBox(-1.0F, -1.0F, -1.0F, 16, 2, 2, 0.0F);
		m_spiderLeg6->setRotationPoint(4.0F, 15.0F, 0.0F);
		m_spiderLeg7 = new ModelRenderer(18, 0);
		m_spiderLeg7->addBox(-15.0F, -1.0F, -1.0F, 16, 2, 2, 0.0F);
		m_spiderLeg7->setRotationPoint(-4.0F, 15.0F, -1.0F);
		m_spiderLeg8 = new ModelRenderer(18, 0);
		m_spiderLeg8->addBox(-1.0F, -1.0F, -1.0F, 16, 2, 2, 0.0F);
		m_spiderLeg8->setRotationPoint(4.0F, 15.0F, -1.0F);
}

ModelSpider::~ModelSpider() {
	m_spiderHead = NULL;
	m_spiderNeck = NULL;
	m_spiderBody = NULL;
	m_spiderLeg1 = NULL;
	m_spiderLeg2 = NULL;
	m_spiderLeg3 = NULL;
	m_spiderLeg4 = NULL;
	m_spiderLeg5 = NULL;
	m_spiderLeg6 = NULL;
	m_spiderLeg7 = NULL;
	m_spiderLeg8 = NULL;
}

void ModelSpider::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		setRotationAngles(var1, var2, var3, var4, var5, 1.0F);
		m_spiderHead->render(1.0F);
		m_spiderNeck->render(1.0F);
		m_spiderBody->render(1.0F);
		m_spiderLeg1->render(1.0F);
		m_spiderLeg2->render(1.0F);
		m_spiderLeg3->render(1.0F);
		m_spiderLeg4->render(1.0F);
		m_spiderLeg5->render(1.0F);
		m_spiderLeg6->render(1.0F);
		m_spiderLeg7->render(1.0F);
		m_spiderLeg8->render(1.0F);
}

void ModelSpider::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		m_spiderHead->m_rotateAngleY = var4 / (180.0F / 3.14159265358979323846f);
		m_spiderHead->m_rotateAngleX = var5 / (180.0F / 3.14159265358979323846f);
		m_spiderLeg1->m_rotateAngleZ = 3.14159265358979323846f * -0.25F;
		m_spiderLeg2->m_rotateAngleZ = 3.14159265358979323846f * 0.25F;
		m_spiderLeg3->m_rotateAngleZ = -(3.14159265358979323846f * 0.185F);
		m_spiderLeg4->m_rotateAngleZ = 3.14159265358979323846f * 0.185F;
		m_spiderLeg5->m_rotateAngleZ = -(3.14159265358979323846f * 0.185F);
		m_spiderLeg6->m_rotateAngleZ = 3.14159265358979323846f * 0.185F;
		m_spiderLeg7->m_rotateAngleZ = 3.14159265358979323846f * -0.25F;
		m_spiderLeg8->m_rotateAngleZ = 3.14159265358979323846f * 0.25F;
		m_spiderLeg1->m_rotateAngleY = 3.14159265358979323846f * 0.25F;
		m_spiderLeg2->m_rotateAngleY = 3.14159265358979323846f * -0.25F;
		m_spiderLeg3->m_rotateAngleY = 3.14159265358979323846f * 0.125F;
		m_spiderLeg4->m_rotateAngleY = 3.14159265358979323846f * -0.125F;
		m_spiderLeg5->m_rotateAngleY = 3.14159265358979323846f * -0.125F;
		m_spiderLeg6->m_rotateAngleY = 3.14159265358979323846f * 0.125F;
		m_spiderLeg7->m_rotateAngleY = 3.14159265358979323846f * -0.25F;
		m_spiderLeg8->m_rotateAngleY = 3.14159265358979323846f * 0.25F;
		var3 = -(cosf(var1 * 0.6662F * 2.0F) * 0.4F) * var2;
		var4 = -(cosf(var1 * 0.6662F * 2.0F + 3.14159265358979323846f) * 0.4F) * var2;
		var5 = -(cosf(var1 * 0.6662F * 2.0F + 3.14159265358979323846f * 0.5F) * 0.4F) * var2;
		var6 = -(cosf(var1 * 0.6662F * 2.0F + 3.14159265358979323846f * 3.0F / 2.0F) * 0.4F) * var2;
		float var7 = abs(sinf(var1 * 0.6662F) * 0.4F) * var2;
		float var8 = abs(sinf(var1 * 0.6662F + 3.14159265358979323846f) * 0.4F) * var2;
		float var9 = abs(sinf(var1 * 0.6662F + 3.14159265358979323846f * 0.5F) * 0.4F) * var2;
		var1 = abs(sinf(var1 * 0.6662F + 3.14159265358979323846f * 3.0F / 2.0F) * 0.4F) * var2;
		m_spiderLeg1->m_rotateAngleY += var3;
		m_spiderLeg2->m_rotateAngleY -= var3;
		m_spiderLeg3->m_rotateAngleY += var4;
		m_spiderLeg4->m_rotateAngleY -= var4;
		m_spiderLeg5->m_rotateAngleY += var5;
		m_spiderLeg6->m_rotateAngleY -= var5;
		m_spiderLeg7->m_rotateAngleY += var6;
		m_spiderLeg8->m_rotateAngleY -= var6;
		m_spiderLeg1->m_rotateAngleZ += var7;
		m_spiderLeg2->m_rotateAngleZ -= var7;
		m_spiderLeg3->m_rotateAngleZ += var8;
		m_spiderLeg4->m_rotateAngleZ -= var8;
		m_spiderLeg5->m_rotateAngleZ += var9;
		m_spiderLeg6->m_rotateAngleZ -= var9;
		m_spiderLeg7->m_rotateAngleZ += var1;
		m_spiderLeg8->m_rotateAngleZ -= var1;
}
