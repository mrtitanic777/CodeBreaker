#ifndef ICAMERA_H
#define ICAMERA_H

// Auto-generated from ICamera.java
// Target: Xbox 360 / C++03 / DirectX 9

struct AxisAlignedBB;

class ICamera {
public:
	virtual ~ICamera() {}
	virtual bool func_342_a(AxisAlignedBB var1) = 0;
	virtual void func_343_a(double var1, double var3, double var5) = 0;
};

#endif // ICAMERA_H