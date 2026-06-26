#ifndef ICAMERA_H
#define ICAMERA_H

// Auto-generated from ICamera.java
// Target: Xbox 360 / C++03 / DirectX 9

struct AxisAlignedBB;

class ICamera {
public:
	virtual ~ICamera() {}
	virtual bool isBoundingBoxInFrustrum(const AxisAlignedBB& var1) = 0;
};

#endif // ICAMERA_H