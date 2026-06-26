#ifndef FRUSTRUM_H
#define FRUSTRUM_H

// Auto-generated from Frustrum.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ICamera.h"

struct AxisAlignedBB;
class ClippingHelperFaces;
class EntityLiving;

class Frustrum : public ICamera {
public:
	Frustrum();
	Frustrum(EntityLiving* var1, float var2, float var3);
	virtual ~Frustrum();

	virtual bool isBoundingBoxInFrustrum(const AxisAlignedBB& var1);

private:
	ClippingHelperFaces* m_clippingHelper;

};

#endif // FRUSTRUM_H