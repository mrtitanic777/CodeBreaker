#ifndef CLIPPINGHELPER_H
#define CLIPPINGHELPER_H

// Auto-generated from ClippingHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ClippingHelper {
public:
	ClippingHelper();
	virtual ~ClippingHelper();

	JArray<JArray<float > > frustum;
	JArray<float > projectionMatrix;
	JArray<float > modelviewMatrix;
	JArray<float > clippingMatrix;

	virtual bool isBoxInFrustum(double var1, double var3, double var5, double var7, double var9, double var11);

	static void initStatics();
};

#endif // CLIPPINGHELPER_H