#ifndef AXISALIGNEDBB_H
#define AXISALIGNEDBB_H

// Auto-generated from AxisAlignedBB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class MovingObjectPosition;
class Vec3D;

class AxisAlignedBB {
public:
	AxisAlignedBB(double var1, double var3, double var5, double var7, double var9, double var11);
	virtual ~AxisAlignedBB();

	double minX;
	double minY;
	double minZ;
	double maxX;
	double maxY;
	double maxZ;
	static JList<void*> s_boundingBoxes;
	static int s_numBoundingBoxesInUse;

	static AxisAlignedBB* getBoundingBox(double var0, double var2, double var4, double var6, double var8, double var10);
	static void clearBoundingBoxPool();
	static AxisAlignedBB* getBoundingBoxFromPool(double var0, double var2, double var4, double var6, double var8, double var10);
	virtual AxisAlignedBB* setBounds(double var1, double var3, double var5, double var7, double var9, double var11);
	virtual AxisAlignedBB* addCoord(double var1, double var3, double var5);
	virtual AxisAlignedBB* expands(double var1, double var3, double var5);
	virtual AxisAlignedBB* getOffsetBoundingBox(double var1, double var3, double var5);
	virtual double calculateXOffset(AxisAlignedBB* var1, double var2);
	virtual double calculateYOffset(AxisAlignedBB* var1, double var2);
	virtual double calculateZOffset(AxisAlignedBB* var1, double var2);
	virtual bool intersectsWith(AxisAlignedBB* var1);
	virtual AxisAlignedBB* offset(double var1, double var3, double var5);
	virtual bool isVecInside(Vec3D* var1);
	virtual double getAverageEdgeLength();
	virtual AxisAlignedBB* copy();
	virtual MovingObjectPosition* calculateIntercept(Vec3D* var1, Vec3D* var2);
	virtual bool isVecInYZ(Vec3D* var1);
	virtual bool isVecInXZ(Vec3D* var1);
	virtual bool isVecInXY(Vec3D* var1);
	virtual void setBB(AxisAlignedBB* var1);

	static void initStatics();
};

#endif // AXISALIGNEDBB_H