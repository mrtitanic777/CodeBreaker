#include "stdafx.h"
// Auto-generated from NoiseGeneratorDistort.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGeneratorDistort.h"
#include "Common.h"

NoiseGeneratorDistort::NoiseGeneratorDistort()
	: m_source(NULL)
	, m_distort(NULL)
{
}

NoiseGeneratorDistort::NoiseGeneratorDistort(NoiseGenerator* var1, NoiseGenerator* var2)
	: m_source(NULL)
	, m_distort(NULL)
{
		m_source = var1;
		m_distort = var2;
}

NoiseGeneratorDistort::~NoiseGeneratorDistort() {
	m_source = NULL;
	m_distort = NULL;
}

double NoiseGeneratorDistort::generateNoise(double var1, double var3) {
		return m_source->generateNoise(var1 + m_distort->generateNoise(var1, var3), var3);
}
