#include "stdafx.h"
// Auto-generated from EntityCreature.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreature.h"
#include "Common.h"
#include "Entity.h"
#include "PathEntity.h"
#include "World.h"
#include "EntityLiving.h"
#include "Vec3.h"
#include "Pathfinder.h"
#include "Vec3D.h"

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

void EntityCreature::updatePlayerActionState() {
		m_hasAttacked = false;
		float var6;
		float var7;
		float var8;
		if(m_playerToAttack == NULL) {
			m_playerToAttack = findPlayerToAttack();
			if(m_playerToAttack != NULL) {
				m_pathToEntity = worldObj->m_pathFinder->createEntityPathTo(this, m_playerToAttack, 16.0F);
			}
		} else if(!m_playerToAttack->isEntityAlive()) {
			m_playerToAttack = NULL;
		} else {
			Entity* var5 = m_playerToAttack;
			var7 = var5->posX - posX;
			var8 = var5->posY - posY;
			var6 = var5->posZ - posZ;
			float var1 = sqrtf(var7 * var7 + var8 * var8 + var6 * var6);
			if(worldObj->rayTraceBlocks(new Vec3D(posX, posY + getEyeHeight(), posZ), new Vec3D(m_playerToAttack->posX, m_playerToAttack->posY + m_playerToAttack->getEyeHeight(), m_playerToAttack->posZ)) == NULL) {
				attackEntity(m_playerToAttack, var1);
			}
		}

		if(m_hasAttacked) {
			moveStrafing = 0.0F;
			moveForward = 0.0F;
			isJumping = false;
		} else {
			float var4;
			if(m_playerToAttack == NULL || m_pathToEntity != NULL && rand->nextInt(20) != 0) {
				if(m_pathToEntity == NULL || rand->nextInt(100) == 0) {
					int var10 = -1;
					int var2 = -1;
					int var3 = -1;
					var4 = -99999.0F;

					for(int var14 = 0; var14 < 200; ++var14) {
						int var16 = (int)(posX + (float)rand->nextInt(21) - 10.0F);
						int var17 = (int)(posY + (float)rand->nextInt(9) - 4.0F);
						int var18 = (int)(posZ + (float)rand->nextInt(21) - 10.0F);
						float var9 = getBlockPathWeight(var16, var17, var18);
						if(var9 > var4) {
							var4 = var9;
							var10 = var16;
							var2 = var17;
							var3 = var18;
						}
					}

					if(var10 > 0) {
						m_pathToEntity = worldObj->m_pathFinder->createEntityPathTo(this, var10, var2, var3, 16.0F);
					}
				}
			} else {
				m_pathToEntity = worldObj->m_pathFinder->createEntityPathTo(this, m_playerToAttack, 16.0F);
			}

			bool var11 = handleWaterMovement();
			bool var12 = handleLavaMovement();
			if(m_pathToEntity == NULL) {
				if(var11 || var12) {
					isJumping = rand->nextFloat() < 0.8F;
				}

			} else {
				Vec3D* var13 = m_pathToEntity->getPosition(this);
				var4 = width * 2.0F;

				float var15;
				while(var13 != NULL) {
					var8 = posZ;
					var7 = posY;
					var6 = posX;
					var6 -= var13->m_xCoord;
					var7 -= var13->m_yCoord;
					var15 = var8 - var13->m_zCoord;
					if(var6 * var6 + var7 * var7 + var15 * var15 >= var4 * var4 || var13->m_yCoord > posY) {
						break;
					}

					m_pathToEntity->incrementPathIndex();
					if(m_pathToEntity->isFinished()) {
						var13 = NULL;
						m_pathToEntity = NULL;
					} else {
						var13 = m_pathToEntity->getPosition(this);
					}
				}

				isJumping = false;
				if(var13 != NULL) {
					var15 = var13->m_xCoord - posX;
					var6 = var13->m_zCoord - posZ;
					var7 = var13->m_yCoord - posY;
					rotationYaw = (float)(atan2f((double)var6, (double)var15) * 180.0f / (double)(3.14159265358979323846f)) - 90.0F;
					moveForward = moveSpeed;
					if(var7 > 0.0F) {
						isJumping = true;
					}
				}

				if(var11 || var12) {
					isJumping = rand->nextFloat() < 0.8F;
				}

			}
		}
}

void EntityCreature::attackEntity(Entity* var1, float var2) {
	// TODO: implement
}

float EntityCreature::getBlockPathWeight(int var1, int var2, int var3) {
		return 0.0F;
}

Entity* EntityCreature::findPlayerToAttack() {
		return NULL;
}

bool EntityCreature::getCanSpawnHere(float var1, float var2, float var3) {
		return EntityLiving::getCanSpawnHere(var1, var2, var3) && getBlockPathWeight((int)var1, (int)var2, (int)var3) >= 0.0F;
}
