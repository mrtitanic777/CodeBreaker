#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelSheep2.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSheep2.h"
#include "ModelQuadraped.h"
#include "ModelRenderer.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelSheep2::ModelSheep2()
	: ModelQuadraped(12, 0.0F)
{
	this->head = new ModelRenderer(0, 0);
	this->head->addBox(-3.0F, -4.0F, -6.0F, 6, 6, 8, 0.0F);
	this->head->setPosition(0.0F, 6.0F, -8.0F);
	this->body = new ModelRenderer(28, 8);
	this->body->addBox(-4.0F, -10.0F, -7.0F, 8, 16, 6, 0.0F);
	this->body->setPosition(0.0F, 5.0F, 2.0F);
}

ModelSheep2::~ModelSheep2()
{
}

void ModelSheep2::initStatics()
{
}
