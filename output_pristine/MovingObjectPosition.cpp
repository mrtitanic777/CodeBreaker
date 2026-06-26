#include "stdafx.h"
// Auto-generated from MovingObjectPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MovingObjectPosition.h"
#include "Common.h"
#include "Entity.h"
#include "Vec3D.h"
#include "Vec3.h"

MovingObjectPosition::MovingObjectPosition()
	: m_typeOfHit(0)
	, m_blockX(0)
	, m_blockY(0)
	, m_blockZ(0)
	, m_sideHit(0)
	, m_hitVec(NULL)
	, m_entityHit(NULL)
{
}

MovingObjectPosition::MovingObjectPosition(int var1, int var2, int var3, int var4, Vec3D* var5)
	: m_typeOfHit(0)
	, m_blockX(0)
	, m_blockY(0)
	, m_blockZ(0)
	, m_sideHit(0)
	, m_hitVec(NULL)
	, m_entityHit(NULL)
{
		m_typeOfHit = 0;
		m_blockX = var1;
		m_blockY = var2;
		m_blockZ = var3;
		m_sideHit = var4;
		m_hitVec = new Vec3::0f(var5->m_xCoord, var5->m_yCoord, var5->m_zCoord);
}

MovingObjectPosition::MovingObjectPosition(Entity* var1)
	: m_typeOfHit(0)
	, m_blockX(0)
	, m_blockY(0)
	, m_blockZ(0)
	, m_sideHit(0)
	, m_hitVec(NULL)
	, m_entityHit(NULL)
{
		m_typeOfHit = 1;
		m_entityHit = var1;
		m_hitVec = new Vec3::0f(var1->posX, var1->posY, var1->posZ);
}

MovingObjectPosition::~MovingObjectPosition() {
	m_hitVec = NULL;
	m_entityHit = NULL;
}
