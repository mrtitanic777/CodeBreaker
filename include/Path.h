#ifndef PATH_H
#define PATH_H

// Auto-generated from Path.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PathPoint.h"

class Path {
public:
	Path();
	virtual ~Path();

	virtual PathPoint addPoint(const PathPoint& var1);
	virtual void clearPath();
	virtual PathPoint b();
	virtual void changeDistance(const PathPoint& var1, float var2);
	virtual bool isPathEmpty();

private:
	PathPoint* m_pathPoints;
	int m_count;

	void sortBack(int var1);
	void sortForward(int var1);

};

#endif // PATH_H