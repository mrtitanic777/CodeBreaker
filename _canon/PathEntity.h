#ifndef PATHENTITY_H
#define PATHENTITY_H

// Auto-generated from PathEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Entity;
class PathPoint;
class Vec3D;

class PathEntity {
public:
	PathEntity(JArray<PathPoint* > var1);
	virtual ~PathEntity();

	JArray<PathPoint* > points;
	int pathLength;
	int pathIndex;

	virtual void incrementPathIndex();
	virtual bool isFinished();
	virtual Vec3D* getPosition(Entity* var1);

	static void initStatics();
};

#endif // PATHENTITY_H