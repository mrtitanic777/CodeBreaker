#include "stdafx.h"
// Auto-generated from EntitySlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySlime.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "Chunk.h"
#include "EntityLiving.h"
#include "Item.h"

EntitySlime::EntitySlime()
	: m_field_768_a(0)
	, m_field_767_b(0)
	, m_field_769_d(0)
	, m_field_770_c(1)
{
}

EntitySlime::EntitySlime(World* var1)
	: EntityLiving(var1)
	, m_field_768_a(0)
	, m_field_767_b(0)
	, m_field_769_d(0)
	, m_field_770_c(1)
{
		texture = "/mob/slime.png";
		m_field_770_c = 1 << rand->nextInt(3);
		yOffset = 0.0F;
		m_field_769_d = rand->nextInt(20) + 10;
		func_441_c(m_field_770_c);
}

EntitySlime::~EntitySlime() {
	// No dynamic resources
}

void EntitySlime::func_441_c(int var1) {
		m_field_770_c = var1;
		setSize(0.6F * (float)var1, 0.6F * (float)var1);
		health = var1 * var1;
		setPosition(posX, posY, posZ);
}

void EntitySlime::writeEntityToNBT(NBTTagCompound* var1) {
		EntityLiving::writeEntityToNBT(var1);
		var1->setInteger("Size", m_field_770_c - 1);
}

void EntitySlime::readEntityFromNBT(NBTTagCompound* var1) {
		EntityLiving::readEntityFromNBT(var1);
		m_field_770_c = var1->getInteger("Size") + 1;
}

void EntitySlime::onUpdate() {
		m_field_767_b = m_field_768_a;
		bool var1 = onGround;
		EntityLiving::onUpdate();
		if(onGround && !var1) {
			for(int var2 = 0; var2 < m_field_770_c * 8; ++var2) {
				float var3 = rand->nextFloat() * 3.14159265358979323846f * 2.0F;
				float var4 = rand->nextFloat() * 0.5F + 0.5F;
				float var5 = sinf(var3) * (float)m_field_770_c * 0.5F * var4;
				float var6 = cosf(var3) * (float)m_field_770_c * 0.5F * var4;
				worldObj->spawnParticle("slime", posX + (double)var5, boundingBox.minY, posZ + (double)var6, 0.0f, 0.0f, 0.0f);
			}

			if(m_field_770_c > 2) {
				worldObj->playSoundAtEntity(this, "mob.slime", getSoundVolume(), ((rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F) / 0.8F);
			}

			m_field_768_a = -0.5F;
		}

		m_field_768_a *= 0.6F;
}

void EntitySlime::func_418_b_() {
		EntityPlayer* var1 = worldObj->getClosestPlayerToEntity(this, 16.0f);
		if(var1 != NULL) {
			faceEntity(var1, 10.0F);
		}

		if(onGround && m_field_769_d-- <= 0) {
			m_field_769_d = rand->nextInt(20) + 10;
			if(var1 != NULL) {
				m_field_769_d /= 3;
			}

			isJumping = true;
			if(m_field_770_c > 1) {
				worldObj->playSoundAtEntity(this, "mob.slime", getSoundVolume(), ((rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F) * 0.8F);
			}

			m_field_768_a = 1.0F;
			m_field_9342_ah = 1.0F - rand->nextFloat() * 2.0F;
			m_field_9340_ai = (float)(1 * m_field_770_c);
		} else {
			isJumping = false;
			if(onGround) {
				m_field_9342_ah = m_field_9340_ai = 0.0F;
			}
		}

}

void EntitySlime::setEntityDead() {
		if(m_field_770_c > 1 && health == 0) {
			for(int var1 = 0; var1 < 4; ++var1) {
				float var2 = ((float)(var1 % 2) - 0.5F) * (float)m_field_770_c / 4.0F;
				float var3 = ((float)(var1 / 2) - 0.5F) * (float)m_field_770_c / 4.0F;
				EntitySlime* var4 = new EntitySlime(worldObj);
				var4->func_441_c(m_field_770_c / 2);
				var4->setLocationAndAngles(posX + (double)var2, posY + 0.5f, posZ + (double)var3, rand->nextFloat() * 360.0F, 0.0F);
				worldObj->entityJoinedWorld(var4);
			}
		}

		EntityLiving::setEntityDead();
}

void EntitySlime::onCollideWithPlayer(EntityPlayer* var1) {
		if(m_field_770_c > 1 && canEntityBeSeen(var1) && (double)getDistanceToEntity(var1) < 0.6f * (double)m_field_770_c && var1->canAttackEntity(this, m_field_770_c)) {
			worldObj->playSoundAtEntity(this, "mob.slimeattack", 1.0F, (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
		}

}

const std::string& EntitySlime::getHurtSound() {
		static const std::string _ret = "mob.slime"; return _ret;
}

const std::string& EntitySlime::getDeathSound() {
		static const std::string _ret = "mob.slime"; return _ret;
}

int EntitySlime::getDropItemId() {
		return m_field_770_c == 1 ? Item::s_slimeBall->shiftedIndex : 0;
}

bool EntitySlime::getCanSpawnHere() {
		Chunk* var1 = worldObj->getChunkFromBlockCoords((int)floor(posX), (int)floor(posZ));
		return (m_field_770_c == 1 || worldObj->m_difficultySetting > 0) && rand->nextInt(10) == 0 && var1->func_997_a(987234911L)->nextInt(10) == 0 && posY < 16.0f;
}

float EntitySlime::getSoundVolume() {
		return 0.6F;
}
