#ifndef AXISALIGNEDBB_H
#define AXISALIGNEDBB_H

// Auto-generated from AxisAlignedBB.java
// Target: Xbox 360 / C++03 / DirectX 9

struct MovingObjectPosition;
struct Vec3D;

struct AxisAlignedBB {
public:
	AxisAlignedBB();
	AxisAlignedBB(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual ~AxisAlignedBB();

	float minX;
	float minY;
	float minZ;
	float maxX;
	float maxY;
	float maxZ;

	virtual AxisAlignedBB addCoord(float var1, float var2, float var3);
	virtual AxisAlignedBB expand(float var1, float var2, float var3);
	virtual float calculateXOffset(const AxisAlignedBB& var1, float var2);
	virtual float calculateYOffset(const AxisAlignedBB& var1, float var2);
	virtual float calculateZOffset(const AxisAlignedBB& var1, float var2);
	virtual bool intersectsWith(const AxisAlignedBB& var1);
	virtual void offset(float var1, float var2, float var3);
	virtual AxisAlignedBB copy();
	virtual MovingObjectPosition calculateIntercept(const Vec3D& var1, const Vec3D& var2);

private:
	float m_epsilon;

	bool isVecInYZ(const Vec3D& var1);
	bool isVecInXZ(const Vec3D& var1);
	bool isVecInXY(const Vec3D& var1);

};

#endif // AXISALIGNEDBB_H