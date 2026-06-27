#ifndef NOISEGENERATOROCTAVES2_H
#define NOISEGENERATOROCTAVES2_H

// Auto-generated from NoiseGeneratorOctaves2.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NoiseGenerator.h"

class NoiseGenerator2;
class Random;

class NoiseGeneratorOctaves2 : public NoiseGenerator {
public:
	NoiseGeneratorOctaves2(Random* var1, int var2);
	virtual ~NoiseGeneratorOctaves2();

	JArray<NoiseGenerator2* > octaveGenerators;
	int octaveCount;

	virtual JArray<double > generateNoiseOctaves(JArray<double > var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12);
	virtual JArray<double > generateNoiseOctaves(JArray<double > var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12, double var14);

	static void initStatics();
};

#endif // NOISEGENERATOROCTAVES2_H