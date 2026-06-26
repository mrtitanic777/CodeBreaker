#include "stdafx.h"
// Auto-generated from ClippingHelperFaces.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ClippingHelperFaces.h"
#include "Common.h"

ClippingHelperFaces::ClippingHelperFaces()
	: posX(0)
	, posY(0)
	, posZ(0)
	, m_posXPlus(0)
	, m_posYPlus(0)
	, m_posZPlus(0)
	, m_clippingBoundingBox(0)
{
}

ClippingHelperFaces::ClippingHelperFaces(float var1, float var2, float var3, float var4, float var5)
	: posX(0)
	, posY(0)
	, posZ(0)
	, m_posXPlus(0)
	, m_posYPlus(0)
	, m_posZPlus(0)
	, m_clippingBoundingBox(0)
{
		posX = var1;
		posY = var2;
		posZ = var3;
		m_posXPlus = sinf(var4 / 180.0F * 3.14159265358979323846f) * cosf(var5 / 180.0F * 3.14159265358979323846f);
		m_posZPlus = -cosf(var4 / 180.0F * 3.14159265358979323846f) * cosf(var5 / 180.0F * 3.14159265358979323846f);
		m_posYPlus = -sinf(var5 / 180.0F * 3.14159265358979323846f);
		m_clippingBoundingBox = posX * m_posXPlus + posY * m_posYPlus + posZ * m_posZPlus;
}

ClippingHelperFaces::~ClippingHelperFaces() {
	// No dynamic resources
}

bool ClippingHelperFaces::isVisible(float var1, float var2, float var3, float var4) {
		return var1 * m_posXPlus + var2 * m_posYPlus + var3 * m_posZPlus > m_clippingBoundingBox - var4;
}

bool ClippingHelperFaces::isVisible(float var1, float var2, float var3, float var4, float var5, float var6) {
		return var1 * m_posXPlus + var2 * m_posYPlus + var3 * m_posZPlus > m_clippingBoundingBox || var4 * m_posXPlus + var2 * m_posYPlus + var3 * m_posZPlus > m_clippingBoundingBox || var1 * m_posXPlus + var5 * m_posYPlus + var3 * m_posZPlus > m_clippingBoundingBox || var4 * m_posXPlus + var5 * m_posYPlus + var3 * m_posZPlus > m_clippingBoundingBox || var1 * m_posXPlus + var2 * m_posYPlus + var6 * m_posZPlus > m_clippingBoundingBox || var4 * m_posXPlus + var2 * m_posYPlus + var6 * m_posZPlus > m_clippingBoundingBox || var1 * m_posXPlus + var5 * m_posYPlus + var6 * m_posZPlus > m_clippingBoundingBox || var4 * m_posXPlus + var5 * m_posYPlus + var6 * m_posZPlus > m_clippingBoundingBox;
}
