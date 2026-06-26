#include "stdafx.h"
// Auto-generated from ModelBiped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBiped.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelBiped::ModelBiped()
	: m_bipedHead(NULL)
	, m_field_1285_b(NULL)
	, m_field_1284_c(NULL)
	, m_bipedRightArm(NULL)
	, m_bipedLeftArm(NULL)
	, m_bipedRightLeg(NULL)
	, m_bipedLeftLeg(NULL)
	, m_field_1279_h(false)
	, m_field_1278_i(false)
	, m_field_1277_j(false)
{
  // PORT: constructor delegation  // was: this(0.0F);
}

ModelBiped::ModelBiped(float var1)
	: m_bipedHead(NULL)
	, m_field_1285_b(NULL)
	, m_field_1284_c(NULL)
	, m_bipedRightArm(NULL)
	, m_bipedLeftArm(NULL)
	, m_bipedRightLeg(NULL)
	, m_bipedLeftLeg(NULL)
	, m_field_1279_h(false)
	, m_field_1278_i(false)
	, m_field_1277_j(false)
{
  // PORT: constructor delegation  // was: this(var1, 0.0F);
}

ModelBiped::ModelBiped(float var1, float var2)
	: m_bipedHead(NULL)
	, m_field_1285_b(NULL)
	, m_field_1284_c(NULL)
	, m_bipedRightArm(NULL)
	, m_bipedLeftArm(NULL)
	, m_bipedRightLeg(NULL)
	, m_bipedLeftLeg(NULL)
	, m_field_1279_h(false)
	, m_field_1278_i(false)
	, m_field_1277_j(false)
{
		m_field_1279_h = false;
		m_field_1278_i = false;
		m_field_1277_j = false;
		m_bipedHead = new ModelRenderer(0, 0);
		m_bipedHead->addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1);
		m_bipedHead->setPosition(0.0F, 0.0F + var2, 0.0F);
		m_field_1285_b = new ModelRenderer(32, 0);
		m_field_1285_b->addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1 + 0.5F);
		m_field_1285_b->setPosition(0.0F, 0.0F + var2, 0.0F);
		m_field_1284_c = new ModelRenderer(16, 16);
		m_field_1284_c->addBox(-4.0F, 0.0F, -2.0F, 8, 12, 4, var1);
		m_field_1284_c->setPosition(0.0F, 0.0F + var2, 0.0F);
		m_bipedRightArm = new ModelRenderer(40, 16);
		m_bipedRightArm->addBox(-3.0F, -2.0F, -2.0F, 4, 12, 4, var1);
		m_bipedRightArm->setPosition(-5.0F, 2.0F + var2, 0.0F);
		m_bipedLeftArm = new ModelRenderer(40, 16);
		m_bipedLeftArm->m_mirror = true;
		m_bipedLeftArm->addBox(-1.0F, -2.0F, -2.0F, 4, 12, 4, var1);
		m_bipedLeftArm->setPosition(5.0F, 2.0F + var2, 0.0F);
		m_bipedRightLeg = new ModelRenderer(0, 16);
		m_bipedRightLeg->addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4, var1);
		m_bipedRightLeg->setPosition(-2.0F, 12.0F + var2, 0.0F);
		m_bipedLeftLeg = new ModelRenderer(0, 16);
		m_bipedLeftLeg->m_mirror = true;
		m_bipedLeftLeg->addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4, var1);
		m_bipedLeftLeg->setPosition(2.0F, 12.0F + var2, 0.0F);
}

ModelBiped::~ModelBiped() {
	m_bipedHead = NULL;
	m_field_1285_b = NULL;
	m_field_1284_c = NULL;
	m_bipedRightArm = NULL;
	m_bipedLeftArm = NULL;
	m_bipedRightLeg = NULL;
	m_bipedLeftLeg = NULL;
}

void ModelBiped::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		setRotationAngles(var1, var2, var3, var4, var5, var6);
		m_bipedHead->render(var6);
		m_field_1284_c->render(var6);
		m_bipedRightArm->render(var6);
		m_bipedLeftArm->render(var6);
		m_bipedRightLeg->render(var6);
		m_bipedLeftLeg->render(var6);
		m_field_1285_b->render(var6);
}

void ModelBiped::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		m_bipedHead->m_rotateAngleY = var4 / (180.0F / 3.14159265358979323846f);
		m_bipedHead->m_rotateAngleX = var5 / (180.0F / 3.14159265358979323846f);
		m_field_1285_b->m_rotateAngleY = m_bipedHead->m_rotateAngleY;
		m_field_1285_b->m_rotateAngleX = m_bipedHead->m_rotateAngleX;
		m_bipedRightArm->m_rotateAngleX = cosf(var1 * 0.6662F + 3.14159265358979323846f) * 2.0F * var2 * 0.5F;
		m_bipedLeftArm->m_rotateAngleX = cosf(var1 * 0.6662F) * 2.0F * var2 * 0.5F;
		m_bipedRightArm->m_rotateAngleZ = 0.0F;
		m_bipedLeftArm->m_rotateAngleZ = 0.0F;
		m_bipedRightLeg->m_rotateAngleX = cosf(var1 * 0.6662F) * 1.4F * var2;
		m_bipedLeftLeg->m_rotateAngleX = cosf(var1 * 0.6662F + 3.14159265358979323846f) * 1.4F * var2;
		m_bipedRightLeg->m_rotateAngleY = 0.0F;
		m_bipedLeftLeg->m_rotateAngleY = 0.0F;
		if(m_field_1243_l) {
			m_bipedRightArm->m_rotateAngleX += 3.14159265358979323846f * -0.2F;
			m_bipedLeftArm->m_rotateAngleX += 3.14159265358979323846f * -0.2F;
			m_bipedRightLeg->m_rotateAngleX = 3.14159265358979323846f * -0.4F;
			m_bipedLeftLeg->m_rotateAngleX = 3.14159265358979323846f * -0.4F;
			m_bipedRightLeg->m_rotateAngleY = 3.14159265358979323846f * 0.1F;
			m_bipedLeftLeg->m_rotateAngleY = 3.14159265358979323846f * -0.1F;
		}

		if(m_field_1279_h) {
			m_bipedLeftArm->m_rotateAngleX = m_bipedLeftArm->m_rotateAngleX * 0.5F - 3.14159265358979323846f * 0.1F;
		}

		if(m_field_1278_i) {
			m_bipedRightArm->m_rotateAngleX = m_bipedRightArm->m_rotateAngleX * 0.5F - 3.14159265358979323846f * 0.1F;
		}

		m_bipedRightArm->m_rotateAngleY = 0.0F;
		m_bipedLeftArm->m_rotateAngleY = 0.0F;
		if(m_field_1244_k > -9990.0F) {
			float var7 = m_field_1244_k;
			m_field_1284_c->m_rotateAngleY = sinf(sqrtf(var7) * 3.14159265358979323846f * 2.0F) * 0.2F;
			m_bipedRightArm->m_offsetZ = sinf(m_field_1284_c->m_rotateAngleY) * 5.0F;
			m_bipedRightArm->m_offsetX = -cosf(m_field_1284_c->m_rotateAngleY) * 5.0F;
			m_bipedLeftArm->m_offsetZ = -sinf(m_field_1284_c->m_rotateAngleY) * 5.0F;
			m_bipedLeftArm->m_offsetX = cosf(m_field_1284_c->m_rotateAngleY) * 5.0F;
			m_bipedRightArm->m_rotateAngleY += m_field_1284_c->m_rotateAngleY;
			m_bipedLeftArm->m_rotateAngleY += m_field_1284_c->m_rotateAngleY;
			m_bipedLeftArm->m_rotateAngleX += m_field_1284_c->m_rotateAngleY;
			var7 = 1.0F - m_field_1244_k;
			var7 *= var7;
			var7 *= var7;
			var7 = 1.0F - var7;
			float var8 = sinf(var7 * 3.14159265358979323846f);
			float var9 = sinf(m_field_1244_k * 3.14159265358979323846f) * -(m_bipedHead->m_rotateAngleX - 0.7F) * (12.0F / 16.0F);
			m_bipedRightArm->m_rotateAngleX = (float)((double)m_bipedRightArm->m_rotateAngleX - ((double)var8 * 1.2f + (double)var9));
			m_bipedRightArm->m_rotateAngleY += m_field_1284_c->m_rotateAngleY * 2.0F;
			m_bipedRightArm->m_rotateAngleZ = sinf(m_field_1244_k * 3.14159265358979323846f) * -0.4F;
		}

		if(m_field_1277_j) {
			m_field_1284_c->m_rotateAngleX = 0.5F;
			m_bipedRightLeg->m_rotateAngleX -= 0.0F;
			m_bipedLeftLeg->m_rotateAngleX -= 0.0F;
			m_bipedRightArm->m_rotateAngleX += 0.4F;
			m_bipedLeftArm->m_rotateAngleX += 0.4F;
			m_bipedRightLeg->m_offsetZ = 4.0F;
			m_bipedLeftLeg->m_offsetZ = 4.0F;
			m_bipedRightLeg->m_offsetY = 9.0F;
			m_bipedLeftLeg->m_offsetY = 9.0F;
			m_bipedHead->m_offsetY = 1.0F;
		} else {
			m_field_1284_c->m_rotateAngleX = 0.0F;
			m_bipedRightLeg->m_offsetZ = 0.0F;
			m_bipedLeftLeg->m_offsetZ = 0.0F;
			m_bipedRightLeg->m_offsetY = 12.0F;
			m_bipedLeftLeg->m_offsetY = 12.0F;
			m_bipedHead->m_offsetY = 0.0F;
		}

		m_bipedRightArm->m_rotateAngleZ += cosf(var3 * 0.09F) * 0.05F + 0.05F;
		m_bipedLeftArm->m_rotateAngleZ -= cosf(var3 * 0.09F) * 0.05F + 0.05F;
		m_bipedRightArm->m_rotateAngleX += sinf(var3 * 0.067F) * 0.05F;
		m_bipedLeftArm->m_rotateAngleX -= sinf(var3 * 0.067F) * 0.05F;
}
