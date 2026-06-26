#ifndef NOISEGENERATOROCTAVES_H
#define NOISEGENERATOROCTAVES_H

// Auto-generated from NoiseGeneratorOctaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGenerator.h"

class NoiseGeneratorPerlin;
class Random;

class NoiseGeneratorOctaves : public NoiseGenerator {
public:
	NoiseGeneratorOctaves();
	NoiseGeneratorOctaves(Random* var1, int var2);
	virtual ~NoiseGeneratorOctaves();

	virtual double generateNoise(double var1, double var3);

private:
	NoiseGeneratorPerlin* m_generatorCollection;
	int m_octaves;

};

#endif // NOISEGENERATOROCTAVES_H