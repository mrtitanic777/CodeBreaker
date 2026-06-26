#include "stdafx.h"
// Auto-generated from EntityGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityGhast.h"
#include "Common.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "World.h"
#include <string>
#include "AxisAlignedBB.h"
#include "Item.h"
#include "Vec3D.h"
#include "EntityFireball.h"
#include "EntityFlying.h"

EntityGhast::EntityGhast()
	: m_field_4121_a(0)
	, m_field_4120_b(0)
	, m_field_4127_c(0)
	, m_field_4126_d(0)
	, m_field_4125_e(0)
	, m_field_4124_f(0)
	, m_field_4123_g(NULL)
	, m_field_4122_h(0)
{
}

EntityGhast::EntityGhast(World* var1)
	: EntityFlying(var1)
	, m_field_4121_a(0)
	, m_field_4120_b(0)
	, m_field_4127_c(0)
	, m_field_4126_d(0)
	, m_field_4125_e(0)
	, m_field_4124_f(0)
	, m_field_4123_g(NULL)
	, m_field_4122_h(0)
{
		texture = "/mob/ghast.png";
		setSize(4.0F, 4.0F);
		m_isImmuneToFire = true;
}

EntityGhast::~EntityGhast() {
	m_field_4123_g = NULL;
}

void EntityGhast::func_418_b_() {
		if(worldObj->m_difficultySetting == 0) {
			setEntityDead();
		}

		m_field_4125_e = m_field_4124_f;
		double var1 = m_field_4120_b - posX;
		double var3 = m_field_4127_c - posY;
		double var5 = m_field_4126_d - posZ;
		double var7 = (double)sqrt(var1 * var1 + var3 * var3 + var5 * var5);
		if(var7 < 1.0f || var7 > 60.0f) {
			m_field_4120_b = posX + (double)((rand->nextFloat() * 2.0F - 1.0F) * 16.0F);
			m_field_4127_c = posY + (double)((rand->nextFloat() * 2.0F - 1.0F) * 16.0F);
			m_field_4126_d = posZ + (double)((rand->nextFloat() * 2.0F - 1.0F) * 16.0F);
		}

		if(m_field_4121_a-- <= 0) {
			m_field_4121_a += rand->nextInt(5) + 2;
			if(func_4050_a(m_field_4120_b, m_field_4127_c, m_field_4126_d, var7)) {
				motionX += var1 / var7 * 0.1f;
				motionY += var3 / var7 * 0.1f;
				motionZ += var5 / var7 * 0.1f;
			} else {
				m_field_4120_b = posX;
				m_field_4127_c = posY;
				m_field_4126_d = posZ;
			}
		}

		if(m_field_4123_g != NULL && m_field_4123_g->isDead) {
			m_field_4123_g = NULL;
		}

		if(m_field_4123_g == NULL || m_field_4122_h-- <= 0) {
			m_field_4123_g = (Entity*)worldObj->getClosestPlayerToEntity(this, 100.0f);
			if(m_field_4123_g != NULL) {
				m_field_4122_h = 20;
			}
		}

		double var9 = 64.0f;
		if(m_field_4123_g != NULL && m_field_4123_g->getDistanceSqToEntity(this) < var9 * var9) {
			double var11 = m_field_4123_g->posX - posX;
			double var13 = m_field_4123_g->boundingBox.minY + (double)(m_field_4123_g->height / 2.0F) - (posY + (double)(height / 2.0F));
			double var15 = m_field_4123_g->posZ - posZ;
			m_field_735_n = rotationYaw = -((float)atan2f(var11, var15)) * 180.0F / 3.14159265358979323846f;
			if(canEntityBeSeen(m_field_4123_g)) {
				if(m_field_4124_f == 10) {
					worldObj->playSoundAtEntity(this, "mob.ghast.charge", getSoundVolume(), (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
				}

				++m_field_4124_f;
				if(m_field_4124_f == 20) {
					worldObj->playSoundAtEntity(this, "mob.ghast.fireball", getSoundVolume(), (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
					EntityFireball* var17 = new EntityFireball(worldObj, this, var11, var13, var15);
					double var18 = 4.0f;
					Vec3D* var20 = getLook(1.0F);
					var17->posX = posX + var20->xCoord * var18;
					var17->posY = posY + (double)(height / 2.0F) + 0.5f;
					var17->posZ = posZ + var20->zCoord * var18;
					worldObj->entityJoinedWorld(var17);
					m_field_4124_f = -40;
				}
			} else if(m_field_4124_f > 0) {
				--m_field_4124_f;
			}
		} else {
			m_field_735_n = rotationYaw = -((float)atan2f(motionX, motionZ)) * 180.0F / 3.14159265358979323846f;
			if(m_field_4124_f > 0) {
				--m_field_4124_f;
			}
		}

		texture = m_field_4124_f > 10 ? "/mob/ghast_fire.png" : "/mob/ghast.png";
}

bool EntityGhast::func_4050_a(double var1, double var3, double var5, double var7) {
		double var9 = (m_field_4120_b - posX) / var7;
		double var11 = (m_field_4127_c - posY) / var7;
		double var13 = (m_field_4126_d - posZ) / var7;
		AxisAlignedBB var15 = boundingBox.copy();

		for(int var16 = 1; (double)var16 < var7; ++var16) {
			var15.offset(var9, var11, var13);
			if(worldObj->getCollidingBoundingBoxes(this, var15).size() > 0) {
				return false;
			}
		}

		return true;
}

const std::string& EntityGhast::getLivingSound() {
		static const std::string s("mob.ghast.moan");
		return s;
}

const std::string& EntityGhast::getHurtSound() {
		static const std::string s("mob.ghast.scream");
		return s;
}

const std::string& EntityGhast::getDeathSound() {
		static const std::string s("mob.ghast.death");
		return s;
}

int EntityGhast::getDropItemId() {
		return Item::s_gunpowder->shiftedIndex;
}

float EntityGhast::getSoundVolume() {
		return 10.0F;
}

bool EntityGhast::getCanSpawnHere() {
		return rand->nextInt(20) == 0 && EntityFlying::getCanSpawnHere() && worldObj->m_difficultySetting > 0;
}

int EntityGhast::func_6391_i() {
		return 1;
}
