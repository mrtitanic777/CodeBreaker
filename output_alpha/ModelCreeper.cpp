#include "stdafx.h"
// Auto-generated from ModelCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelCreeper.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelCreeper::ModelCreeper()
	: head(NULL)
	, m_unusedModel(NULL)
	, body(NULL)
	, leg1(NULL)
	, leg2(NULL)
	, leg3(NULL)
	, leg4(NULL)
{
		float var1 = 0.0F;
		unsigned char var2 = 4;
		head = new ModelRenderer(0, 0);
		head->addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1);
		head->setPosition(0.0F, (float)var2, 0.0F);
		m_unusedModel = new ModelRenderer(32, 0);
		m_unusedModel->addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1 + 0.5F);
		m_unusedModel->setPosition(0.0F, (float)var2, 0.0F);
		body = new ModelRenderer(16, 16);
		body->addBox(-4.0F, 0.0F, -2.0F, 8, 12, 4, var1);
		body->setPosition(0.0F, (float)var2, 0.0F);
		leg1 = new ModelRenderer(0, 16);
		leg1->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
		leg1->setPosition(-2.0F, (float)(12 + var2), 4.0F);
		leg2 = new ModelRenderer(0, 16);
		leg2->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
		leg2->setPosition(2.0F, (float)(12 + var2), 4.0F);
		leg3 = new ModelRenderer(0, 16);
		leg3->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
		leg3->setPosition(-2.0F, (float)(12 + var2), -4.0F);
		leg4 = new ModelRenderer(0, 16);
		leg4->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
		leg4->setPosition(2.0F, (float)(12 + var2), -4.0F);
}

ModelCreeper::~ModelCreeper() {
	head = NULL;
	m_unusedModel = NULL;
	body = NULL;
	leg1 = NULL;
	leg2 = NULL;
	leg3 = NULL;
	leg4 = NULL;
}

void ModelCreeper::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		setRotationAngles(var1, var2, var3, var4, var5, var6);
		head->render(var6);
		body->render(var6);
		leg1->render(var6);
		leg2->render(var6);
		leg3->render(var6);
		leg4->render(var6);
}

void ModelCreeper::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		head->m_rotateAngleY = var4 / (180.0F / 3.14159265358979323846f);
		head->m_rotateAngleX = var5 / (180.0F / 3.14159265358979323846f);
		leg1->m_rotateAngleX = cosf(var1 * 0.6662F) * 1.4F * var2;
		leg2->m_rotateAngleX = cosf(var1 * 0.6662F + 3.14159265358979323846f) * 1.4F * var2;
		leg3->m_rotateAngleX = cosf(var1 * 0.6662F + 3.14159265358979323846f) * 1.4F * var2;
		leg4->m_rotateAngleX = cosf(var1 * 0.6662F) * 1.4F * var2;
}
