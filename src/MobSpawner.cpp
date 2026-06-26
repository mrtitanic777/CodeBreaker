#include "stdafx.h"
// Auto-generated from MobSpawner.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MobSpawner.h"
#include "Common.h"
#include "World.h"
#include "EntityCreeper.h"
#include "EntityPig.h"
#include "EntitySheep.h"
#include "EntitySkeleton.h"
#include "EntitySpider.h"
#include "EntityZombie.h"

MobSpawner::MobSpawner()
	: worldObj(NULL)
{
}

MobSpawner::MobSpawner(World* var1)
	: worldObj(NULL)
{
		worldObj = var1;
}

MobSpawner::~MobSpawner() {
	worldObj = NULL;
}

void MobSpawner::performSpawning() {
		int var1 = worldObj->width * worldObj /* PORT: array length - use tracked size */ * worldObj->height * 20 / 64 / 64 / 64;
		var1 /= 2;
		if(worldObj->m_difficultySetting == 0) {
			var1 = 0 / 4;
		}

		if(worldObj->m_difficultySetting == 1) {
			var1 = var1 * 3 / 4;
		}

		if(worldObj->m_difficultySetting == 2) {
			var1 = (var1 << 2) / 4;
		}

		if(worldObj->m_difficultySetting == 3) {
			var1 = var1 * 6 / 4;
		}

		int var2 = worldObj->width * worldObj /* PORT: array length - use tracked size */ / 4000;
		// PORT-BLOCKED: int var3 = worldObj->entitiesInLevelList(EntityMob.class);

		int var4;
		EntityLiving var5;
		int var6;
		int var7;
		int var8;
		int var9;
		int var10;
		int var11;
		int var12;
		int var13;
		int var14;
		int var15;
		float var16;
		float var17;
		float var18;
		float var19;
		float var20;
		float var21;
		MobSpawner* var22;
		void* var23;
		for(var4 = 0; var4 < 4; ++var4) {
			if(var3 < var1) {
				var5 = worldObj->m_playerEntity;
				var22 = this;
				var6 = 0;
				var7 = worldObj->m_random->nextInt(5);
				var8 = worldObj->m_random->nextInt(worldObj->width);
				var9 = (int)(min(worldObj->m_random->nextFloat(), worldObj->m_random->nextFloat()) * (float)worldObj->height);
				var10 = worldObj->m_random->nextInt(worldObj /* PORT: array length - use tracked size */);

				for(var11 = 0; var11 < 2; ++var11) {
					var12 = var8;
					var13 = var9;
					var14 = var10;

					for(var15 = 0; var15 < 3; ++var15) {
						var12 += var22->worldObj->m_random->nextInt(6) - var22->worldObj->m_random->nextInt(6);
						var13 += var22->worldObj->m_random->nextInt(1) - var22->worldObj->m_random->nextInt(1);
						var14 += var22->worldObj->m_random->nextInt(6) - var22->worldObj->m_random->nextInt(6);
						if(var12 >= 0 && var14 > 0 && var13 >= 0 && var13 < var22->worldObj->height - 2 && var12 < var22->worldObj->width && var14 < var22->worldObj /* PORT: array length - use tracked size */) {
							var16 = (float)var12 + 0.5F;
							var17 = (float)var13 + 0.5F;
							var18 = (float)var14 + 0.5F;
							if(var5 != NULL) {
								var19 = var16 - var5.posX;
								var20 = var17 - var5.posY;
								var21 = var18 - var5.posZ;
								var19 = var19 * var19 + var20 * var20 + var21 * var21;
								if(var19 < 1024.0F) {
									continue;
								}
							} else {
								var19 = var16 - (float)var22->worldObj->m_xSpawn;
								var20 = var17 - (float)var22->worldObj->m_ySpawn;
								var21 = var18 - (float)var22->worldObj->m_zSpawn;
								var19 = var19 * var19 + var20 * var20 + var21 * var21;
								if(var19 < 1024.0F) {
									continue;
								}
							}

							var23 = NULL;
							if(var7 == 0) {
								var23 = new EntitySkeleton(var22->worldObj);
							}

							if(var7 == 1) {
								var23 = new EntityCreeper(var22->worldObj);
							}

							if(var7 == 2) {
								var23 = new EntitySpider(var22->worldObj);
							}

							if(var7 == 3) {
								var23 = new EntityZombie(var22->worldObj);
							}

							if(dynamic_cast<EntityMob*>(var23) != NULL && var22->worldObj->m_difficultySetting == 0) {
								var23 = NULL;
							}

							if(var23 != NULL && !var22->worldObj->isBlockNormalCube(var12, var13, var14) && var22->worldObj->isBlockNormalCube(var12, var13 - 1, var14) && ((EntityLiving*)var23).getCanSpawnHere(var16, var17, var18)) {
								var21 = var22->worldObj->m_random->nextFloat() * 360.0F;
								((EntityLiving*)var23).setPositionAndRotation(var16, var17, var18, var21, 0.0F);
								++var6;
								var22->worldObj->spawnEntityInWorld((Entity*)var23);
							}
						}
					}
				}

				var3 += var6;
			}
		}

		// PORT-BLOCKED: var4 = worldObj->entitiesInLevelList(EntityAnimal.class);

		for(var1 = 0; var1 < 4; ++var1) {
			if(var4 < var2) {
				var5 = worldObj->m_playerEntity;
				var22 = this;
				var6 = 0;
				var7 = worldObj->m_random->nextInt(2);
				var8 = worldObj->m_random->nextInt(worldObj->width);
				var9 = worldObj->m_random->nextInt(worldObj->height);
				var10 = worldObj->m_random->nextInt(worldObj /* PORT: array length - use tracked size */);

				for(var11 = 0; var11 < 2; ++var11) {
					var12 = var8;
					var13 = var9;
					var14 = var10;

					for(var15 = 0; var15 < 3; ++var15) {
						var12 += var22->worldObj->m_random->nextInt(6) - var22->worldObj->m_random->nextInt(6);
						var13 += var22->worldObj->m_random->nextInt(1) - var22->worldObj->m_random->nextInt(1);
						var14 += var22->worldObj->m_random->nextInt(6) - var22->worldObj->m_random->nextInt(6);
						if(var12 >= 0 && var14 > 0 && var13 >= 0 && var13 < var22->worldObj->height - 2 && var12 < var22->worldObj->width && var14 < var22->worldObj /* PORT: array length - use tracked size */) {
							var16 = (float)var12 + 0.5F;
							var17 = (float)var13 + 0.5F;
							var18 = (float)var14 + 0.5F;
							if(var5 != NULL) {
								var19 = var16 - var5.posX;
								var20 = var17 - var5.posY;
								var21 = var18 - var5.posZ;
								var19 = var19 * var19 + var20 * var20 + var21 * var21;
								if(var19 < 1024.0F) {
									continue;
								}
							} else {
								var19 = var16 - (float)var22->worldObj->m_xSpawn;
								var20 = var17 - (float)var22->worldObj->m_ySpawn;
								var21 = var18 - (float)var22->worldObj->m_zSpawn;
								var19 = var19 * var19 + var20 * var20 + var21 * var21;
								if(var19 < 1024.0F) {
									continue;
								}
							}

							var23 = NULL;
							if(var7 == 0) {
								var23 = new EntityPig(var22->worldObj);
							}

							if(var7 == 1) {
								var23 = new EntitySheep(var22->worldObj);
							}

							if(var23 != NULL && !var22->worldObj->isBlockNormalCube(var12, var13, var14) && var22->worldObj->isBlockNormalCube(var12, var13 - 1, var14) && ((EntityLiving*)var23).getCanSpawnHere(var16, var17, var18)) {
								var21 = var22->worldObj->m_random->nextFloat() * 360.0F;
								((EntityLiving*)var23).setPositionAndRotation(var16, var17, var18, var21, 0.0F);
								++var6;
								var22->worldObj->spawnEntityInWorld((Entity*)var23);
							}
						}
					}
				}

				var4 += var6;
			}
		}

}
