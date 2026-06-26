#include "stdafx.h"
// Auto-generated from ModelSlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSlime.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelSlime::ModelSlime()
	: m_field_1258_a(NULL)
	, m_field_1257_b(NULL)
	, m_field_1260_c(NULL)
	, m_field_1259_d(NULL)
{
}

ModelSlime::ModelSlime(int var1)
	: m_field_1258_a(NULL)
	, m_field_1257_b(NULL)
	, m_field_1260_c(NULL)
	, m_field_1259_d(NULL)
{
		m_field_1258_a = new ModelRenderer(0, var1);
		m_field_1258_a->func_921_a(-4.0F, 16.0F, -4.0F, 8, 8, 8);
		if(var1 > 0) {
			m_field_1258_a = new ModelRenderer(0, var1);
			m_field_1258_a->func_921_a(-3.0F, 17.0F, -3.0F, 6, 6, 6);
			m_field_1257_b = new ModelRenderer(32, 0);
			m_field_1257_b->func_921_a(-3.25F, 18.0F, -3.5F, 2, 2, 2);
			m_field_1260_c = new ModelRenderer(32, 4);
			m_field_1260_c->func_921_a(1.25F, 18.0F, -3.5F, 2, 2, 2);
			m_field_1259_d = new ModelRenderer(32, 8);
			m_field_1259_d->func_921_a(0.0F, 21.0F, -3.5F, 1, 1, 1);
		}

}

ModelSlime::~ModelSlime() {
	m_field_1258_a = NULL;
	m_field_1257_b = NULL;
	m_field_1260_c = NULL;
	m_field_1259_d = NULL;
}

void ModelSlime::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
	// TODO: implement
}

void ModelSlime::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		setRotationAngles(var1, var2, var3, var4, var5, var6);
		m_field_1258_a->render(var6);
		if(m_field_1257_b != NULL) {
			m_field_1257_b->render(var6);
			m_field_1260_c->render(var6);
			m_field_1259_d->render(var6);
		}

}
