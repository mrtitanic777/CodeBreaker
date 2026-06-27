#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TexturedQuad.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexturedQuad.h"
#include "ByteBuffer.h"
#include "FloatBuffer.h"
#include "IntBuffer.h"
#include "PositionTexureVertex.h"
#include "Tessellator.h"
#include "Vec3D.h"

TexturedQuad::TexturedQuad(JArray<PositionTexureVertex* > var1)
	: nVertices(0)
	, invertNormal(0)
{
	this->nVertices = 0;
	this->invertNormal = false;
	this->vertexPositions = var1;
	this->nVertices = var1.length;
}

TexturedQuad::TexturedQuad(JArray<PositionTexureVertex* > var1, int var2, int var3, int var4, int var5)
	: nVertices(0)
	, invertNormal(0)
{
	// inlined this(var1) -- C++03 lacks ctor delegation
	this->nVertices = 0;
	this->invertNormal = false;
	this->vertexPositions = var1;
	this->nVertices = var1.length;
	float var6 = 0.0015625F;
	float var7 = 0.003125F;
	var1[0] = var1[0]->setTexturePosition((float)var4 / 64.0F - var6, (float)var3 / 32.0F + var7);
	var1[1] = var1[1]->setTexturePosition((float)var2 / 64.0F + var6, (float)var3 / 32.0F + var7);
	var1[2] = var1[2]->setTexturePosition((float)var2 / 64.0F + var6, (float)var5 / 32.0F - var7);
	var1[3] = var1[3]->setTexturePosition((float)var4 / 64.0F - var6, (float)var5 / 32.0F - var7);
}

TexturedQuad::~TexturedQuad()
{
}

void TexturedQuad::flipFace()
{
	JArray<PositionTexureVertex* > var1 = JArray<PositionTexureVertex* >(this->vertexPositions.length);
	for(int var2 = 0; var2 < this->vertexPositions.length; ++var2) {
	var1[var2] = this->vertexPositions[this->vertexPositions.length - var2 - 1];
	}
	this->vertexPositions = var1;
}

void TexturedQuad::draw(Tessellator* var1, float var2)
{
	Vec3D* var3 = this->vertexPositions[1]->vector3D->subtract(this->vertexPositions[0]->vector3D);
	Vec3D* var4 = this->vertexPositions[1]->vector3D->subtract(this->vertexPositions[2]->vector3D);
	Vec3D* var5 = var4->crossProduct(var3)->normalize();
	var1->startDrawingQuads();
	if(this->invertNormal) {
	var1->setNormal(-((float)var5->xCoord), -((float)var5->yCoord), -((float)var5->zCoord));
	} else {
	var1->setNormal((float)var5->xCoord, (float)var5->yCoord, (float)var5->zCoord);
	}
	for(int var6 = 0; var6 < 4; ++var6) {
	PositionTexureVertex* var7 = this->vertexPositions[var6];
	var1->addVertexWithUV((double)((float)var7->vector3D->xCoord * var2), (double)((float)var7->vector3D->yCoord * var2), (double)((float)var7->vector3D->zCoord * var2), (double)var7->texturePositionX, (double)var7->texturePositionY);
	}
	var1->draw();
}

void TexturedQuad::initStatics()
{
}
