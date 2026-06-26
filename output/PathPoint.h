#ifndef PATHPOINT_H
#define PATHPOINT_H

// Auto-generated from PathPoint.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class PathPoint {
public:
	PathPoint();
	PathPoint(int var1, int var2, int var3);
	virtual ~PathPoint();

	int m_xCoord;
	int m_yCoord;
	int m_zCoord;
	int m_hash;
	bool m_isFirst;

	virtual float distanceTo(PathPoint* var1);
	virtual bool equals(void* /* PORT: was Object */ var1);
	virtual int hashCode();
	virtual bool isAssigned();
	virtual const std::string& toString();

public: // was private
	int m_index;
	float m_totalPathDistance;
	float m_distanceToNext;
	float m_distanceToTarget;
	PathPoint* m_previous;

};

#endif // PATHPOINT_H