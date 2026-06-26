#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

// Auto-generated from NoiseGenerator.java
// Target: Xbox 360 / C++03 / DirectX 9

class NoiseGenerator {
public:
	NoiseGenerator();
	virtual ~NoiseGenerator();

	virtual double generateNoise(double var1, double var3) = 0;

};

#endif // NOISEGENERATOR_H