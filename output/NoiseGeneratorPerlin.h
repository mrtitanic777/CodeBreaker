#ifndef NOISEGENERATORPERLIN_H
#define NOISEGENERATORPERLIN_H

// Auto-generated from NoiseGeneratorPerlin.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGenerator.h"

class Random;

class NoiseGeneratorPerlin : public NoiseGenerator {
public:
	NoiseGeneratorPerlin();
	NoiseGeneratorPerlin(Random* var1);
	virtual ~NoiseGeneratorPerlin();

	virtual double generateNoise(double var1, double var3);

public: // was private
	int* m_permutations;

	double generateNoise(double var0);
	double lerp(double var0, double var2, double var4);
	double grad(int var0, double var1, double var3, double var5);

public:  // array-length companions
	int m_permutations_count;

};

#endif // NOISEGENERATORPERLIN_H