#ifndef NOISEGENERATORDISTORT_H
#define NOISEGENERATORDISTORT_H

// Auto-generated from NoiseGeneratorDistort.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGenerator.h"

class NoiseGeneratorDistort : public NoiseGenerator {
public:
	NoiseGeneratorDistort();
	NoiseGeneratorDistort(NoiseGenerator* var1, NoiseGenerator* var2);
	virtual ~NoiseGeneratorDistort();

	virtual double generateNoise(double var1, double var3);

private:
	NoiseGenerator* m_source;
	NoiseGenerator* m_distort;

};

#endif // NOISEGENERATORDISTORT_H