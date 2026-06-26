#include "stdafx.h"
// Auto-generated from EntityCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreeper.h"
#include "Common.h"
#include "Entity.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityMob.h"

EntityCreeper::EntityCreeper()
	: m_timeSinceIgnited(0)
	, m_lastActiveTime(0)
	, m_fuseTime(30)
	, m_creeperState(-1)
{
}

EntityCreeper::EntityCreeper(World* var1)
	: EntityMob(var1)
	, m_timeSinceIgnited(0)
	, m_lastActiveTime(0)
	, m_fuseTime(30)
	, m_creeperState(-1)
{
		texture = "/mob/creeper.png";
}

EntityCreeper::~EntityCreeper() {
	// No dynamic resources
}

void EntityCreeper::writeEntityToNBT(NBTTagCompound* var1) {
		EntityMob::writeEntityToNBT(var1);
}

void EntityCreeper::readEntityFromNBT(NBTTagCompound* var1) {
		EntityMob::readEntityFromNBT(var1);
}

const std::string& EntityCreeper::getEntityString() {
		return "Creeper";
}

void EntityCreeper::updatePlayerActionState() {
		m_lastActiveTime = m_timeSinceIgnited;
		if(m_timeSinceIgnited > 0 && m_creeperState < 0) {
			--m_timeSinceIgnited;
		}

		if(m_creeperState >= 0) {
			m_creeperState = 2;
		}

		EntityMob::updatePlayerActionState();
		if(m_creeperState != 1) {
			m_creeperState = -1;
		}

}

void EntityCreeper::attackEntity(Entity* var1, float var2) {
		if(m_creeperState <= 0 && var2 < 3.0F || m_creeperState > 0 && var2 < 7.0F) {
			if(m_timeSinceIgnited == 0) {
				worldObj->a(this, "random.m_fuse", 1.0F, 0.5F);
			}

			m_creeperState = 1;
			++m_timeSinceIgnited;
			if(m_timeSinceIgnited == m_fuseTime) {
				worldObj->createExplosion(this, posX, posY, posZ, 3.0F);
				setEntityDead();
			}

			hasAttacked = true;
		}

}

float EntityCreeper::c(float var1) {
		return ((float)m_lastActiveTime + (float)(m_timeSinceIgnited - m_lastActiveTime) * var1) / (float)(m_fuseTime - 2);
}
