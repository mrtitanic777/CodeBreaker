#ifndef ICAMERA_H
#define ICAMERA_H

// Auto-generated from ICamera.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class AxisAlignedBB;

class ICamera {
public:

	virtual bool isBoundingBoxInFrustum(AxisAlignedBB* var1) = 0;
	virtual void setPosition(double var1, double var3, double var5) = 0;
};

#endif // ICAMERA_H