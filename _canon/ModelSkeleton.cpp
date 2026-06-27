#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelSkeleton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSkeleton.h"
#include "ModelRenderer.h"
#include "ModelZombie.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelSkeleton::ModelSkeleton()
{
	float var1 = 0.0F;
	this->bipedRightArm = new ModelRenderer(40, 16);
	this->bipedRightArm->addBox(-1.0F, -2.0F, -1.0F, 2, 12, 2, var1);
	this->bipedRightArm->setPosition(-5.0F, 2.0F, 0.0F);
	this->bipedLeftArm = new ModelRenderer(40, 16);
	this->bipedLeftArm->mirror = true;
	this->bipedLeftArm->addBox(-1.0F, -2.0F, -1.0F, 2, 12, 2, var1);
	this->bipedLeftArm->setPosition(5.0F, 2.0F, 0.0F);
	this->bipedRightLeg = new ModelRenderer(0, 16);
	this->bipedRightLeg->addBox(-1.0F, 0.0F, -1.0F, 2, 12, 2, var1);
	this->bipedRightLeg->setPosition(-2.0F, 12.0F, 0.0F);
	this->bipedLeftLeg = new ModelRenderer(0, 16);
	this->bipedLeftLeg->mirror = true;
	this->bipedLeftLeg->addBox(-1.0F, 0.0F, -1.0F, 2, 12, 2, var1);
	this->bipedLeftLeg->setPosition(2.0F, 12.0F, 0.0F);
}

ModelSkeleton::~ModelSkeleton()
{
}

void ModelSkeleton::initStatics()
{
}
