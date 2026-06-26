#include "stdafx.h"
// Auto-generated from EntityCreature.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreature.h"
#include "Common.h"
#include "Entity.h"
#include "PathEntity.h"
#include "World.h"
#include "EntityLiving.h"
#include "Vec3D.h"
#include "Pathfinder.h"

EntityCreature::EntityCreature()
	: m_pathToEntity(NULL)
	, m_playerToAttack(NULL)
	, m_hasAttacked(false)
{
}

EntityCreature::EntityCreature(World* var1)
	: EntityLiving(var1)
	, m_pathToEntity(NULL)
	, m_playerToAttack(NULL)
	, m_hasAttacked(false)
{
}

EntityCreature::~EntityCreature() {
	m_pathToEntity = NULL;
	m_playerToAttack = NULL;
}

void EntityCreature::func_418_b_() {
		m_hasAttacked = false;
		float var1 = 16.0F;
		if(m_playerToAttack == NULL) {
			m_playerToAttack = findPlayerToAttack();
			if(m_playerToAttack != NULL) {
				m_pathToEntity = worldObj->getPathToEntity(this, m_playerToAttack, var1);
			}
		} else if(!m_playerToAttack->isEntityAlive()) {
			m_playerToAttack = NULL;
		} else {
			float var2 = m_playerToAttack->getDistanceToEntity(this);
			if(canEntityBeSeen(m_playerToAttack)) {
				attackEntity(m_playerToAttack, var2);
			}
		}

		if(m_hasAttacked || m_playerToAttack == NULL || m_pathToEntity != NULL && rand->nextInt(20) != 0) {
			if(m_pathToEntity == NULL && rand->nextInt(80) == 0 || rand->nextInt(80) == 0) {
				bool var21 = false;
				int var3 = -1;
				int var4 = -1;
				int var5 = -1;
				float var6 = -99999.0F;

				for(int var7 = 0; var7 < 10; ++var7) {
					int var8 = (int)floor(posX + (double)rand->nextInt(13) - 6.0f);
					int var9 = (int)floor(posY + (double)rand->nextInt(7) - 3.0f);
					int var10 = (int)floor(posZ + (double)rand->nextInt(13) - 6.0f);
					float var11 = func_439_a(var8, var9, var10);
					if(var11 > var6) {
						var6 = var11;
						var3 = var8;
						var4 = var9;
						var5 = var10;
						var21 = true;
					}
				}

				if(var21) {
					m_pathToEntity = worldObj->getEntityPathToXYZ(this, var3, var4, var5, 10.0F);
				}
			}
		} else {
			m_pathToEntity = worldObj->getPathToEntity(this, m_playerToAttack, var1);
		}

		int var22 = (int)floor(boundingBox.minY);
		bool var23 = handleWaterMovement();
		bool var24 = handleLavaMovement();
		rotationPitch = 0.0F;
		if(m_pathToEntity != NULL && rand->nextInt(100) != 0) {
			Vec3D* var25 = m_pathToEntity->getPosition(this);
			double var26 = (double)(width * 2.0F);

			while(var25 != NULL && var25->squareDistanceTo(posX, var25->yCoord, posZ) < var26 * var26) {
				m_pathToEntity->incrementPathIndex();
				if(m_pathToEntity->isFinished()) {
					var25 = NULL;
					m_pathToEntity = NULL;
				} else {
					var25 = m_pathToEntity->getPosition(this);
				}
			}

			isJumping = false;
			if(var25 != NULL) {
				double var27 = var25->xCoord - posX;
				double var28 = var25->zCoord - posZ;
				double var12 = var25->yCoord - (double)var22;
				float var14 = (float)(atan2f(var28, var27) * 180.0f / (double)(3.14159265358979323846f)) - 90.0F;
				float var15 = var14 - rotationYaw;

				for(m_field_9340_ai = m_field_9333_am; var15 < -180.0F; var15 += 360.0F) {
				}

				while(var15 >= 180.0F) {
					var15 -= 360.0F;
				}

				if(var15 > 30.0F) {
					var15 = 30.0F;
				}

				if(var15 < -30.0F) {
					var15 = -30.0F;
				}

				rotationYaw += var15;
				if(m_hasAttacked && m_playerToAttack != NULL) {
					double var16 = m_playerToAttack->posX - posX;
					double var18 = m_playerToAttack->posZ - posZ;
					float var20 = rotationYaw;
					rotationYaw = (float)(atan2f(var18, var16) * 180.0f / (double)(3.14159265358979323846f)) - 90.0F;
					var15 = (var20 - rotationYaw + 90.0F) * 3.14159265358979323846f / 180.0F;
					m_field_9342_ah = -sinf(var15) * m_field_9340_ai * 1.0F;
					m_field_9340_ai = cosf(var15) * m_field_9340_ai * 1.0F;
				}

				if(var12 > 0.0f) {
					isJumping = true;
				}
			}

			if(m_playerToAttack != NULL) {
				faceEntity(m_playerToAttack, 30.0F);
			}

			if(m_field_9297_aI) {
				isJumping = true;
			}

			if(rand->nextFloat() < 0.8F && (var23 || var24)) {
				isJumping = true;
			}

		} else {
			EntityLiving::func_418_b_();
			m_pathToEntity = NULL;
		}
}

void EntityCreature::attackEntity(Entity* var1, float var2) {
	// TODO: implement
}

float EntityCreature::func_439_a(int var1, int var2, int var3) {
		return 0.0F;
}

Entity* EntityCreature::findPlayerToAttack() {
		return NULL;
}

bool EntityCreature::getCanSpawnHere() {
		int var1 = (int)floor(posX);
		int var2 = (int)floor(boundingBox.minY);
		int var3 = (int)floor(posZ);
		return EntityLiving::getCanSpawnHere() && func_439_a(var1, var2, var3) >= 0.0F;
}
