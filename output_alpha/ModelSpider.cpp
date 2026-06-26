#include "stdafx.h"
// Auto-generated from ModelSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSpider.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelSpider::ModelSpider()
	: m_field_1255_a(NULL)
	, m_field_1254_b(NULL)
	, m_field_1253_c(NULL)
	, m_field_1252_d(NULL)
	, m_field_1251_e(NULL)
	, m_field_1250_f(NULL)
	, m_field_1249_g(NULL)
	, m_field_1248_h(NULL)
	, m_field_1247_i(NULL)
	, m_field_1246_j(NULL)
	, m_field_1245_m(NULL)
{
		float var1 = 0.0F;
		unsigned char var2 = 15;
		m_field_1255_a = new ModelRenderer(32, 4);
		m_field_1255_a->addBox(-4.0F, -4.0F, -8.0F, 8, 8, 8, var1);
		m_field_1255_a->setPosition(0.0F, (float)(0 + var2), -3.0F);
		m_field_1254_b = new ModelRenderer(0, 0);
		m_field_1254_b->addBox(-3.0F, -3.0F, -3.0F, 6, 6, 6, var1);
		m_field_1254_b->setPosition(0.0F, (float)var2, 0.0F);
		m_field_1253_c = new ModelRenderer(0, 12);
		m_field_1253_c->addBox(-5.0F, -4.0F, -6.0F, 10, 8, 12, var1);
		m_field_1253_c->setPosition(0.0F, (float)(0 + var2), 9.0F);
		m_field_1252_d = new ModelRenderer(18, 0);
		m_field_1252_d->addBox(-15.0F, -1.0F, -1.0F, 16, 2, 2, var1);
		m_field_1252_d->setPosition(-4.0F, (float)(0 + var2), 2.0F);
		m_field_1251_e = new ModelRenderer(18, 0);
		m_field_1251_e->addBox(-1.0F, -1.0F, -1.0F, 16, 2, 2, var1);
		m_field_1251_e->setPosition(4.0F, (float)(0 + var2), 2.0F);
		m_field_1250_f = new ModelRenderer(18, 0);
		m_field_1250_f->addBox(-15.0F, -1.0F, -1.0F, 16, 2, 2, var1);
		m_field_1250_f->setPosition(-4.0F, (float)(0 + var2), 1.0F);
		m_field_1249_g = new ModelRenderer(18, 0);
		m_field_1249_g->addBox(-1.0F, -1.0F, -1.0F, 16, 2, 2, var1);
		m_field_1249_g->setPosition(4.0F, (float)(0 + var2), 1.0F);
		m_field_1248_h = new ModelRenderer(18, 0);
		m_field_1248_h->addBox(-15.0F, -1.0F, -1.0F, 16, 2, 2, var1);
		m_field_1248_h->setPosition(-4.0F, (float)(0 + var2), 0.0F);
		m_field_1247_i = new ModelRenderer(18, 0);
		m_field_1247_i->addBox(-1.0F, -1.0F, -1.0F, 16, 2, 2, var1);
		m_field_1247_i->setPosition(4.0F, (float)(0 + var2), 0.0F);
		m_field_1246_j = new ModelRenderer(18, 0);
		m_field_1246_j->addBox(-15.0F, -1.0F, -1.0F, 16, 2, 2, var1);
		m_field_1246_j->setPosition(-4.0F, (float)(0 + var2), -1.0F);
		m_field_1245_m = new ModelRenderer(18, 0);
		m_field_1245_m->addBox(-1.0F, -1.0F, -1.0F, 16, 2, 2, var1);
		m_field_1245_m->setPosition(4.0F, (float)(0 + var2), -1.0F);
}

ModelSpider::~ModelSpider() {
	m_field_1255_a = NULL;
	m_field_1254_b = NULL;
	m_field_1253_c = NULL;
	m_field_1252_d = NULL;
	m_field_1251_e = NULL;
	m_field_1250_f = NULL;
	m_field_1249_g = NULL;
	m_field_1248_h = NULL;
	m_field_1247_i = NULL;
	m_field_1246_j = NULL;
	m_field_1245_m = NULL;
}

void ModelSpider::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		setRotationAngles(var1, var2, var3, var4, var5, var6);
		m_field_1255_a->render(var6);
		m_field_1254_b->render(var6);
		m_field_1253_c->render(var6);
		m_field_1252_d->render(var6);
		m_field_1251_e->render(var6);
		m_field_1250_f->render(var6);
		m_field_1249_g->render(var6);
		m_field_1248_h->render(var6);
		m_field_1247_i->render(var6);
		m_field_1246_j->render(var6);
		m_field_1245_m->render(var6);
}

void ModelSpider::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		m_field_1255_a->m_rotateAngleY = var4 / (180.0F / 3.14159265358979323846f);
		m_field_1255_a->m_rotateAngleX = var5 / (180.0F / 3.14159265358979323846f);
		float var7 = 3.14159265358979323846f * 0.25F;
		m_field_1252_d->m_rotateAngleZ = -var7;
		m_field_1251_e->m_rotateAngleZ = var7;
		m_field_1250_f->m_rotateAngleZ = -var7 * 0.74F;
		m_field_1249_g->m_rotateAngleZ = var7 * 0.74F;
		m_field_1248_h->m_rotateAngleZ = -var7 * 0.74F;
		m_field_1247_i->m_rotateAngleZ = var7 * 0.74F;
		m_field_1246_j->m_rotateAngleZ = -var7;
		m_field_1245_m->m_rotateAngleZ = var7;
		float var8 = -0.0F;
		float var9 = 3.14159265358979323846f * 0.125F;
		m_field_1252_d->m_rotateAngleY = var9 * 2.0F + var8;
		m_field_1251_e->m_rotateAngleY = -var9 * 2.0F - var8;
		m_field_1250_f->m_rotateAngleY = var9 * 1.0F + var8;
		m_field_1249_g->m_rotateAngleY = -var9 * 1.0F - var8;
		m_field_1248_h->m_rotateAngleY = -var9 * 1.0F + var8;
		m_field_1247_i->m_rotateAngleY = var9 * 1.0F - var8;
		m_field_1246_j->m_rotateAngleY = -var9 * 2.0F + var8;
		m_field_1245_m->m_rotateAngleY = var9 * 2.0F - var8;
		float var10 = -(cosf(var1 * 0.6662F * 2.0F + 0.0F) * 0.4F) * var2;
		float var11 = -(cosf(var1 * 0.6662F * 2.0F + 3.14159265358979323846f) * 0.4F) * var2;
		float var12 = -(cosf(var1 * 0.6662F * 2.0F + 3.14159265358979323846f * 0.5F) * 0.4F) * var2;
		float var13 = -(cosf(var1 * 0.6662F * 2.0F + 3.14159265358979323846f * 3.0F / 2.0F) * 0.4F) * var2;
		float var14 = abs(sinf(var1 * 0.6662F + 0.0F) * 0.4F) * var2;
		float var15 = abs(sinf(var1 * 0.6662F + 3.14159265358979323846f) * 0.4F) * var2;
		float var16 = abs(sinf(var1 * 0.6662F + 3.14159265358979323846f * 0.5F) * 0.4F) * var2;
		float var17 = abs(sinf(var1 * 0.6662F + 3.14159265358979323846f * 3.0F / 2.0F) * 0.4F) * var2;
		m_field_1252_d->m_rotateAngleY += var10;
		m_field_1251_e->m_rotateAngleY += -var10;
		m_field_1250_f->m_rotateAngleY += var11;
		m_field_1249_g->m_rotateAngleY += -var11;
		m_field_1248_h->m_rotateAngleY += var12;
		m_field_1247_i->m_rotateAngleY += -var12;
		m_field_1246_j->m_rotateAngleY += var13;
		m_field_1245_m->m_rotateAngleY += -var13;
		m_field_1252_d->m_rotateAngleZ += var14;
		m_field_1251_e->m_rotateAngleZ += -var14;
		m_field_1250_f->m_rotateAngleZ += var15;
		m_field_1249_g->m_rotateAngleZ += -var15;
		m_field_1248_h->m_rotateAngleZ += var16;
		m_field_1247_i->m_rotateAngleZ += -var16;
		m_field_1246_j->m_rotateAngleZ += var17;
		m_field_1245_m->m_rotateAngleZ += -var17;
}
