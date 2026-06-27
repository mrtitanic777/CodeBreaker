#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelCreeper.h"
#include "MathHelper.h"
#include "ModelBase.h"
#include "ModelRenderer.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelCreeper::ModelCreeper()
	: head(NULL)
	, unusedModel(NULL)
	, body(NULL)
	, leg1(NULL)
	, leg2(NULL)
	, leg3(NULL)
	, leg4(NULL)
{
	float var1 = 0.0F;
	unsigned char var2 = 4;
	this->head = new ModelRenderer(0, 0);
	this->head->addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1);
	this->head->setPosition(0.0F, (float)var2, 0.0F);
	this->unusedModel = new ModelRenderer(32, 0);
	this->unusedModel->addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1 + 0.5F);
	this->unusedModel->setPosition(0.0F, (float)var2, 0.0F);
	this->body = new ModelRenderer(16, 16);
	this->body->addBox(-4.0F, 0.0F, -2.0F, 8, 12, 4, var1);
	this->body->setPosition(0.0F, (float)var2, 0.0F);
	this->leg1 = new ModelRenderer(0, 16);
	this->leg1->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
	this->leg1->setPosition(-2.0F, (float)(12 + var2), 4.0F);
	this->leg2 = new ModelRenderer(0, 16);
	this->leg2->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
	this->leg2->setPosition(2.0F, (float)(12 + var2), 4.0F);
	this->leg3 = new ModelRenderer(0, 16);
	this->leg3->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
	this->leg3->setPosition(-2.0F, (float)(12 + var2), -4.0F);
	this->leg4 = new ModelRenderer(0, 16);
	this->leg4->addBox(-2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
	this->leg4->setPosition(2.0F, (float)(12 + var2), -4.0F);
}

ModelCreeper::~ModelCreeper()
{
	delete body;
	body = NULL;
	delete head;
	head = NULL;
	delete leg1;
	leg1 = NULL;
	delete leg2;
	leg2 = NULL;
	delete leg3;
	leg3 = NULL;
	delete leg4;
	leg4 = NULL;
	delete unusedModel;
	unusedModel = NULL;
}

void ModelCreeper::render(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->setRotationAngles(var1, var2, var3, var4, var5, var6);
	this->head->render(var6);
	this->body->render(var6);
	this->leg1->render(var6);
	this->leg2->render(var6);
	this->leg3->render(var6);
	this->leg4->render(var6);
}

void ModelCreeper::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->head->rotateAngleY = var4 / (180.0F / (float)3.1415927F);
	this->head->rotateAngleX = var5 / (180.0F / (float)3.1415927F);
	this->leg1->rotateAngleX = MathHelper::cos(var1 * 0.6662F) * 1.4F * var2;
	this->leg2->rotateAngleX = MathHelper::cos(var1 * 0.6662F + (float)3.1415927F) * 1.4F * var2;
	this->leg3->rotateAngleX = MathHelper::cos(var1 * 0.6662F + (float)3.1415927F) * 1.4F * var2;
	this->leg4->rotateAngleX = MathHelper::cos(var1 * 0.6662F) * 1.4F * var2;
}

void ModelCreeper::initStatics()
{
}
