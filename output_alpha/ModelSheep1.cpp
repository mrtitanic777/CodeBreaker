#include "stdafx.h"
// Auto-generated from ModelSheep1.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSheep1.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelSheep1::ModelSheep1()
	: ModelQuadraped(12, 0.0f)
{
		head = new ModelRenderer(0, 0);
		head->addBox(-3.0F, -4.0F, -4.0F, 6, 6, 6, 0.6F);
		head->setPosition(0.0F, 6.0F, -8.0F);
		body = new ModelRenderer(28, 8);
		body->addBox(-4.0F, -10.0F, -7.0F, 8, 16, 6, 1.75F);
		body->setPosition(0.0F, 5.0F, 2.0F);
		float var1 = 0.5F;
		leg1 = new ModelRenderer(0, 16);
		leg1->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
		leg1->setPosition(-3.0F, 12.0F, 7.0F);
		leg2 = new ModelRenderer(0, 16);
		leg2->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
		leg2->setPosition(3.0F, 12.0F, 7.0F);
		leg3 = new ModelRenderer(0, 16);
		leg3->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
		leg3->setPosition(-3.0F, 12.0F, -5.0F);
		leg4 = new ModelRenderer(0, 16);
		leg4->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
		leg4->setPosition(3.0F, 12.0F, -5.0F);
}

ModelSheep1::~ModelSheep1() {
	// No dynamic resources
}
