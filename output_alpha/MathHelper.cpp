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
		return s_SIN_TABLE[(int)(var0 * 10430.378F) & 0xFFFF];
}

float MathHelper::cos(float var0) {
		return s_SIN_TABLE[(int)(var0 * 10430.378F + 16384.0F) & 0xFFFF];
}

float MathHelper::sqrt_float(float var0) {
		return sqrtf(var0);
}

float MathHelper::sqrt_double(double var0) {
		return (float)sqrtf(var0);
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

double MathHelper::abs_max(double var0, double var2) {
		if(var0 < 0.0f) {
			var0 = -var0;
		}

		if(var2 < 0.0f) {
			var2 = -var2;
		}

		return var0 > var2 ? var0 : var2;
}

int MathHelper::bucketInt(int var0, int var1) {
		return var0 < 0 ? -((-var0 - 1) / var1) - 1 : var0 / var1;
}

void MathHelper::initStatics() {
		for(int var0 = 0; var0 < 65536; ++var0) {
			s_SIN_TABLE[var0] = (float)sinf((double)var0 * 3.14159265358979323846f * 2.0f / 65536.0f);
		}

}
