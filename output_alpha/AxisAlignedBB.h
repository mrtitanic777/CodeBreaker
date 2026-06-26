#ifndef AXISALIGNEDBB_H
#define AXISALIGNEDBB_H

// Auto-generated from AxisAlignedBB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <vector>

class MovingObjectPosition;
class Vec3D;

struct AxisAlignedBB {
public:
	AxisAlignedBB();
	AxisAlignedBB(double var1, double var3, double var5, double var7, double var9, double var11);
	virtual ~AxisAlignedBB();

	double minX;
	double minY;
	double minZ;
	double maxX;
	double maxY;
	double maxZ;

	static AxisAlignedBB getBoundingBox(double var0, double var2, double var4, double var6, double var8, double var10);
	static void clearBoundingBoxPool();
	static AxisAlignedBB getBoundingBoxFromPool(double var0, double var2, double var4, double var6, double var8, double var10);
	virtual AxisAlignedBB setBounds(double var1, double var3, double var5, double var7, double var9, double var11);
	virtual AxisAlignedBB addCoord(double var1, double var3, double var5);
	virtual AxisAlignedBB expands(double var1, double var3, double var5);
	virtual AxisAlignedBB getOffsetBoundingBox(double var1, double var3, double var5);
	virtual double func_1163_a(AxisAlignedBB var1, double var2);
	virtual double func_1172_b(AxisAlignedBB var1, double var2);
	virtual double func_1162_c(AxisAlignedBB var1, double var2);
	virtual bool intersectsWith(AxisAlignedBB var1);
	virtual AxisAlignedBB offset(double var1, double var3, double var5);
	virtual bool isVecInside(Vec3D* var1);
	virtual double getAverageEdgeLength();
	virtual AxisAlignedBB copy();
	virtual MovingObjectPosition* func_1169_a(Vec3D* var1, Vec3D* var2);
	virtual void setBB(AxisAlignedBB var1);

public: // was private
	static std::vector<void*> /* PORT: determine element type */ s_boundingBoxes;
	static int s_numBoundingBoxesInUse;

	bool isVecInYZ(Vec3D* var1);
	bool isVecInXZ(Vec3D* var1);
	bool isVecInXY(Vec3D* var1);

};

#endif // AXISALIGNEDBB_H