#include "stdafx.h"
// Auto-generated from EntityCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreeper.h"
#include "Common.h"
#include "Entity.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntitySkeleton.h"
#include "Item.h"
#include "EntityMobs.h"

EntityCreeper::EntityCreeper()
	: m_timeSinceIgnited(0)
	, m_lastActiveTime(0)
	, m_fuseTime(30)
	, m_creeperState(-1)
	, m_field_12241_e(-1)
{
}

EntityCreeper::EntityCreeper(World* var1)
	: EntityMobs(var1)
	, m_timeSinceIgnited(0)
	, m_lastActiveTime(0)
	, m_fuseTime(30)
	, m_creeperState(-1)
	, m_field_12241_e(-1)
{
		texture = "/mob/creeper.png";
}

EntityCreeper::~EntityCreeper() {
	// No dynamic resources
}

void EntityCreeper::writeEntityToNBT(NBTTagCompound* var1) {
		EntityMobs::writeEntityToNBT(var1);
}

void EntityCreeper::readEntityFromNBT(NBTTagCompound* var1) {
		EntityMobs::readEntityFromNBT(var1);
}

void EntityCreeper::func_9282_a(unsigned char var1) {
		EntityMobs::func_9282_a(var1);
		if(var1 == 4) {
			if(m_timeSinceIgnited == 0) {
				worldObj->playSoundAtEntity(this, "random.m_fuse", 1.0F, 0.5F);
			}

			m_creeperState = 1;
		}

		if(var1 == 5) {
			m_creeperState = -1;
		}

}

void EntityCreeper::onUpdate() {
		m_lastActiveTime = m_timeSinceIgnited;
		if(worldObj->m_multiplayerWorld) {
			m_timeSinceIgnited += m_creeperState;
			if(m_timeSinceIgnited < 0) {
				m_timeSinceIgnited = 0;
			}

			if(m_timeSinceIgnited >= m_fuseTime) {
				m_timeSinceIgnited = m_fuseTime;
			}
		}

		EntityMobs::onUpdate();
}

void EntityCreeper::func_418_b_() {
		if(m_field_12241_e != m_creeperState) {
			m_field_12241_e = m_creeperState;
			if(m_creeperState > 0) {
				worldObj->func_9425_a(this, (unsigned char)4);
			} else {
				worldObj->func_9425_a(this, (unsigned char)5);
			}
		}

		m_lastActiveTime = m_timeSinceIgnited;
		if(worldObj->m_multiplayerWorld) {
			EntityMobs::func_418_b_();
		} else {
			if(m_timeSinceIgnited > 0 && m_creeperState < 0) {
				--m_timeSinceIgnited;
			}

			if(m_creeperState >= 0) {
				m_creeperState = 2;
			}

			EntityMobs::func_418_b_();
			if(m_creeperState != 1) {
				m_creeperState = -1;
			}
		}

}

const std::string& EntityCreeper::getHurtSound() {
		static const std::string _ret = "mob.creeper"; return _ret;
}

const std::string& EntityCreeper::getDeathSound() {
		static const std::string _ret = "mob.creeperdeath"; return _ret;
}

void EntityCreeper::onDeath(Entity* var1) {
		EntityMobs::onDeath(var1);
		if(dynamic_cast<EntitySkeleton*>(var1) != NULL) {
			dropItem(Item::s_record13->shiftedIndex + rand->nextInt(2), 1);
		}

}

void EntityCreeper::attackEntity(Entity* var1, float var2) {
		if(m_creeperState <= 0 && var2 < 3.0F || m_creeperState > 0 && var2 < 7.0F) {
			if(m_timeSinceIgnited == 0) {
				worldObj->playSoundAtEntity(this, "random.m_fuse", 1.0F, 0.5F);
			}

			m_creeperState = 1;
			++m_timeSinceIgnited;
			if(m_timeSinceIgnited == m_fuseTime) {
				worldObj->func_12243_a(this, posX, posY, posZ, 3.0F);
				setEntityDead();
			}

			m_hasAttacked = true;
		}

}

float EntityCreeper::func_440_b(float var1) {
		return ((float)m_lastActiveTime + (float)(m_timeSinceIgnited - m_lastActiveTime) * var1) / (float)(m_fuseTime - 2);
}

int EntityCreeper::getDropItemId() {
		return Item::s_gunpowder->shiftedIndex;
}
