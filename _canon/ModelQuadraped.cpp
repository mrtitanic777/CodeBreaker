#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelQuadraped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelQuadraped.h"
#include "MathHelper.h"
#include "ModelBase.h"
#include "ModelRenderer.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelQuadraped::ModelQuadraped(int var1, float var2)
	: head(NULL)
	, body(NULL)
	, leg1(NULL)
	, leg2(NULL)
	, leg3(NULL)
	, leg4(NULL)
{
	this->head = new ModelRenderer(0, 0);
	this->head->addBox(-4.0F, -4.0F, -8.0F, 8, 8, 8, var2);
	this->head->setPosition(0.0F, (float)(18 - var1), -6.0F);
	this->body = new ModelRenderer(28, 8);
	this->body->addBox(-5.0F, -10.0F, -7.0F, 10, 16, 8, var2);
	this->body->setPosition(0.0F, (float)(17 - var1), 2.0F);
	this->leg1 = new ModelRenderer(0, 16);
	this->leg1->addBox(-2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
	this->leg1->setPosition(-3.0F, (float)(24 - var1), 7.0F);
	this->leg2 = new ModelRenderer(0, 16);
	this->leg2->addBox(-2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
	this->leg2->setPosition(3.0F, (float)(24 - var1), 7.0F);
	this->leg3 = new ModelRenderer(0, 16);
	this->leg3->addBox(-2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
	this->leg3->setPosition(-3.0F, (float)(24 - var1), -5.0F);
	this->leg4 = new ModelRenderer(0, 16);
	this->leg4->addBox(-2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
	this->leg4->setPosition(3.0F, (float)(24 - var1), -5.0F);
}

ModelQuadraped::~ModelQuadraped()
{
	delete body;
	body = NULL;
	delete leg1;
	leg1 = NULL;
	delete leg2;
	leg2 = NULL;
	delete leg3;
	leg3 = NULL;
	delete leg4;
	leg4 = NULL;
}

void ModelQuadraped::render(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->setRotationAngles(var1, var2, var3, var4, var5, var6);
	this->head->render(var6);
	this->body->render(var6);
	this->leg1->render(var6);
	this->leg2->render(var6);
	this->leg3->render(var6);
	this->leg4->render(var6);
}

void ModelQuadraped::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->head->rotateAngleX = -(var5 / (180.0F / (float)3.1415927F));
	this->head->rotateAngleY = var4 / (180.0F / (float)3.1415927F);
	this->body->rotateAngleX = (float)3.1415927F * 0.5F;
	this->leg1->rotateAngleX = MathHelper::cos(var1 * 0.6662F) * 1.4F * var2;
	this->leg2->rotateAngleX = MathHelper::cos(var1 * 0.6662F + (float)3.1415927F) * 1.4F * var2;
	this->leg3->rotateAngleX = MathHelper::cos(var1 * 0.6662F + (float)3.1415927F) * 1.4F * var2;
	this->leg4->rotateAngleX = MathHelper::cos(var1 * 0.6662F) * 1.4F * var2;
}

void ModelQuadraped::initStatics()
{
}
