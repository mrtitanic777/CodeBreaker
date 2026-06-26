#include "stdafx.h"
// Auto-generated from PathEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PathEntity.h"
#include "Common.h"
#include "Entity.h"
#include "Vec3D.h"
#include "Vec3.h"

PathEntity::PathEntity()
	: m_points(NULL)
	, m_pathIndex(0)
{
}

PathEntity::PathEntity(PathPoint* var1)
	: m_points(NULL)
	, m_pathIndex(0)
{
		m_points = var1;
}

PathEntity::~PathEntity() {
	delete[] m_points;
	m_points = NULL;
}

void PathEntity::incrementPathIndex() {
		++m_pathIndex;
}

bool PathEntity::isFinished() {
		return m_pathIndex >= 4 /* PORT: quad vertex count */;
}

Vec3D PathEntity::getPosition(Entity* var1) {
		float var2 = (float)m_points[m_pathIndex].m_xCoord + (float)((int)(var1->width + 1.0F)) * 0.5F;
		float var3 = (float)m_points[m_pathIndex].m_yCoord;
		float var4 = (float)m_points[m_pathIndex].m_zCoord + (float)((int)(var1->width + 1.0F)) * 0.5F;
		return new Vec3D(var2, var3, var4);
}
