#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ClippingHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ClippingHelper.h"

ClippingHelper::ClippingHelper()
{
	this->frustum = j2x_newarr2<float >(16, 16);
	this->projectionMatrix = JArray<float >(16);
	this->modelviewMatrix = JArray<float >(16);
	this->clippingMatrix = JArray<float >(16);
}

ClippingHelper::~ClippingHelper()
{
}

bool ClippingHelper::isBoxInFrustum(double var1, double var3, double var5, double var7, double var9, double var11)
{
	for(int var13 = 0; var13 < 6; ++var13) {
	if((double)this->frustum[var13][0] * var1 + (double)this->frustum[var13][1] * var3 + (double)this->frustum[var13][2] * var5 + (double)this->frustum[var13][3] <= 0.0 && (double)this->frustum[var13][0] * var7 + (double)this->frustum[var13][1] * var3 + (double)this->frustum[var13][2] * var5 + (double)this->frustum[var13][3] <= 0.0 && (double)this->frustum[var13][0] * var1 + (double)this->frustum[var13][1] * var9 + (double)this->frustum[var13][2] * var5 + (double)this->frustum[var13][3] <= 0.0 && (double)this->frustum[var13][0] * var7 + (double)this->frustum[var13][1] * var9 + (double)this->frustum[var13][2] * var5 + (double)this->frustum[var13][3] <= 0.0 && (double)this->frustum[var13][0] * var1 + (double)this->frustum[var13][1] * var3 + (double)this->frustum[var13][2] * var11 + (double)this->frustum[var13][3] <= 0.0 && (double)this->frustum[var13][0] * var7 + (double)this->frustum[var13][1] * var3 + (double)this->frustum[var13][2] * var11 + (double)this->frustum[var13][3] <= 0.0 && (double)this->frustum[var13][0] * var1 + (double)this->frustum[var13][1] * var9 + (double)this->frustum[var13][2] * var11 + (double)this->frustum[var13][3] <= 0.0 && (double)this->frustum[var13][0] * var7 + (double)this->frustum[var13][1] * var9 + (double)this->frustum[var13][2] * var11 + (double)this->frustum[var13][3] <= 0.0) {
	return false;
	}
	}
	return true;
}

void ClippingHelper::initStatics()
{
}
