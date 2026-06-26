#include "stdafx.h"
// Auto-generated from NoiseGeneratorOctaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGeneratorOctaves.h"
#include "Common.h"
#include "NoiseGeneratorPerlin.h"
#include "Random.h"

NoiseGeneratorOctaves::NoiseGeneratorOctaves()
	: m_generatorCollection(NULL)
	, m_octaves(0)
{
}

NoiseGeneratorOctaves::NoiseGeneratorOctaves(Random* var1, int var2)
	: m_generatorCollection(NULL)
	, m_octaves(0)
{
		m_octaves = var2;
		m_generatorCollection = new NoiseGeneratorPerlin[var2];

		for(int var3 = 0; var3 < var2; ++var3) {
			m_generatorCollection[var3] = NoiseGeneratorPerlin(var1);
		}

}

NoiseGeneratorOctaves::~NoiseGeneratorOctaves() {
	delete[] m_generatorCollection;
	m_generatorCollection = NULL;
}

double NoiseGeneratorOctaves::generateNoise(double var1, double var3) {
		double var5 = 0.0f;
		double var7 = 1.0f;

		for(int var9 = 0; var9 < m_octaves; ++var9) {
			var5 += m_generatorCollection[var9].generateNoise(var1 / var7, var3 / var7) * var7;
			var7 *= 2.0f;
		}

		return var5;
}
