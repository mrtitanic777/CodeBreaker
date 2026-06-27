#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ColorizerGrass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ColorizerGrass.h"
#include "BufferedImage.h"
#include "Class.h"
#include "ColorizerFoliage.h"
#include "Exception.h"
#include "ImageIO.h"

JArray<int > ColorizerGrass::s_grassBuffer = JArray<int >(65536);

ColorizerGrass::~ColorizerGrass()
{
}

int ColorizerGrass::getGrassColor(double var0, double var2)
{
	var2 *= var0;
	int var4 = (int)((1.0 - var0) * 255.0);
	int var5 = (int)((1.0 - var2) * 255.0);
	return s_grassBuffer[var5 << 8 | var4];
}

void ColorizerGrass::initStatics()
{
	try {
	BufferedImage* var0 = ImageIO::read(Class(typeid(ColorizerFoliage)).getResource("/misc/grasscolor.png"));
	var0->getRGB(0, 0, 256, 256, s_grassBuffer, 0, 256);
	} catch (Exception* var1) {
	var1->printStackTrace();
	}
}
