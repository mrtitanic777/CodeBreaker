#ifndef AXISALIGNEDBB_H
#define AXISALIGNEDBB_H

// Auto-generated from AxisAlignedBB.java
// Target: Xbox 360 / C++03 / DirectX 9

class MovingObjectPosition;
class Vec3D;

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
	virtual float calculateXOffset(AxisAlignedBB var1, float var2);
	virtual float calculateYOffset(AxisAlignedBB var1, float var2);
	virtual float calculateZOffset(AxisAlignedBB var1, float var2);
	virtual bool intersectsWith(AxisAlignedBB var1);
	virtual void offset(float var1, float var2, float var3);
	virtual AxisAlignedBB copy();
	virtual MovingObjectPosition* calculateIntercept(Vec3D* var1, Vec3D* var2);

private:
	float m_epsilon;

	bool isVecInYZ(Vec3D* var1);
	bool isVecInXZ(Vec3D* var1);
	bool isVecInXY(Vec3D* var1);

};

#endif // AXISALIGNEDBB_H