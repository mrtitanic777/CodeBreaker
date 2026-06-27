#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ColorizerFoliage.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ColorizerFoliage.h"
#include "BufferedImage.h"
#include "Class.h"
#include "Exception.h"
#include "ImageIO.h"

JArray<int > ColorizerFoliage::s_foliageColorBuffer = JArray<int >(65536);

ColorizerFoliage::~ColorizerFoliage()
{
}

int ColorizerFoliage::getFoliageColor(double var0, double var2)
{
	var2 *= var0;
	int var4 = (int)((1.0 - var0) * 255.0);
	int var5 = (int)((1.0 - var2) * 255.0);
	return s_foliageColorBuffer[var5 << 8 | var4];
}

void ColorizerFoliage::initStatics()
{
	try {
	BufferedImage* var0 = ImageIO::read(Class(typeid(ColorizerFoliage)).getResource("/misc/foliagecolor.png"));
	var0->getRGB(0, 0, 256, 256, s_foliageColorBuffer, 0, 256);
	} catch (Exception* var1) {
	var1->printStackTrace();
	}
}
