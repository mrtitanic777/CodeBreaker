#ifndef NOISEGENERATOR2_H
#define NOISEGENERATOR2_H

// Auto-generated from NoiseGenerator2.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Random;

class NoiseGenerator2 {
public:
	NoiseGenerator2();
	NoiseGenerator2(Random* var1);
	virtual ~NoiseGenerator2();

	JArray<int > permutations;
	double xOffset;
	double yOffset;
	double zOffset;
	static JArray<JArray<int > > s_grad;
	static double s_F2;
	static double s_G2;

	static int fastFloor(double var0);
	static double dot(JArray<int > var0, double var1, double var3);
	virtual void generateNoise(JArray<double > var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12);
};

#endif // NOISEGENERATOR2_H