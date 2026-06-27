#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from SignModel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SignModel.h"
#include "ModelRenderer.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

SignModel::SignModel()
	: signBoard(NULL)
	, signPost(NULL)
{
	this->signBoard = new ModelRenderer(0, 0);
	this->signBoard->addBox(-12.0F, -14.0F, -1.0F, 24, 12, 2, 0.0F);
	this->signPost = new ModelRenderer(0, 14);
	this->signPost->addBox(-1.0F, -2.0F, -1.0F, 2, 14, 2, 0.0F);
}

SignModel::~SignModel()
{
	delete signPost;
	signPost = NULL;
}

void SignModel::renderSign()
{
	this->signBoard->render(1.0F / 16.0F);
	this->signPost->render(1.0F / 16.0F);
}

void SignModel::initStatics()
{
}
