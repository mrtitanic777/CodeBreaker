#ifndef MOVINGOBJECTPOSITION_H
#define MOVINGOBJECTPOSITION_H

// Auto-generated from MovingObjectPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Vec3D.h"

class Entity;

struct MovingObjectPosition {
public:
	MovingObjectPosition();
	MovingObjectPosition(int var1, int var2, int var3, int var4, const Vec3D& var5);
	MovingObjectPosition(Entity* var1);
	virtual ~MovingObjectPosition();

	int m_typeOfHit;
	int m_blockX;
	int m_blockY;
	int m_blockZ;
	int m_sideHit;
	Vec3D m_hitVec;
	bool m_hasHitVec;
	Entity* m_entityHit;


};

#endif // MOVINGOBJECTPOSITION_H