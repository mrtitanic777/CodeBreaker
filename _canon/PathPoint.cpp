#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from PathPoint.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PathPoint.h"
#include "MathHelper.h"

PathPoint::PathPoint(int var1, int var2, int var3)
	: xCoord(0)
	, yCoord(0)
	, zCoord(0)
	, hash(0)
	, index(0)
	, totalPathDistance(0)
	, distanceToNext(0)
	, distanceToTarget(0)
	, previous(NULL)
	, isFirst(0)
{
	this->index = -1;
	this->isFirst = false;
	this->xCoord = var1;
	this->yCoord = var2;
	this->zCoord = var3;
	this->hash = var1 | (var2 << 10) | (var3 << 20);
}

PathPoint::~PathPoint()
{
}

float PathPoint::distanceTo(PathPoint* var1)
{
	float var2 = (float)(var1->xCoord - this->xCoord);
	float var3 = (float)(var1->yCoord - this->yCoord);
	float var4 = (float)(var1->zCoord - this->zCoord);
	return MathHelper::sqrt_float(var2 * var2 + var3 * var3 + var4 * var4);
}

bool PathPoint::equals(JObject var1)
{
	return ((PathPoint*)var1)->hash == this->hash;
}

int PathPoint::hashCode()
{
	return this->hash;
}

bool PathPoint::isAssigned()
{
	return this->index >= 0;
}

JavaString PathPoint::toString()
{
	return (JCat() + this->xCoord + ", " + this->yCoord + ", " + this->zCoord);
}

void PathPoint::initStatics()
{
}
