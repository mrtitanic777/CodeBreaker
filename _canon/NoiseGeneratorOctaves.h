#ifndef NOISEGENERATOROCTAVES_H
#define NOISEGENERATOROCTAVES_H

// Auto-generated from NoiseGeneratorOctaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NoiseGenerator.h"

class NoiseGeneratorPerlin;
class Random;

class NoiseGeneratorOctaves : public NoiseGenerator {
public:
	NoiseGeneratorOctaves(Random* var1, int var2);
	virtual ~NoiseGeneratorOctaves();

	JArray<NoiseGeneratorPerlin* > generatorCollection;
	int octaves;

	virtual double generateNoise(double var1, double var3);
	virtual JArray<double > generateNoiseOctaves(JArray<double > var1, double var2, double var4, double var6, int var8, int var9, int var10, double var11, double var13, double var15);
	virtual JArray<double > generateNoiseOctaves2D(JArray<double > var1, int var2, int var3, int var4, int var5, double var6, double var8, double var10);

	static void initStatics();
};

#endif // NOISEGENERATOROCTAVES_H