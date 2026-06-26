#ifndef NOISEGENERATOROCTAVES2_H
#define NOISEGENERATOROCTAVES2_H

// Auto-generated from NoiseGeneratorOctaves2.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGenerator.h"

class NoiseGenerator2;
class Random;

class NoiseGeneratorOctaves2 : public NoiseGenerator {
public:
	NoiseGeneratorOctaves2();
	NoiseGeneratorOctaves2(Random* var1, int var2);
	virtual ~NoiseGeneratorOctaves2();

	virtual double* func_4112_a(double* var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12);
	virtual double* func_4111_a(double* var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12, double var14);

public: // was private
	NoiseGenerator2* m_field_4234_a;
	int m_field_4233_b;

public:  // array-length companions
	int m_field_4234_a_count;

};

#endif // NOISEGENERATOROCTAVES2_H