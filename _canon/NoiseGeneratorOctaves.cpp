#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NoiseGeneratorOctaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGeneratorOctaves.h"
#include "NoiseGenerator.h"
#include "NoiseGeneratorPerlin.h"
#include "Random.h"

NoiseGeneratorOctaves::NoiseGeneratorOctaves(Random* var1, int var2)
	: octaves(0)
{
	this->octaves = var2;
	this->generatorCollection = JArray<NoiseGeneratorPerlin* >(var2);
	for(int var3 = 0; var3 < var2; ++var3) {
	this->generatorCollection[var3] = new NoiseGeneratorPerlin(var1);
	}
}

NoiseGeneratorOctaves::~NoiseGeneratorOctaves()
{
}

double NoiseGeneratorOctaves::generateNoise(double var1, double var3)
{
	double var5 = 0.0;
	double var7 = 1.0;
	for(int var9 = 0; var9 < this->octaves; ++var9) {
	var5 += this->generatorCollection[var9]->generateNoise2d(var1 * var7, var3 * var7) / var7;
	var7 /= 2.0;
	}
	return var5;
}

JArray<double > NoiseGeneratorOctaves::generateNoiseOctaves(JArray<double > var1, double var2, double var4, double var6, int var8, int var9, int var10, double var11, double var13, double var15)
{
	if(var1 == NULL) {
	var1 = JArray<double >(var8 * var9 * var10);
	} else {
	for(int var17 = 0; var17 < var1.length; ++var17) {
	var1[var17] = 0.0;
	}
	}
	double var20 = 1.0;
	for(int var19 = 0; var19 < this->octaves; ++var19) {
	if (this->generatorCollection[var19] != NULL) { this->generatorCollection[var19]->generateNoiseArray(var1, var2, var4, var6, var8, var9, var10, var11 * var20, var13 * var20, var15 * var20, var20); }
	var20 /= 2.0;
	}
	return var1;
}

JArray<double > NoiseGeneratorOctaves::generateNoiseOctaves2D(JArray<double > var1, int var2, int var3, int var4, int var5, double var6, double var8, double var10)
{
	return this->generateNoiseOctaves(var1, (double)var2, 10.0, (double)var3, var4, 1, var5, var6, 1.0, var8);
}

void NoiseGeneratorOctaves::initStatics()
{
}
