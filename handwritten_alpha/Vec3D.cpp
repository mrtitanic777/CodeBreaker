#include "stdafx.h"
// Auto-generated from Vec3D.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Vec3D.h"
#include "Common.h"
#include <string>
#include <math.h>

std::vector<void*> /* elements are Vec3D* (heap, via createVectorHelper) */ Vec3D::s_vectorList;
int Vec3D::s_nextVector = 0;

Vec3D::Vec3D()
	: xCoord(0)
	, yCoord(0)
	, zCoord(0)
{
}

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

		xCoord = var1;
		yCoord = var3;
		zCoord = var5;
}

Vec3D::~Vec3D() {
	// No dynamic resources
}

Vec3D* Vec3D::createVectorHelper(double var0, double var2, double var4) {
		return new Vec3D(var0, var2, var4);
}

void Vec3D::initialize() {
		s_nextVector = 0;
}

Vec3D* Vec3D::createVector(double var0, double var2, double var4) {
		if(s_nextVector >= (int)s_vectorList.size()) {
			s_vectorList.push_back(createVectorHelper(0.0, 0.0, 0.0));
		}

		return ((Vec3D*)s_vectorList[s_nextVector++])->setComponents(var0, var2, var4);
}

Vec3D* Vec3D::setComponents(double var1, double var3, double var5) {
		xCoord = var1;
		yCoord = var3;
		zCoord = var5;
		return this;
}

Vec3D* Vec3D::func_1262_a(Vec3D* var1) {
		return createVector(var1->xCoord - xCoord, var1->yCoord - yCoord, var1->zCoord - zCoord);
}

Vec3D* Vec3D::normalize() {
		double var1 = (double)sqrt(xCoord * xCoord + yCoord * yCoord + zCoord * zCoord);
		return var1 < 1.0E-4 ? createVector(0.0, 0.0, 0.0) : createVector(xCoord / var1, yCoord / var1, zCoord / var1);
}

Vec3D* Vec3D::crossProduct(Vec3D* var1) {
		return createVector(yCoord * var1->zCoord - zCoord * var1->yCoord, zCoord * var1->xCoord - xCoord * var1->zCoord, xCoord * var1->yCoord - yCoord * var1->xCoord);
}

Vec3D* Vec3D::addVector(double var1, double var3, double var5) {
		return createVector(xCoord + var1, yCoord + var3, zCoord + var5);
}

double Vec3D::distanceTo(Vec3D* var1) {
		double var2 = var1->xCoord - xCoord;
		double var4 = var1->yCoord - yCoord;
		double var6 = var1->zCoord - zCoord;
		return (double)sqrt(var2 * var2 + var4 * var4 + var6 * var6);
}

double Vec3D::squareDistanceTo(Vec3D* var1) {
		double var2 = var1->xCoord - xCoord;
		double var4 = var1->yCoord - yCoord;
		double var6 = var1->zCoord - zCoord;
		return var2 * var2 + var4 * var4 + var6 * var6;
}

double Vec3D::squareDistanceTo(double var1, double var3, double var5) {
		double var7 = var1 - xCoord;
		double var9 = var3 - yCoord;
		double var11 = var5 - zCoord;
		return var7 * var7 + var9 * var9 + var11 * var11;
}

double Vec3D::lengthVector() {
		return (double)sqrt(xCoord * xCoord + yCoord * yCoord + zCoord * zCoord);
}

Vec3D* Vec3D::getIntermediateWithXValue(Vec3D* var1, double var2) {
		double var4 = var1->xCoord - xCoord;
		double var6 = var1->yCoord - yCoord;
		double var8 = var1->zCoord - zCoord;
		if(var4 * var4 < (double)1.0E-7F) {
			return NULL;
		} else {
			double var10 = (var2 - xCoord) / var4;
			return var10 >= 0.0 && var10 <= 1.0 ? createVector(xCoord + var4 * var10, yCoord + var6 * var10, zCoord + var8 * var10) : NULL;
		}
}

Vec3D* Vec3D::getIntermediateWithYValue(Vec3D* var1, double var2) {
		double var4 = var1->xCoord - xCoord;
		double var6 = var1->yCoord - yCoord;
		double var8 = var1->zCoord - zCoord;
		if(var6 * var6 < (double)1.0E-7F) {
			return NULL;
		} else {
			double var10 = (var2 - yCoord) / var6;
			return var10 >= 0.0 && var10 <= 1.0 ? createVector(xCoord + var4 * var10, yCoord + var6 * var10, zCoord + var8 * var10) : NULL;
		}
}

Vec3D* Vec3D::getIntermediateWithZValue(Vec3D* var1, double var2) {
		double var4 = var1->xCoord - xCoord;
		double var6 = var1->yCoord - yCoord;
		double var8 = var1->zCoord - zCoord;
		if(var8 * var8 < (double)1.0E-7F) {
			return NULL;
		} else {
			double var10 = (var2 - zCoord) / var8;
			return var10 >= 0.0 && var10 <= 1.0 ? createVector(xCoord + var4 * var10, yCoord + var6 * var10, zCoord + var8 * var10) : NULL;
		}
}

std::string Vec3D::toString() {
		// PORT: Java string concat. Return a reference into a static buffer so the
		// declared `const std::string&` return type stays valid after return.
		static std::string s_str;
		s_str = "(" + toStr(xCoord) + ", " + toStr(yCoord) + ", " + toStr(zCoord) + ")";
		return s_str;
}

void Vec3D::rotateAroundX(float var1) {
		float var2 = cosf(var1);
		float var3 = sinf(var1);
		double var4 = xCoord;
		double var6 = yCoord * (double)var2 + zCoord * (double)var3;
		double var8 = zCoord * (double)var2 - yCoord * (double)var3;
		xCoord = var4;
		yCoord = var6;
		zCoord = var8;
}

void Vec3D::rotateAroundY(float var1) {
		float var2 = cosf(var1);
		float var3 = sinf(var1);
		double var4 = xCoord * (double)var2 + zCoord * (double)var3;
		double var6 = yCoord;
		double var8 = zCoord * (double)var2 - xCoord * (double)var3;
		xCoord = var4;
		yCoord = var6;
		zCoord = var8;
}
