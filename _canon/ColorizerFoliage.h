#ifndef COLORIZERFOLIAGE_H
#define COLORIZERFOLIAGE_H

// Auto-generated from ColorizerFoliage.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ColorizerFoliage {
public:
	virtual ~ColorizerFoliage();

	static JArray<int > s_foliageColorBuffer;

	static int getFoliageColor(double var0, double var2);

	static void initStatics();
};

#endif // COLORIZERFOLIAGE_H