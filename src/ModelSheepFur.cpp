#include "stdafx.h"
// Auto-generated from ModelSheepFur.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSheepFur.h"
#include "Common.h"
#include "ModelQuadruped.h"
#include "ModelRenderer.h"

ModelSheepFur::ModelSheepFur()
	: ModelQuadruped(12, 0.0f)
{
		head = new ModelRenderer(0, 0);
		head->addBox(-3.0F, -4.0F, -4.0F, 6, 6, 6, 0.6F);
		head->setRotationPoint(0.0F, 6.0F, -8.0F);
		body = new ModelRenderer(28, 8);
		body->addBox(-4.0F, -10.0F, -7.0F, 8, 16, 6, 1.75F);
		body->setRotationPoint(0.0F, 5.0F, 2.0F);
		leg1 = new ModelRenderer(0, 16);
		leg1->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, 0.5F);
		leg1->setRotationPoint(-3.0F, 12.0F, 7.0F);
		leg2 = new ModelRenderer(0, 16);
		leg2->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, 0.5F);
		leg2->setRotationPoint(3.0F, 12.0F, 7.0F);
		leg3 = new ModelRenderer(0, 16);
		leg3->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, 0.5F);
		leg3->setRotationPoint(-3.0F, 12.0F, -5.0F);
		leg4 = new ModelRenderer(0, 16);
		leg4->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, 0.5F);
		leg4->setRotationPoint(3.0F, 12.0F, -5.0F);
}

ModelSheepFur::~ModelSheepFur() {
	// No dynamic resources
}
