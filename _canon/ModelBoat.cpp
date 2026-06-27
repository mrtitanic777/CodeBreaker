#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBoat.h"
#include "ModelBase.h"
#include "ModelRenderer.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelBoat::ModelBoat()
{
	this->boatSides = JArray<ModelRenderer* >(5);
	this->boatSides[0] = new ModelRenderer(0, 8);
	this->boatSides[1] = new ModelRenderer(0, 0);
	this->boatSides[2] = new ModelRenderer(0, 0);
	this->boatSides[3] = new ModelRenderer(0, 0);
	this->boatSides[4] = new ModelRenderer(0, 0);
	unsigned char var1 = 24;
	unsigned char var2 = 6;
	unsigned char var3 = 20;
	unsigned char var4 = 4;
	if (this->boatSides[0] != NULL) { this->boatSides[0]->addBox((float)(-var1 / 2), (float)(-var3 / 2 + 2), -3.0F, var1, var3 - 4, 4, 0.0F); }
	if (this->boatSides[0] != NULL) { this->boatSides[0]->setPosition(0.0F, (float)(0 + var4), 0.0F); }
	if (this->boatSides[1] != NULL) { this->boatSides[1]->addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F); }
	if (this->boatSides[1] != NULL) { this->boatSides[1]->setPosition((float)(-var1 / 2 + 1), (float)(0 + var4), 0.0F); }
	if (this->boatSides[2] != NULL) { this->boatSides[2]->addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F); }
	if (this->boatSides[2] != NULL) { this->boatSides[2]->setPosition((float)(var1 / 2 - 1), (float)(0 + var4), 0.0F); }
	if (this->boatSides[3] != NULL) { this->boatSides[3]->addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F); }
	if (this->boatSides[3] != NULL) { this->boatSides[3]->setPosition(0.0F, (float)(0 + var4), (float)(-var3 / 2 + 1)); }
	if (this->boatSides[4] != NULL) { this->boatSides[4]->addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F); }
	if (this->boatSides[4] != NULL) { this->boatSides[4]->setPosition(0.0F, (float)(0 + var4), (float)(var3 / 2 - 1)); }
	this->boatSides[0]->rotateAngleX = (float)3.1415927F * 0.5F;
	this->boatSides[1]->rotateAngleY = (float)3.1415927F * 3.0F / 2.0F;
	this->boatSides[2]->rotateAngleY = (float)3.1415927F * 0.5F;
	this->boatSides[3]->rotateAngleY = (float)3.1415927F;
}

ModelBoat::~ModelBoat()
{
}

void ModelBoat::render(float var1, float var2, float var3, float var4, float var5, float var6)
{
	for(int var7 = 0; var7 < 5; ++var7) {
	if (this->boatSides[var7] != NULL) { this->boatSides[var7]->render(var6); }
	}
}

void ModelBoat::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6)
{
}

void ModelBoat::initStatics()
{
}
