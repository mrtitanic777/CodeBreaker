#ifndef COLORIZERGRASS_H
#define COLORIZERGRASS_H

// Auto-generated from ColorizerGrass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ColorizerGrass {
public:
	virtual ~ColorizerGrass();

	static JArray<int > s_grassBuffer;

	static int getGrassColor(double var0, double var2);

	static void initStatics();
};

#endif // COLORIZERGRASS_H