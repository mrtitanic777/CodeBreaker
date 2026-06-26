#ifndef PATH_H
#define PATH_H

// Auto-generated from Path.java
// Target: Xbox 360 / C++03 / DirectX 9

class PathPoint;

class Path {
public:
	Path();
	virtual ~Path();

	virtual PathPoint* addPoint(PathPoint* var1);
	virtual void clearPath();
	virtual PathPoint* dequeue();
	virtual void changeDistance(PathPoint* var1, float var2);
	virtual bool isPathEmpty();

public: // was private
	PathPoint** m_pathPoints;
	int m_count;

	void sortBack(int var1);
	void sortForward(int var1);

public:  // array-length companions
	int m_pathPoints_count;

};

#endif // PATH_H