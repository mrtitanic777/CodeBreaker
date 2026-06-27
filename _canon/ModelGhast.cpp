#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelGhast.h"
#include "MathHelper.h"
#include "ModelBase.h"
#include "ModelRenderer.h"
#include "PositionTexureVertex.h"
#include "Random.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelGhast::ModelGhast()
	: body(NULL)
{
	this->tentacles = JArray<ModelRenderer* >(9);
	unsigned char var1 = -16;
	this->body = new ModelRenderer(0, 0);
	this->body->addBox(-8.0F, -8.0F, -8.0F, 16, 16, 16);
	this->body->offsetY += (float)(24 + var1);
	Random* var2 = new Random(1660LL);
	for(int var3 = 0; var3 < this->tentacles.length; ++var3) {
	this->tentacles[var3] = new ModelRenderer(0, 0);
	float var4 = (((float)(var3 % 3) - (float)(var3 / 3 % 2) * 0.5F + 0.25F) / 2.0F * 2.0F - 1.0F) * 5.0F;
	float var5 = ((float)(var3 / 3) / 2.0F * 2.0F - 1.0F) * 5.0F;
	int var6 = var2->nextInt(7) + 8;
	if (this->tentacles[var3] != NULL) { this->tentacles[var3]->addBox(-1.0F, 0.0F, -1.0F, 2, var6, 2); }
	this->tentacles[var3]->offsetX = var4;
	this->tentacles[var3]->offsetZ = var5;
	this->tentacles[var3]->offsetY = (float)(31 + var1);
	}
}

ModelGhast::~ModelGhast()
{
	delete body;
	body = NULL;
}

void ModelGhast::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6)
{
	for(int var7 = 0; var7 < this->tentacles.length; ++var7) {
	this->tentacles[var7]->rotateAngleX = 0.2F * MathHelper::sin(var3 * 0.3F + (float)var7) + 0.4F;
	}
}

void ModelGhast::render(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->setRotationAngles(var1, var2, var3, var4, var5, var6);
	this->body->render(var6);
	for(int var7 = 0; var7 < this->tentacles.length; ++var7) {
	if (this->tentacles[var7] != NULL) { this->tentacles[var7]->render(var6); }
	}
}

void ModelGhast::initStatics()
{
}
