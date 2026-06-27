#ifndef PATHPOINT_H
#define PATHPOINT_H

// Auto-generated from PathPoint.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class PathPoint {
public:
	PathPoint(int var1, int var2, int var3);
	virtual ~PathPoint();

	int xCoord;
	int yCoord;
	int zCoord;
	int hash;
	int index;
	float totalPathDistance;
	float distanceToNext;
	float distanceToTarget;
	PathPoint* previous;
	bool isFirst;

	virtual float distanceTo(PathPoint* var1);
	virtual bool equals(JObject var1);
	virtual int hashCode();
	virtual bool isAssigned();
	virtual JavaString toString();

	static void initStatics();
};

#endif // PATHPOINT_H