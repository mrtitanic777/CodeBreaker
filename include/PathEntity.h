#ifndef PATHENTITY_H
#define PATHENTITY_H

// Auto-generated from PathEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PathPoint.h"

class Entity;
struct Vec3D;

class PathEntity {
public:
	PathEntity();
	PathEntity(PathPoint* var1);
	virtual ~PathEntity();

	virtual void incrementPathIndex();
	virtual bool isFinished();
	virtual Vec3D getPosition(Entity* var1);

private:
	PathPoint* m_points;
	int m_pathIndex;

};

#endif // PATHENTITY_H