#include "stdafx.h"
// Auto-generated from ModelCow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelCow.h"
#include "Common.h"
#include "ModelRenderer.h"
#include "ModelQuadraped.h"

ModelCow::ModelCow()
	: ModelQuadraped(12, 0.0f)
	, m_udders(NULL)
	, m_horn1(NULL)
	, m_horn2(NULL)
{
		head = new ModelRenderer(0, 0);
		head->addBox(-4.0F, -4.0F, -6.0F, 8, 8, 6, 0.0F);
		head->setPosition(0.0F, 4.0F, -8.0F);
		m_horn1 = new ModelRenderer(22, 0);
		m_horn1->addBox(-4.0F, -5.0F, -4.0F, 1, 3, 1, 0.0F);
		m_horn1->setPosition(0.0F, 3.0F, -7.0F);
		m_horn2 = new ModelRenderer(22, 0);
		m_horn2->addBox(4.0F, -5.0F, -4.0F, 1, 3, 1, 0.0F);
		m_horn2->setPosition(0.0F, 3.0F, -7.0F);
		m_udders = new ModelRenderer(52, 0);
		m_udders->addBox(-2.0F, -3.0F, 0.0F, 4, 6, 2, 0.0F);
		m_udders->setPosition(0.0F, 14.0F, 6.0F);
		m_udders->m_rotateAngleX = 3.14159265358979323846f * 0.5F;
		body = new ModelRenderer(18, 4);
		body->addBox(-6.0F, -10.0F, -7.0F, 12, 18, 10, 0.0F);
		body->setPosition(0.0F, 5.0F, 2.0F);
		--leg1->m_offsetX;
		++leg2->m_offsetX;
		leg1->m_offsetZ += 0.0F;
		leg2->m_offsetZ += 0.0F;
		--leg3->m_offsetX;
		++leg4->m_offsetX;
		--leg3->m_offsetZ;
		--leg4->m_offsetZ;
}

ModelCow::~ModelCow() {
	m_udders = NULL;
	m_horn1 = NULL;
	m_horn2 = NULL;
}

void ModelCow::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		ModelQuadraped::render(var1, var2, var3, var4, var5, var6);
		m_horn1->render(var6);
		m_horn2->render(var6);
		m_udders->render(var6);
}

void ModelCow::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		ModelQuadraped::setRotationAngles(var1, var2, var3, var4, var5, var6);
		m_horn1->m_rotateAngleY = head->m_rotateAngleY;
		m_horn1->m_rotateAngleX = head->m_rotateAngleX;
		m_horn2->m_rotateAngleY = head->m_rotateAngleY;
		m_horn2->m_rotateAngleX = head->m_rotateAngleX;
}
