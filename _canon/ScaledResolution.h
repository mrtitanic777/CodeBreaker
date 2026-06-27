#ifndef SCALEDRESOLUTION_H
#define SCALEDRESOLUTION_H

// Auto-generated from ScaledResolution.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ScaledResolution {
public:
	ScaledResolution(int var1, int var2);
	virtual ~ScaledResolution();

	int scaledWidth;
	int scaledHeight;
	int scaleFactor;

	virtual int getScaledWidth();
	virtual int getScaledHeight();

	static void initStatics();
};

#endif // SCALEDRESOLUTION_H