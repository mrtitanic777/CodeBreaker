#include "stdafx.h"
// Auto-generated from ModelChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelChicken.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelChicken::ModelChicken()
	: m_field_1289_a(NULL)
	, m_field_1288_b(NULL)
	, m_field_1295_c(NULL)
	, m_field_1294_d(NULL)
	, m_field_1293_e(NULL)
	, m_field_1292_f(NULL)
	, m_field_1291_g(NULL)
	, m_field_1290_h(NULL)
{
		unsigned char var1 = 16;
		m_field_1289_a = new ModelRenderer(0, 0);
		m_field_1289_a->addBox(-2.0F, -6.0F, -2.0F, 4, 6, 3, 0.0F);
		m_field_1289_a->setPosition(0.0F, (float)(-1 + var1), -4.0F);
		m_field_1291_g = new ModelRenderer(14, 0);
		m_field_1291_g->addBox(-2.0F, -4.0F, -4.0F, 4, 2, 2, 0.0F);
		m_field_1291_g->setPosition(0.0F, (float)(-1 + var1), -4.0F);
		m_field_1290_h = new ModelRenderer(14, 4);
		m_field_1290_h->addBox(-1.0F, -2.0F, -3.0F, 2, 2, 2, 0.0F);
		m_field_1290_h->setPosition(0.0F, (float)(-1 + var1), -4.0F);
		m_field_1288_b = new ModelRenderer(0, 9);
		m_field_1288_b->addBox(-3.0F, -4.0F, -3.0F, 6, 8, 6, 0.0F);
		m_field_1288_b->setPosition(0.0F, (float)(0 + var1), 0.0F);
		m_field_1295_c = new ModelRenderer(26, 0);
		m_field_1295_c->func_921_a(-1.0F, 0.0F, -3.0F, 3, 5, 3);
		m_field_1295_c->setPosition(-2.0F, (float)(3 + var1), 1.0F);
		m_field_1294_d = new ModelRenderer(26, 0);
		m_field_1294_d->func_921_a(-1.0F, 0.0F, -3.0F, 3, 5, 3);
		m_field_1294_d->setPosition(1.0F, (float)(3 + var1), 1.0F);
		m_field_1293_e = new ModelRenderer(24, 13);
		m_field_1293_e->func_921_a(0.0F, 0.0F, -3.0F, 1, 4, 6);
		m_field_1293_e->setPosition(-4.0F, (float)(-3 + var1), 0.0F);
		m_field_1292_f = new ModelRenderer(24, 13);
		m_field_1292_f->func_921_a(-1.0F, 0.0F, -3.0F, 1, 4, 6);
		m_field_1292_f->setPosition(4.0F, (float)(-3 + var1), 0.0F);
}

ModelChicken::~ModelChicken() {
	m_field_1289_a = NULL;
	m_field_1288_b = NULL;
	m_field_1295_c = NULL;
	m_field_1294_d = NULL;
	m_field_1293_e = NULL;
	m_field_1292_f = NULL;
	m_field_1291_g = NULL;
	m_field_1290_h = NULL;
}

void ModelChicken::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		setRotationAngles(var1, var2, var3, var4, var5, var6);
		m_field_1289_a->render(var6);
		m_field_1291_g->render(var6);
		m_field_1290_h->render(var6);
		m_field_1288_b->render(var6);
		m_field_1295_c->render(var6);
		m_field_1294_d->render(var6);
		m_field_1293_e->render(var6);
		m_field_1292_f->render(var6);
}

void ModelChicken::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		m_field_1289_a->m_rotateAngleX = -(var5 / (180.0F / 3.14159265358979323846f));
		m_field_1289_a->m_rotateAngleY = var4 / (180.0F / 3.14159265358979323846f);
		m_field_1291_g->m_rotateAngleX = m_field_1289_a->m_rotateAngleX;
		m_field_1291_g->m_rotateAngleY = m_field_1289_a->m_rotateAngleY;
		m_field_1290_h->m_rotateAngleX = m_field_1289_a->m_rotateAngleX;
		m_field_1290_h->m_rotateAngleY = m_field_1289_a->m_rotateAngleY;
		m_field_1288_b->m_rotateAngleX = 3.14159265358979323846f * 0.5F;
		m_field_1295_c->m_rotateAngleX = cosf(var1 * 0.6662F) * 1.4F * var2;
		m_field_1294_d->m_rotateAngleX = cosf(var1 * 0.6662F + 3.14159265358979323846f) * 1.4F * var2;
		m_field_1293_e->m_rotateAngleZ = var3;
		m_field_1292_f->m_rotateAngleZ = -var3;
}
