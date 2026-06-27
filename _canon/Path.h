#ifndef PATH_H
#define PATH_H

// Auto-generated from Path.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class PathPoint;

class Path {
public:
	Path();
	virtual ~Path();

	JArray<PathPoint* > pathPoints;
	int count;

	virtual PathPoint* addPoint(PathPoint* var1);
	virtual void clearPath();
	virtual PathPoint* dequeue();
	virtual void changeDistance(PathPoint* var1, float var2);
	virtual void sortBack(int var1);
	virtual void sortForward(int var1);
	virtual bool isPathEmpty();

	static void initStatics();
};

#endif // PATH_H