#ifndef MOVINGOBJECTPOSITION_H
#define MOVINGOBJECTPOSITION_H

// Auto-generated from MovingObjectPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Entity;
class Vec3D;

class MovingObjectPosition {
public:
	MovingObjectPosition(int var1, int var2, int var3, int var4, Vec3D* var5);
	MovingObjectPosition(Entity* var1);
	virtual ~MovingObjectPosition();

	int typeOfHit;
	int blockX;
	int blockY;
	int blockZ;
	int sideHit;
	Vec3D* hitVec;
	Entity* entityHit;

	static void initStatics();
};

#endif // MOVINGOBJECTPOSITION_H