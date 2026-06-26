#include "stdafx.h"
// Auto-generated from SignModel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SignModel.h"
#include "Common.h"
#include "ModelRenderer.h"

SignModel::SignModel()
	: m_field_1346_a(new ModelRenderer(0, 0))
	, m_field_1345_b(NULL)
{
		m_field_1346_a->addBox(-12.0F, -14.0F, -1.0F, 24, 12, 2, 0.0F);
		m_field_1345_b = new ModelRenderer(0, 14);
		m_field_1345_b->addBox(-1.0F, -2.0F, -1.0F, 2, 14, 2, 0.0F);
}

SignModel::~SignModel() {
	m_field_1346_a = NULL;
	m_field_1345_b = NULL;
}

void SignModel::func_887_a() {
		m_field_1346_a->render(1.0F / 16.0F);
		m_field_1345_b->render(1.0F / 16.0F);
}
