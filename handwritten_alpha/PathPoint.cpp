#include "stdafx.h"
// Auto-generated from PathPoint.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PathPoint.h"
#include "Common.h"
#include <string>

PathPoint::PathPoint()
	: xCoord(0)
	, yCoord(0)
	, zCoord(0)
	, m_hash(0)
	, m_isFirst(false)
	, m_index(-1)
	, m_totalPathDistance(0)
	, m_distanceToNext(0)
	, m_distanceToTarget(0)
	, m_previous(NULL)
{
}

PathPoint::PathPoint(int var1, int var2, int var3)
	: xCoord(0)
	, yCoord(0)
	, zCoord(0)
	, m_hash(0)
	, m_isFirst(false)
	, m_index(-1)
	, m_totalPathDistance(0)
	, m_distanceToNext(0)
	, m_distanceToTarget(0)
	, m_previous(NULL)
{
		xCoord = var1;
		yCoord = var2;
		zCoord = var3;
		m_hash = var1 | var2 << 10 | var3 << 20;
}

PathPoint::~PathPoint() {
	m_previous = NULL;
}

float PathPoint::distanceTo(PathPoint* var1) {
		float var2 = (float)(var1->xCoord - xCoord);
		float var3 = (float)(var1->yCoord - yCoord);
		float var4 = (float)(var1->zCoord - zCoord);
		return sqrtf(var2 * var2 + var3 * var3 + var4 * var4);
}

bool PathPoint::equals(void* /* PORT: was Object */ var1) {
		return ((PathPoint*)var1)->m_hash == m_hash;
}

int PathPoint::hashCode() {
		return m_hash;
}

bool PathPoint::isAssigned() {
		return m_index >= 0;
}

std::string PathPoint::toString() {
		// PORT: header returns const std::string& (auto-gen convention). Cache in a
		// static so we don't return a reference to a destroyed temporary.
		static std::string s;
		s = toStr(xCoord) + ", " + toStr(yCoord) + ", " + toStr(zCoord);
		return s;
}
