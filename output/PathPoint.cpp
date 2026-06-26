#include "stdafx.h"
// Auto-generated from PathPoint.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PathPoint.h"
#include "Common.h"
#include <string>

PathPoint::PathPoint()
	: m_xCoord(0)
	, m_yCoord(0)
	, m_zCoord(0)
	, m_hash(0)
	, m_index(-1)
	, m_totalPathDistance(0)
	, m_distanceToNext(0)
	, m_distanceToTarget(0)
	, m_previous(NULL)
	, m_isFirst(false)
{
}

PathPoint::PathPoint(int var1, int var2, int var3)
	: m_xCoord(0)
	, m_yCoord(0)
	, m_zCoord(0)
	, m_hash(0)
	, m_index(-1)
	, m_totalPathDistance(0)
	, m_distanceToNext(0)
	, m_distanceToTarget(0)
	, m_previous(NULL)
	, m_isFirst(false)
{
		m_xCoord = var1;
		m_yCoord = var2;
		m_zCoord = var3;
		m_hash = var1 | var2 << 10 | var3 << 20;
}

PathPoint::~PathPoint() {
	m_previous = NULL;
}

float PathPoint::distanceTo(PathPoint* var1) {
		float var2 = (float)(var1->m_xCoord - m_xCoord);
		float var3 = (float)(var1->m_yCoord - m_yCoord);
		float var4 = (float)(var1->m_zCoord - m_zCoord);
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

const std::string& PathPoint::toString() {
		return toStr(m_xCoord) + ", " + toStr(m_yCoord) + ", " + toStr(m_zCoord);
}
