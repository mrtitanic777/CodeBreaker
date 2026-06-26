#include "stdafx.h"
// Auto-generated from ModelBiped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBiped.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelBiped::ModelBiped()
	: m_e(NULL)
	, m_f(NULL)
	, m_g(NULL)
	, m_a(NULL)
	, m_b(NULL)
	, m_c(NULL)
	, m_d(NULL)
{
  // PORT: constructor delegation  // was: this(0.0F);
}

ModelBiped::ModelBiped(float var1)
	: m_e(NULL)
	, m_f(NULL)
	, m_g(NULL)
	, m_a(NULL)
	, m_b(NULL)
	, m_c(NULL)
	, m_d(NULL)
{
		m_e = new ModelRenderer(0, 0);
		m_e->addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1);
		m_f = new ModelRenderer(32, 0);
		m_f->addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1 + 0.5F);
		m_g = new ModelRenderer(16, 16);
		m_g->addBox(-4.0F, 0.0F, -2.0F, 8, 12, 4, var1);
		m_a = new ModelRenderer(40, 16);
		m_a->addBox(-3.0F, -2.0F, -2.0F, 4, 12, 4, var1);
		m_a->setRotationPoint(-5.0F, 2.0F, 0.0F);
		m_b = new ModelRenderer(40, 16);
		m_b->m_mirror = true;
		m_b->addBox(-1.0F, -2.0F, -2.0F, 4, 12, 4, var1);
		m_b->setRotationPoint(5.0F, 2.0F, 0.0F);
		m_c = new ModelRenderer(0, 16);
		m_c->addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4, var1);
		m_c->setRotationPoint(-2.0F, 12.0F, 0.0F);
		m_d = new ModelRenderer(0, 16);
		m_d->m_mirror = true;
		m_d->addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4, var1);
		m_d->setRotationPoint(2.0F, 12.0F, 0.0F);
}

ModelBiped::~ModelBiped() {
	m_e = NULL;
	m_f = NULL;
	m_g = NULL;
	m_a = NULL;
	m_b = NULL;
	m_c = NULL;
	m_d = NULL;
}

void ModelBiped::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		setRotationAngles(var1, var2, var3, var4, var5, 1.0F);
		m_e->render(1.0F);
		m_g->render(1.0F);
		m_a->render(1.0F);
		m_b->render(1.0F);
		m_c->render(1.0F);
		m_d->render(1.0F);
		m_f->render(1.0F);
}

void ModelBiped::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		m_e->m_rotateAngleY = var4 / (180.0F / 3.14159265358979323846f);
		m_e->m_rotateAngleX = var5 / (180.0F / 3.14159265358979323846f);
		m_f->m_rotateAngleY = m_e->m_rotateAngleY;
		m_f->m_rotateAngleX = m_e->m_rotateAngleX;
		m_a->m_rotateAngleX = cosf(var1 * 0.6662F + 3.14159265358979323846f) * 2.0F * var2;
		m_a->m_rotateAngleZ = (cosf(var1 * 0.2312F) + 1.0F) * var2;
		m_b->m_rotateAngleX = cosf(var1 * 0.6662F) * 2.0F * var2;
		m_b->m_rotateAngleZ = (cosf(var1 * 0.2812F) - 1.0F) * var2;
		m_c->m_rotateAngleX = cosf(var1 * 0.6662F) * 1.4F * var2;
		m_d->m_rotateAngleX = cosf(var1 * 0.6662F + 3.14159265358979323846f) * 1.4F * var2;
		m_a->m_rotateAngleZ += cosf(var3 * 0.09F) * 0.05F + 0.05F;
		m_b->m_rotateAngleZ -= cosf(var3 * 0.09F) * 0.05F + 0.05F;
		m_a->m_rotateAngleX += sinf(var3 * 0.067F) * 0.05F;
		m_b->m_rotateAngleX -= sinf(var3 * 0.067F) * 0.05F;
}
