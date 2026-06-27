#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ScaledResolution.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ScaledResolution.h"

ScaledResolution::ScaledResolution(int var1, int var2)
	: scaledWidth(0)
	, scaledHeight(0)
	, scaleFactor(0)
{
	this->scaledWidth = var1;
	this->scaledHeight = var2;
	for(this->scaleFactor = 1; this->scaledWidth / (this->scaleFactor + 1) >= 320 && this->scaledHeight / (this->scaleFactor + 1) >= 240; ++this->scaleFactor) {
	}
	this->scaledWidth /= this->scaleFactor;
	this->scaledHeight /= this->scaleFactor;
}

ScaledResolution::~ScaledResolution()
{
}

int ScaledResolution::getScaledWidth()
{
	return this->scaledWidth;
}

int ScaledResolution::getScaledHeight()
{
	return this->scaledHeight;
}

void ScaledResolution::initStatics()
{
}
