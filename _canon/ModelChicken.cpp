#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelChicken.h"
#include "MathHelper.h"
#include "ModelBase.h"
#include "ModelRenderer.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelChicken::ModelChicken()
	: head(NULL)
	, body(NULL)
	, rightLeg(NULL)
	, leftLeg(NULL)
	, rightWing(NULL)
	, leftWing(NULL)
	, bill(NULL)
	, chin(NULL)
{
	unsigned char var1 = 16;
	this->head = new ModelRenderer(0, 0);
	this->head->addBox(-2.0F, -6.0F, -2.0F, 4, 6, 3, 0.0F);
	this->head->setPosition(0.0F, (float)(-1 + var1), -4.0F);
	this->bill = new ModelRenderer(14, 0);
	this->bill->addBox(-2.0F, -4.0F, -4.0F, 4, 2, 2, 0.0F);
	this->bill->setPosition(0.0F, (float)(-1 + var1), -4.0F);
	this->chin = new ModelRenderer(14, 4);
	this->chin->addBox(-1.0F, -2.0F, -3.0F, 2, 2, 2, 0.0F);
	this->chin->setPosition(0.0F, (float)(-1 + var1), -4.0F);
	this->body = new ModelRenderer(0, 9);
	this->body->addBox(-3.0F, -4.0F, -3.0F, 6, 8, 6, 0.0F);
	this->body->setPosition(0.0F, (float)(0 + var1), 0.0F);
	this->rightLeg = new ModelRenderer(26, 0);
	this->rightLeg->addBox(-1.0F, 0.0F, -3.0F, 3, 5, 3);
	this->rightLeg->setPosition(-2.0F, (float)(3 + var1), 1.0F);
	this->leftLeg = new ModelRenderer(26, 0);
	this->leftLeg->addBox(-1.0F, 0.0F, -3.0F, 3, 5, 3);
	this->leftLeg->setPosition(1.0F, (float)(3 + var1), 1.0F);
	this->rightWing = new ModelRenderer(24, 13);
	this->rightWing->addBox(0.0F, 0.0F, -3.0F, 1, 4, 6);
	this->rightWing->setPosition(-4.0F, (float)(-3 + var1), 0.0F);
	this->leftWing = new ModelRenderer(24, 13);
	this->leftWing->addBox(-1.0F, 0.0F, -3.0F, 1, 4, 6);
	this->leftWing->setPosition(4.0F, (float)(-3 + var1), 0.0F);
}

ModelChicken::~ModelChicken()
{
	delete bill;
	bill = NULL;
	delete body;
	body = NULL;
	delete chin;
	chin = NULL;
	delete head;
	head = NULL;
	delete leftLeg;
	leftLeg = NULL;
	delete leftWing;
	leftWing = NULL;
	delete rightLeg;
	rightLeg = NULL;
	delete rightWing;
	rightWing = NULL;
}

void ModelChicken::render(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->setRotationAngles(var1, var2, var3, var4, var5, var6);
	this->head->render(var6);
	this->bill->render(var6);
	this->chin->render(var6);
	this->body->render(var6);
	this->rightLeg->render(var6);
	this->leftLeg->render(var6);
	this->rightWing->render(var6);
	this->leftWing->render(var6);
}

void ModelChicken::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->head->rotateAngleX = -(var5 / (180.0F / (float)3.1415927F));
	this->head->rotateAngleY = var4 / (180.0F / (float)3.1415927F);
	this->bill->rotateAngleX = this->head->rotateAngleX;
	this->bill->rotateAngleY = this->head->rotateAngleY;
	this->chin->rotateAngleX = this->head->rotateAngleX;
	this->chin->rotateAngleY = this->head->rotateAngleY;
	this->body->rotateAngleX = (float)3.1415927F * 0.5F;
	this->rightLeg->rotateAngleX = MathHelper::cos(var1 * 0.6662F) * 1.4F * var2;
	this->leftLeg->rotateAngleX = MathHelper::cos(var1 * 0.6662F + (float)3.1415927F) * 1.4F * var2;
	this->rightWing->rotateAngleZ = var3;
	this->leftWing->rotateAngleZ = -var3;
}

void ModelChicken::initStatics()
{
}
