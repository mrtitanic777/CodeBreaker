#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Path.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Path.h"
#include "IllegalStateException.h"
#include "PathPoint.h"

Path::Path()
	: count(0)
{
	this->pathPoints = JArray<PathPoint* >(1024);
	this->count = 0;
}

Path::~Path()
{
}

PathPoint* Path::addPoint(PathPoint* var1)
{
	if(var1->index >= 0) {
	throw new IllegalStateException("OW KNOWS!");
	} else {
	if(this->count == this->pathPoints.length) {
	JArray<PathPoint* > var2 = JArray<PathPoint* >(this->count << 1);
	System::arraycopy(this->pathPoints, 0, var2, 0, this->count);
	this->pathPoints = var2;
	}
	this->pathPoints[this->count] = var1;
	var1->index = this->count;
	this->sortBack(this->count++);
	return var1;
	}
}

void Path::clearPath()
{
	this->count = 0;
}

PathPoint* Path::dequeue()
{
	PathPoint* var1 = this->pathPoints[0];
	this->pathPoints[0] = this->pathPoints[--this->count];
	this->pathPoints[this->count] = NULL;
	if(this->count > 0) {
	this->sortForward(0);
	}
	var1->index = -1;
	return var1;
}

void Path::changeDistance(PathPoint* var1, float var2)
{
	float var3 = var1->distanceToTarget;
	var1->distanceToTarget = var2;
	if(var2 < var3) {
	this->sortBack(var1->index);
	} else {
	this->sortForward(var1->index);
	}
}

void Path::sortBack(int var1)
{
	PathPoint* var2 = this->pathPoints[var1];
	int var4;
	for(float var3 = var2->distanceToTarget; var1 > 0; var1 = var4) {
	var4 = (var1 - 1) >> 1;
	PathPoint* var5 = this->pathPoints[var4];
	if(var3 >= var5->distanceToTarget) {
	break;
	}
	this->pathPoints[var1] = var5;
	var5->index = var1;
	}
	this->pathPoints[var1] = var2;
	var2->index = var1;
}

void Path::sortForward(int var1)
{
	PathPoint* var2 = this->pathPoints[var1];
	float var3 = var2->distanceToTarget;
	while(true) {
	int var4 = 1 + (var1 << 1);
	int var5 = var4 + 1;
	if(var4 >= this->count) {
	break;
	}
	PathPoint* var6 = this->pathPoints[var4];
	float var7 = var6->distanceToTarget;
	PathPoint* var8;
	float var9;
	if(var5 >= this->count) {
	var8 = NULL;
	var9 = Float::POSITIVE_INFINITY;
	} else {
	var8 = this->pathPoints[var5];
	var9 = var8->distanceToTarget;
	}
	if(var7 < var9) {
	if(var7 >= var3) {
	break;
	}
	this->pathPoints[var1] = var6;
	var6->index = var1;
	var1 = var4;
	} else {
	if(var9 >= var3) {
	break;
	}
	this->pathPoints[var1] = var8;
	var8->index = var1;
	var1 = var5;
	}
	}
	this->pathPoints[var1] = var2;
	var2->index = var1;
}

bool Path::isPathEmpty()
{
	return this->count == 0;
}

void Path::initStatics()
{
}
