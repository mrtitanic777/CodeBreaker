#ifndef PATHENTITY_H
#define PATHENTITY_H

// Auto-generated from PathEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

class Entity;
class PathPoint;
class Vec3D;

class PathEntity {
public:
	PathEntity();
	PathEntity(PathPoint** var1, int var1_count = 0);
	virtual ~PathEntity();

	int m_pathLength;

	virtual void incrementPathIndex();
	virtual bool isFinished();
	virtual Vec3D* getPosition(Entity* var1);

public: // was private
	PathPoint** m_points;
	int m_pathIndex;

public:  // array-length companions
	int m_points_count;

};

#endif // PATHENTITY_H