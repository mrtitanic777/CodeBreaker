#ifndef FRUSTRUM_H
#define FRUSTRUM_H

// Auto-generated from Frustrum.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ICamera.h"

class AxisAlignedBB;
class ClippingHelper;

class Frustrum : public ICamera {
public:
	Frustrum();
	virtual ~Frustrum();

	ClippingHelper* clippingHelper;
	double xPosition;
	double yPosition;
	double zPosition;

	virtual void setPosition(double var1, double var3, double var5);
	virtual bool isBoxInFrustum(double var1, double var3, double var5, double var7, double var9, double var11);
	virtual bool isBoundingBoxInFrustum(AxisAlignedBB* var1);

	static void initStatics();
};

#endif // FRUSTRUM_H