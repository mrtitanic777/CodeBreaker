#include "stdafx.h"
// Auto-generated from EntityPickupFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPickupFX.h"
#include "Common.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "Tesselator.h"
#include "RenderManager.h"

EntityPickupFX::EntityPickupFX()
	: m_entityToPickUp(NULL)
	, m_entityPickingUp(NULL)
	, m_age(0)
	, m_maxAge(0)
	, m_yOffs(0)
{
}

EntityPickupFX::EntityPickupFX(World* var1, Entity* var2, EntityLiving* var3, float var4)
	: EntityFX(var1, var2->posX, var2->posY, var2->posZ, var2->motionX, var2->motionY, var2->motionZ)
	, m_entityToPickUp(NULL)
	, m_entityPickingUp(NULL)
	, m_age(0)
	, m_maxAge(0)
	, m_yOffs(0)
{
		m_entityToPickUp = var2;
		m_entityPickingUp = var3;
		m_maxAge = 3;
		m_yOffs = -0.5F;
}

EntityPickupFX::~EntityPickupFX() {
	m_entityToPickUp = NULL;
	m_entityPickingUp = NULL;
}

void EntityPickupFX::renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var9 = ((float)m_age + var2) / (float)m_maxAge;
		var9 *= var9;
		var3 = m_entityToPickUp->posX;
		var4 = m_entityToPickUp->posY;
		var5 = m_entityToPickUp->posZ;
		var6 = m_entityPickingUp->lastTickPosX + (m_entityPickingUp->posX - m_entityPickingUp->lastTickPosX) * var2;
		var7 = m_entityPickingUp->lastTickPosY + (m_entityPickingUp->posY - m_entityPickingUp->lastTickPosY) * var2 + m_yOffs;
		float var8 = m_entityPickingUp->lastTickPosZ + (m_entityPickingUp->posZ - m_entityPickingUp->lastTickPosZ) * var2;
		var3 += (var6 - var3) * var9;
		var4 += (var7 - var4) * var9;
		var9 = var5 + (var8 - var5) * var9;
		var5 = worldObj->getLightBrightness((int)var3, (int)var4, (int)var9);
  Tesselator::getInstance().color4f(var5, var5, var5, 1.0F);
		RenderManager::instance->a(m_entityToPickUp, var3, var4, var9, m_entityToPickUp->rotationYaw, var2);
}

void EntityPickupFX::onEntityUpdate() {
		++m_age;
		if(m_age == m_maxAge) {
			setEntityDead();
		}

}

int EntityPickupFX::getFXLayer() {
		return 2;
}
