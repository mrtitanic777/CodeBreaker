#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from PositionTexureVertex.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PositionTexureVertex.h"
#include "Vec3D.h"

PositionTexureVertex::PositionTexureVertex(float var1, float var2, float var3, float var4, float var5)
	: vector3D(NULL)
	, texturePositionX(0)
	, texturePositionY(0)
{
	// inlined this(Vec3D::createVectorHelper((double)var1, (double)var2, (double)var3), var4, var5) -- C++03 lacks ctor delegation
	this->vector3D = Vec3D::createVectorHelper((double)var1, (double)var2, (double)var3);
	this->texturePositionX = var4;
	this->texturePositionY = var5;
}

PositionTexureVertex::PositionTexureVertex(PositionTexureVertex* var1, float var2, float var3)
	: vector3D(NULL)
	, texturePositionX(0)
	, texturePositionY(0)
{
	this->vector3D = var1->vector3D;
	this->texturePositionX = var2;
	this->texturePositionY = var3;
}

PositionTexureVertex::PositionTexureVertex(Vec3D* var1, float var2, float var3)
	: vector3D(NULL)
	, texturePositionX(0)
	, texturePositionY(0)
{
	this->vector3D = var1;
	this->texturePositionX = var2;
	this->texturePositionY = var3;
}

PositionTexureVertex::~PositionTexureVertex()
{
}

PositionTexureVertex* PositionTexureVertex::setTexturePosition(float var1, float var2)
{
	return new PositionTexureVertex(this, var1, var2);
}
