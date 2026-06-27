#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Vec3D.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Vec3D.h"
#include "MathHelper.h"

JList<void*> Vec3D::s_vectorList;
int Vec3D::s_nextVector = 0;

Vec3D::Vec3D(double var1, double var3, double var5)
	: xCoord(0)
	, yCoord(0)
	, zCoord(0)
{
	if(var1 == -0.0) {
	var1 = 0.0;
	}
	if(var3 == -0.0) {
	var3 = 0.0;
	}
	if(var5 == -0.0) {
	var5 = 0.0;
	}
	this->xCoord = var1;
	this->yCoord = var3;
	this->zCoord = var5;
}

Vec3D::~Vec3D()
{
}

Vec3D* Vec3D::createVectorHelper(double var0, double var2, double var4)
{
	return new Vec3D(var0, var2, var4);
}

void Vec3D::initialize()
{
	s_nextVector = 0;
}

Vec3D* Vec3D::createVector(double var0, double var2, double var4)
{
	if(s_nextVector >= s_vectorList.size()) {
	s_vectorList.push_back(createVectorHelper(0.0, 0.0, 0.0));
	}
	return ((Vec3D*)s_vectorList[s_nextVector++])->setComponents(var0, var2, var4);
}

Vec3D* Vec3D::setComponents(double var1, double var3, double var5)
{
	this->xCoord = var1;
	this->yCoord = var3;
	this->zCoord = var5;
	return this;
}

Vec3D* Vec3D::subtract(Vec3D* var1)
{
	return createVector(var1->xCoord - this->xCoord, var1->yCoord - this->yCoord, var1->zCoord - this->zCoord);
}

Vec3D* Vec3D::normalize()
{
	double var1 = (double)MathHelper::sqrt_double(this->xCoord * this->xCoord + this->yCoord * this->yCoord + this->zCoord * this->zCoord);
	return var1 < 1.0E-4 ? createVector(0.0, 0.0, 0.0) : createVector(this->xCoord / var1, this->yCoord / var1, this->zCoord / var1);
}

Vec3D* Vec3D::crossProduct(Vec3D* var1)
{
	return createVector(this->yCoord * var1->zCoord - this->zCoord * var1->yCoord, this->zCoord * var1->xCoord - this->xCoord * var1->zCoord, this->xCoord * var1->yCoord - this->yCoord * var1->xCoord);
}

Vec3D* Vec3D::addVector(double var1, double var3, double var5)
{
	return createVector(this->xCoord + var1, this->yCoord + var3, this->zCoord + var5);
}

double Vec3D::distanceTo(Vec3D* var1)
{
	double var2 = var1->xCoord - this->xCoord;
	double var4 = var1->yCoord - this->yCoord;
	double var6 = var1->zCoord - this->zCoord;
	return (double)MathHelper::sqrt_double(var2 * var2 + var4 * var4 + var6 * var6);
}

double Vec3D::squareDistanceTo(Vec3D* var1)
{
	double var2 = var1->xCoord - this->xCoord;
	double var4 = var1->yCoord - this->yCoord;
	double var6 = var1->zCoord - this->zCoord;
	return var2 * var2 + var4 * var4 + var6 * var6;
}

double Vec3D::squareDistanceTo(double var1, double var3, double var5)
{
	double var7 = var1 - this->xCoord;
	double var9 = var3 - this->yCoord;
	double var11 = var5 - this->zCoord;
	return var7 * var7 + var9 * var9 + var11 * var11;
}

double Vec3D::lengthVector()
{
	return (double)MathHelper::sqrt_double(this->xCoord * this->xCoord + this->yCoord * this->yCoord + this->zCoord * this->zCoord);
}

Vec3D* Vec3D::getIntermediateWithXValue(Vec3D* var1, double var2)
{
	double var4 = var1->xCoord - this->xCoord;
	double var6 = var1->yCoord - this->yCoord;
	double var8 = var1->zCoord - this->zCoord;
	if(var4 * var4 < (double)1.0E-7F) {
	return NULL;
	} else {
	double var10 = (var2 - this->xCoord) / var4;
	return var10 >= 0.0 && var10 <= 1.0 ? createVector(this->xCoord + var4 * var10, this->yCoord + var6 * var10, this->zCoord + var8 * var10) : NULL;
	}
}

Vec3D* Vec3D::getIntermediateWithYValue(Vec3D* var1, double var2)
{
	double var4 = var1->xCoord - this->xCoord;
	double var6 = var1->yCoord - this->yCoord;
	double var8 = var1->zCoord - this->zCoord;
	if(var6 * var6 < (double)1.0E-7F) {
	return NULL;
	} else {
	double var10 = (var2 - this->yCoord) / var6;
	return var10 >= 0.0 && var10 <= 1.0 ? createVector(this->xCoord + var4 * var10, this->yCoord + var6 * var10, this->zCoord + var8 * var10) : NULL;
	}
}

Vec3D* Vec3D::getIntermediateWithZValue(Vec3D* var1, double var2)
{
	double var4 = var1->xCoord - this->xCoord;
	double var6 = var1->yCoord - this->yCoord;
	double var8 = var1->zCoord - this->zCoord;
	if(var8 * var8 < (double)1.0E-7F) {
	return NULL;
	} else {
	double var10 = (var2 - this->zCoord) / var8;
	return var10 >= 0.0 && var10 <= 1.0 ? createVector(this->xCoord + var4 * var10, this->yCoord + var6 * var10, this->zCoord + var8 * var10) : NULL;
	}
}

JavaString Vec3D::toString()
{
	return (JCat() + "(" + this->xCoord + ", " + this->yCoord + ", " + this->zCoord + ")");
}

void Vec3D::rotateAroundX(float var1)
{
	float var2 = MathHelper::cos(var1);
	float var3 = MathHelper::sin(var1);
	double var4 = this->xCoord;
	double var6 = this->yCoord * (double)var2 + this->zCoord * (double)var3;
	double var8 = this->zCoord * (double)var2 - this->yCoord * (double)var3;
	this->xCoord = var4;
	this->yCoord = var6;
	this->zCoord = var8;
}

void Vec3D::rotateAroundY(float var1)
{
	float var2 = MathHelper::cos(var1);
	float var3 = MathHelper::sin(var1);
	double var4 = this->xCoord * (double)var2 + this->zCoord * (double)var3;
	double var6 = this->yCoord;
	double var8 = this->zCoord * (double)var2 - this->xCoord * (double)var3;
	this->xCoord = var4;
	this->yCoord = var6;
	this->zCoord = var8;
}

void Vec3D::initStatics()
{
}
