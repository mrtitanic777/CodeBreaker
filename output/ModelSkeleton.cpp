#include "stdafx.h"
// Auto-generated from ModelSkeleton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSkeleton.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelSkeleton::ModelSkeleton()
{
		m_a = new ModelRenderer(40, 16);
		m_a->addBox(-1.0F, -2.0F, -1.0F, 2, 12, 2, 0.0F);
		m_a->setRotationPoint(-5.0F, 2.0F, 0.0F);
		m_b = new ModelRenderer(40, 16);
		m_b->m_mirror = true;
		m_b->addBox(-1.0F, -2.0F, -1.0F, 2, 12, 2, 0.0F);
		m_b->setRotationPoint(5.0F, 2.0F, 0.0F);
		m_c = new ModelRenderer(0, 16);
		m_c->addBox(-1.0F, 0.0F, -1.0F, 2, 12, 2, 0.0F);
		m_c->setRotationPoint(-2.0F, 12.0F, 0.0F);
		m_d = new ModelRenderer(0, 16);
		m_d->m_mirror = true;
		m_d->addBox(-1.0F, 0.0F, -1.0F, 2, 12, 2, 0.0F);
		m_d->setRotationPoint(2.0F, 12.0F, 0.0F);
}

ModelSkeleton::~ModelSkeleton() {
	// No dynamic resources
}
