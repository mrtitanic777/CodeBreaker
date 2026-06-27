#ifndef NOISEGENERATORPERLIN_H
#define NOISEGENERATORPERLIN_H

// Auto-generated from NoiseGeneratorPerlin.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NoiseGenerator.h"

class Random;

class NoiseGeneratorPerlin : public NoiseGenerator {
public:
	NoiseGeneratorPerlin();
	NoiseGeneratorPerlin(Random* var1);
	virtual ~NoiseGeneratorPerlin();

	JArray<int > permutations;
	double xCoord;
	double yCoord;
	double zCoord;

	virtual double generateNoise(double var1, double var3, double var5);
	virtual double lerp(double var1, double var3, double var5);
	virtual double grad2d(int var1, double var2, double var4);
	virtual double grad(int var1, double var2, double var4, double var6);
	virtual double generateNoise2d(double var1, double var3);
	virtual void generateNoiseArray(JArray<double > var1, double var2, double var4, double var6, int var8, int var9, int var10, double var11, double var13, double var15, double var17);
};

#endif // NOISEGENERATORPERLIN_H