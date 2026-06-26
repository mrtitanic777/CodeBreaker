#include "stdafx.h"
// Auto-generated from MathHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MathHelper.h"
#include "Common.h"

float* MathHelper::s_SIN_TABLE = new float[65536];

MathHelper::MathHelper()
{
}

MathHelper::~MathHelper() {
	// No dynamic resources
}

float MathHelper::sin(float var0) {
		return s_SIN_TABLE[(int)(var0 * 10430.378F) & '\uffff'];
}

float MathHelper::cos(float var0) {
		return s_SIN_TABLE[(int)(var0 * 10430.378F + 16384.0F) & '\uffff'];
}

float MathHelper::sqrt_float(float var0) {
		return sqrtf(var0);
}

int MathHelper::floor_float(float var0) {
		int var1 = (int)var0;
		return var0 < (float)var1 ? var1 - 1 : var1;
}

int MathHelper::floor_double(double var0) {
		int var2 = (int)var0;
		return var0 < (double)var2 ? var2 - 1 : var2;
}

float MathHelper::abs(float var0) {
		return var0 >= 0.0F ? var0 : -var0;
}
