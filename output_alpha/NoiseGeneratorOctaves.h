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

	virtual double func_806_a(double var1, double var3);
	virtual double* func_807_a(double* var1, double var2, double var4, double var6, int var8, int var9, int var10, double var11, double var13, double var15);
	virtual double* func_4109_a(double* var1, int var2, int var3, int var4, int var5, double var6, double var8, double var10);

public: // was private
	NoiseGeneratorPerlin* m_generatorCollection;
	int m_field_1191_b;

public:  // array-length companions
	int m_generatorCollection_count;

};

#endif // NOISEGENERATOROCTAVES_H