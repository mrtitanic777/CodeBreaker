#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NoiseGeneratorOctaves2.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGeneratorOctaves2.h"
#include "NoiseGenerator.h"
#include "NoiseGenerator2.h"
#include "Random.h"

NoiseGeneratorOctaves2::NoiseGeneratorOctaves2(Random* var1, int var2)
	: octaveCount(0)
{
	this->octaveCount = var2;
	this->octaveGenerators = JArray<NoiseGenerator2* >(var2);
	for(int var3 = 0; var3 < var2; ++var3) {
	this->octaveGenerators[var3] = new NoiseGenerator2(var1);
	}
}

NoiseGeneratorOctaves2::~NoiseGeneratorOctaves2()
{
}

JArray<double > NoiseGeneratorOctaves2::generateNoiseOctaves(JArray<double > var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12)
{
	return this->generateNoiseOctaves(var1, var2, var4, var6, var7, var8, var10, var12, 0.5);
}

JArray<double > NoiseGeneratorOctaves2::generateNoiseOctaves(JArray<double > var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12, double var14)
{
	var8 /= 1.5;
	var10 /= 1.5;
	if(var1 != NULL && var1.length >= var6 * var7) {
	for(int var16 = 0; var16 < var1.length; ++var16) {
	var1[var16] = 0.0;
	}
	} else {
	var1 = JArray<double >(var6 * var7);
	}
	double var21 = 1.0;
	double var18 = 1.0;
	for(int var20 = 0; var20 < this->octaveCount; ++var20) {
	if (this->octaveGenerators[var20] != NULL) { this->octaveGenerators[var20]->generateNoise(var1, var2, var4, var6, var7, var8 * var18, var10 * var18, 0.55 / var21); }
	var18 *= var12;
	var21 *= var14;
	}
	return var1;
}

void NoiseGeneratorOctaves2::initStatics()
{
}
