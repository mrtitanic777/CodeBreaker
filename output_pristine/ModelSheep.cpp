#include "stdafx.h"
// Auto-generated from ModelSheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSheep.h"
#include "Common.h"
#include "ModelQuadruped.h"
#include "ModelRenderer.h"

ModelSheep::ModelSheep()
	: ModelQuadruped(12, 0.0f)
{
		head = new ModelRenderer(0, 0);
		head->addBox(-3.0F, -4.0F, -6.0F, 6, 6, 8, 0.0F);
		head->setRotationPoint(0.0F, 6.0F, -8.0F);
		body = new ModelRenderer(28, 8);
		body->addBox(-4.0F, -10.0F, -7.0F, 8, 16, 6, 0.0F);
		body->setRotationPoint(0.0F, 5.0F, 2.0F);
}

ModelSheep::~ModelSheep() {
	// No dynamic resources
}
