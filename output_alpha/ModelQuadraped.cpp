#include "stdafx.h"
// Auto-generated from ModelQuadraped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelQuadraped.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelQuadraped::ModelQuadraped()
	: head(new ModelRenderer(0, 0))
	, body(NULL)
	, leg1(NULL)
	, leg2(NULL)
	, leg3(NULL)
	, leg4(NULL)
{
}

ModelQuadraped::ModelQuadraped(int var1, float var2)
	: head(new ModelRenderer(0, 0))
	, body(NULL)
	, leg1(NULL)
	, leg2(NULL)
	, leg3(NULL)
	, leg4(NULL)
{
		head->addBox(-4.0F, -4.0F, -8.0F, 8, 8, 8, var2);
		head->setPosition(0.0F, (float)(18 - var1), -6.0F);
		body = new ModelRenderer(28, 8);
		body->addBox(-5.0F, -10.0F, -7.0F, 10, 16, 8, var2);
		body->setPosition(0.0F, (float)(17 - var1), 2.0F);
		leg1 = new ModelRenderer(0, 16);
		leg1->addBox(-2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
		leg1->setPosition(-3.0F, (float)(24 - var1), 7.0F);
		leg2 = new ModelRenderer(0, 16);
		leg2->addBox(-2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
		leg2->setPosition(3.0F, (float)(24 - var1), 7.0F);
		leg3 = new ModelRenderer(0, 16);
		leg3->addBox(-2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
		leg3->setPosition(-3.0F, (float)(24 - var1), -5.0F);
		leg4 = new ModelRenderer(0, 16);
		leg4->addBox(-2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
		leg4->setPosition(3.0F, (float)(24 - var1), -5.0F);
}

ModelQuadraped::~ModelQuadraped() {
	head = NULL;
	body = NULL;
	leg1 = NULL;
	leg2 = NULL;
	leg3 = NULL;
	leg4 = NULL;
}

void ModelQuadraped::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		setRotationAngles(var1, var2, var3, var4, var5, var6);
		head->render(var6);
		body->render(var6);
		leg1->render(var6);
		leg2->render(var6);
		leg3->render(var6);
		leg4->render(var6);
}

void ModelQuadraped::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		head->m_rotateAngleX = -(var5 / (180.0F / 3.14159265358979323846f));
		head->m_rotateAngleY = var4 / (180.0F / 3.14159265358979323846f);
		body->m_rotateAngleX = 3.14159265358979323846f * 0.5F;
		leg1->m_rotateAngleX = cosf(var1 * 0.6662F) * 1.4F * var2;
		leg2->m_rotateAngleX = cosf(var1 * 0.6662F + 3.14159265358979323846f) * 1.4F * var2;
		leg3->m_rotateAngleX = cosf(var1 * 0.6662F + 3.14159265358979323846f) * 1.4F * var2;
		leg4->m_rotateAngleX = cosf(var1 * 0.6662F) * 1.4F * var2;
}
