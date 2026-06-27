#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelMinecart.h"
#include "ModelBase.h"
#include "ModelRenderer.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelMinecart::ModelMinecart()
{
	this->sideModels = JArray<ModelRenderer* >(7);
	this->sideModels[0] = new ModelRenderer(0, 10);
	this->sideModels[1] = new ModelRenderer(0, 0);
	this->sideModels[2] = new ModelRenderer(0, 0);
	this->sideModels[3] = new ModelRenderer(0, 0);
	this->sideModels[4] = new ModelRenderer(0, 0);
	this->sideModels[5] = new ModelRenderer(44, 10);
	unsigned char var1 = 20;
	unsigned char var2 = 8;
	unsigned char var3 = 16;
	unsigned char var4 = 4;
	if (this->sideModels[0] != NULL) { this->sideModels[0]->addBox((float)(-var1 / 2), (float)(-var3 / 2), -1.0F, var1, var3, 2, 0.0F); }
	if (this->sideModels[0] != NULL) { this->sideModels[0]->setPosition(0.0F, (float)(0 + var4), 0.0F); }
	if (this->sideModels[5] != NULL) { this->sideModels[5]->addBox((float)(-var1 / 2 + 1), (float)(-var3 / 2 + 1), -1.0F, var1 - 2, var3 - 2, 1, 0.0F); }
	if (this->sideModels[5] != NULL) { this->sideModels[5]->setPosition(0.0F, (float)(0 + var4), 0.0F); }
	if (this->sideModels[1] != NULL) { this->sideModels[1]->addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F); }
	if (this->sideModels[1] != NULL) { this->sideModels[1]->setPosition((float)(-var1 / 2 + 1), (float)(0 + var4), 0.0F); }
	if (this->sideModels[2] != NULL) { this->sideModels[2]->addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F); }
	if (this->sideModels[2] != NULL) { this->sideModels[2]->setPosition((float)(var1 / 2 - 1), (float)(0 + var4), 0.0F); }
	if (this->sideModels[3] != NULL) { this->sideModels[3]->addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F); }
	if (this->sideModels[3] != NULL) { this->sideModels[3]->setPosition(0.0F, (float)(0 + var4), (float)(-var3 / 2 + 1)); }
	if (this->sideModels[4] != NULL) { this->sideModels[4]->addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F); }
	if (this->sideModels[4] != NULL) { this->sideModels[4]->setPosition(0.0F, (float)(0 + var4), (float)(var3 / 2 - 1)); }
	this->sideModels[0]->rotateAngleX = (float)3.1415927F * 0.5F;
	this->sideModels[1]->rotateAngleY = (float)3.1415927F * 3.0F / 2.0F;
	this->sideModels[2]->rotateAngleY = (float)3.1415927F * 0.5F;
	this->sideModels[3]->rotateAngleY = (float)3.1415927F;
	this->sideModels[5]->rotateAngleX = (float)3.1415927F * -0.5F;
}

ModelMinecart::~ModelMinecart()
{
}

void ModelMinecart::render(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->sideModels[5]->offsetY = 4.0F - var3;
	for(int var7 = 0; var7 < 6; ++var7) {
	if (this->sideModels[var7] != NULL) { this->sideModels[var7]->render(var6); }
	}
}

void ModelMinecart::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6)
{
}

void ModelMinecart::initStatics()
{
}
