#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelSlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelSlime.h"
#include "ModelBase.h"
#include "ModelRenderer.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelSlime::ModelSlime(int var1)
	: body(NULL)
	, rightEye(NULL)
	, leftEye(NULL)
	, mouth(NULL)
{
	this->body = new ModelRenderer(0, var1);
	this->body->addBox(-4.0F, 16.0F, -4.0F, 8, 8, 8);
	if(var1 > 0) {
	this->body = new ModelRenderer(0, var1);
	this->body->addBox(-3.0F, 17.0F, -3.0F, 6, 6, 6);
	this->rightEye = new ModelRenderer(32, 0);
	this->rightEye->addBox(-3.25F, 18.0F, -3.5F, 2, 2, 2);
	this->leftEye = new ModelRenderer(32, 4);
	this->leftEye->addBox(1.25F, 18.0F, -3.5F, 2, 2, 2);
	this->mouth = new ModelRenderer(32, 8);
	this->mouth->addBox(0.0F, 21.0F, -3.5F, 1, 1, 1);
	}
}

ModelSlime::~ModelSlime()
{
	delete body;
	body = NULL;
	delete leftEye;
	leftEye = NULL;
	delete mouth;
	mouth = NULL;
	delete rightEye;
	rightEye = NULL;
}

void ModelSlime::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6)
{
}

void ModelSlime::render(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->setRotationAngles(var1, var2, var3, var4, var5, var6);
	this->body->render(var6);
	if(this->rightEye != NULL) {
	this->rightEye->render(var6);
	this->leftEye->render(var6);
	this->mouth->render(var6);
	}
}

void ModelSlime::initStatics()
{
}
