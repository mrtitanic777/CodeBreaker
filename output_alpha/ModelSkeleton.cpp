#include "stdafx.h"
// Auto-generated from ModelSkeleton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSkeleton.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelSkeleton::ModelSkeleton()
{
		float var1 = 0.0F;
		m_bipedRightArm = new ModelRenderer(40, 16);
		m_bipedRightArm->addBox(-1.0F, -2.0F, -1.0F, 2, 12, 2, var1);
		m_bipedRightArm->setPosition(-5.0F, 2.0F, 0.0F);
		m_bipedLeftArm = new ModelRenderer(40, 16);
		m_bipedLeftArm->m_mirror = true;
		m_bipedLeftArm->addBox(-1.0F, -2.0F, -1.0F, 2, 12, 2, var1);
		m_bipedLeftArm->setPosition(5.0F, 2.0F, 0.0F);
		m_bipedRightLeg = new ModelRenderer(0, 16);
		m_bipedRightLeg->addBox(-1.0F, 0.0F, -1.0F, 2, 12, 2, var1);
		m_bipedRightLeg->setPosition(-2.0F, 12.0F, 0.0F);
		m_bipedLeftLeg = new ModelRenderer(0, 16);
		m_bipedLeftLeg->m_mirror = true;
		m_bipedLeftLeg->addBox(-1.0F, 0.0F, -1.0F, 2, 12, 2, var1);
		m_bipedLeftLeg->setPosition(2.0F, 12.0F, 0.0F);
}

ModelSkeleton::~ModelSkeleton() {
	// No dynamic resources
}
