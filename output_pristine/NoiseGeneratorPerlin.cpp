#include "stdafx.h"
// Auto-generated from NoiseGeneratorPerlin.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGeneratorPerlin.h"
#include "Common.h"
#include "Random.h"

NoiseGeneratorPerlin::NoiseGeneratorPerlin()
	: m_permutations(0)
{
  // PORT: constructor delegation  // was: this(new Random());
}

NoiseGeneratorPerlin::NoiseGeneratorPerlin(Random* var1)
	: m_permutations(0)
{
		m_permutations = new int[512];

		int var2;
		for(var2 = 0; var2 < 256; m_permutations[var2] = var2++) {
		}

		for(var2 = 0; var2 < 256; ++var2) {
			int var3 = var1->nextInt(256 - var2) + var2;
			int var4 = m_permutations[var2];
			m_permutations[var2] = m_permutations[var3];
			m_permutations[var3] = var4;
			m_permutations[var2 + 256] = m_permutations[var2];
		}

}

NoiseGeneratorPerlin::~NoiseGeneratorPerlin() {
	delete[] m_permutations;
	m_permutations = NULL;
}

double NoiseGeneratorPerlin::generateNoise(double var0) {
		return var0 * var0 * var0 * (var0 * (var0 * 6.0f - 15.0f) + 10.0f);
}

double NoiseGeneratorPerlin::lerp(double var0, double var2, double var4) {
		return var2 + var0 * (var4 - var2);
}

double NoiseGeneratorPerlin::grad(int var0, double var1, double var3, double var5) {
		var0 &= 15;
		double var8 = var0 < 8 ? var1 : var3;
		double var10 = var0 < 4 ? var3 : (var0 != 12 && var0 != 14 ? var5 : var1);
		return ((var0 & 1) == 0 ? var8 : -var8) + ((var0 & 2) == 0 ? var10 : -var10);
}

double NoiseGeneratorPerlin::generateNoise(double var1, double var3) {
		double var10 = 0.0f;
		double var8 = var3;
		int var2 = (int)floor(var1) & 255;
		int var21 = (int)floor(var3) & 255;
		int var4 = (int)floor(0.0f) & 255;
		double var6 = var1 - (double)(int)floor(var1);
		var8 -= (double)(int)floor(var8);
		var10 = 0.0f - (double)(int)floor(0.0f);
		double var15 = generateNoise(var6);
		double var17 = generateNoise(var8);
		double var19 = generateNoise(var10);
		int var5 = m_permutations[var2] + var21;
		int var12 = m_permutations[var5] + var4;
		var5 = m_permutations[var5 + 1] + var4;
		var2 = m_permutations[var2 + 1] + var21;
		var21 = m_permutations[var2] + var4;
		var2 = m_permutations[var2 + 1] + var4;
		return lerp(var19, lerp(var17, lerp(var15, grad(m_permutations[var12], var6, var8, var10), grad(m_permutations[var21], var6 - 1.0f, var8, var10)), lerp(var15, grad(m_permutations[var5], var6, var8 - 1.0f, var10), grad(m_permutations[var2], var6 - 1.0f, var8 - 1.0f, var10))), lerp(var17, lerp(var15, grad(m_permutations[var12 + 1], var6, var8, var10 - 1.0f), grad(m_permutations[var21 + 1], var6 - 1.0f, var8, var10 - 1.0f)), lerp(var15, grad(m_permutations[var5 + 1], var6, var8 - 1.0f, var10 - 1.0f), grad(m_permutations[var2 + 1], var6 - 1.0f, var8 - 1.0f, var10 - 1.0f))));
}
