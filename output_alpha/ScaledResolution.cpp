#include "stdafx.h"
// Auto-generated from ScaledResolution.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ScaledResolution.h"
#include "Common.h"

ScaledResolution::ScaledResolution()
	: m_scaledWidth(0)
	, m_scaledHeight(0)
	, m_scaleFactor(0)
{
}

ScaledResolution::ScaledResolution(int var1, int var2)
	: m_scaledWidth(0)
	, m_scaledHeight(0)
	, m_scaleFactor(0)
{
		m_scaledWidth = var1;
		m_scaledHeight = var2;

		for(m_scaleFactor = 1; m_scaledWidth / (m_scaleFactor + 1) >= 320 && m_scaledHeight / (m_scaleFactor + 1) >= 240; ++m_scaleFactor) {
		}

		m_scaledWidth /= m_scaleFactor;
		m_scaledHeight /= m_scaleFactor;
}

ScaledResolution::~ScaledResolution() {
	// No dynamic resources
}

int ScaledResolution::getScaledWidth() {
		return m_scaledWidth;
}

int ScaledResolution::getScaledHeight() {
		return m_scaledHeight;
}
