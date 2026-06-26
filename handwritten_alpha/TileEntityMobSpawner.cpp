#include "stdafx.h"
// Auto-generated from TileEntityMobSpawner.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntityMobSpawner.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "EntityLiving.h"
// PORT: removed spurious "MobSpawner.h" (no such class)
#include "TileEntity.h"
#include "EntityList.h"
#include "World.h"  // PORT: worldObj-> calls (getClosestPlayer/rand/spawnParticle/...)

TileEntityMobSpawner::TileEntityMobSpawner()
	: m_delay(-1)
	, m_entityID("Pig")
	, m_field_831_c(0)
	, m_field_830_d(0.0f)
{
		m_delay = 20;
}

TileEntityMobSpawner::~TileEntityMobSpawner() {
	// No dynamic resources
}

bool TileEntityMobSpawner::anyPlayerInRange() {
		return worldObj->getClosestPlayer((double)xCoord + 0.5f, (double)yCoord + 0.5f, (double)zCoord + 0.5f, 16.0f) != NULL;
}

void TileEntityMobSpawner::updateEntity() {
		m_field_830_d = m_field_831_c;
		if(anyPlayerInRange()) {
			double var1 = (double)((float)xCoord + worldObj->rand->nextFloat());
			double var3 = (double)((float)yCoord + worldObj->rand->nextFloat());
			double var5 = (double)((float)zCoord + worldObj->rand->nextFloat());
			worldObj->spawnParticle("smoke", var1, var3, var5, 0.0f, 0.0f, 0.0f);
			worldObj->spawnParticle("flame", var1, var3, var5, 0.0f, 0.0f, 0.0f);

			for(m_field_831_c += (double)(1000.0F / ((float)m_delay + 200.0F)); m_field_831_c > 360.0f; m_field_830_d -= 360.0f) {
				m_field_831_c -= 360.0f;
			}

			if(m_delay == -1) {
				updateDelay();
			}

			if(m_delay > 0) {
				--m_delay;
			} else {
				unsigned char var7 = 4;

				for(int var8 = 0; var8 < var7; ++var8) {
					EntityLiving* var9 = (EntityLiving*)((EntityLiving*)EntityList::createEntityInWorld(m_entityID, worldObj));
					if(var9 == NULL) {
						return;
					}

					// PORT: Java passed var9.getClass() to filter by type. No RTTI class
					// token on Xbox 360; World/Chunk ignore the class arg and return all
					// nearby entities, so pass NULL (matches the existing port contract).
					int var10 = worldObj->getEntitiesWithinAABB(NULL, AxisAlignedBB::getBoundingBoxFromPool((double)xCoord, (double)yCoord, (double)zCoord, (double)(xCoord + 1), (double)(yCoord + 1), (double)(zCoord + 1)).expands(8.0f, 4.0f, 8.0f)).size();
					if(var10 >= 6) {
						updateDelay();
						return;
					}

					if(var9 != NULL) {
						double var11 = (double)xCoord + (worldObj->rand->nextDouble() - worldObj->rand->nextDouble()) * 4.0f;
						double var13 = (double)(yCoord + worldObj->rand->nextInt(3) - 1);
						double var15 = (double)zCoord + (worldObj->rand->nextDouble() - worldObj->rand->nextDouble()) * 4.0f;
						var9->setLocationAndAngles(var11, var13, var15, worldObj->rand->nextFloat() * 360.0F, 0.0F);
						if(var9->getCanSpawnHere()) {
							worldObj->entityJoinedWorld(var9);

							for(int var17 = 0; var17 < 20; ++var17) {
								var1 = (double)xCoord + 0.5f + ((double)worldObj->rand->nextFloat() - 0.5f) * 2.0f;
								var3 = (double)yCoord + 0.5f + ((double)worldObj->rand->nextFloat() - 0.5f) * 2.0f;
								var5 = (double)zCoord + 0.5f + ((double)worldObj->rand->nextFloat() - 0.5f) * 2.0f;
								worldObj->spawnParticle("smoke", var1, var3, var5, 0.0f, 0.0f, 0.0f);
								worldObj->spawnParticle("flame", var1, var3, var5, 0.0f, 0.0f, 0.0f);
							}

							var9->spawnExplosionParticle();
							updateDelay();
						}
					}
				}

				TileEntity::updateEntity();
			}
		}
}

void TileEntityMobSpawner::updateDelay() {
		m_delay = 200 + worldObj->rand->nextInt(600);
}

void TileEntityMobSpawner::readFromNBT(NBTTagCompound* var1) {
		TileEntity::readFromNBT(var1);
		m_entityID = var1->getString("EntityId");
		m_delay = var1->getShort("Delay");
}

void TileEntityMobSpawner::writeToNBT(NBTTagCompound* var1) {
		// PORT: base TileEntity::writeToNBT now needs an id (no reflection on 360);
		// write it explicitly then persist coords + spawner state.
		var1->setString("id", "MobSpawner");
		var1->setInteger("x", xCoord);
		var1->setInteger("y", yCoord);
		var1->setInteger("z", zCoord);
		var1->setString("EntityId", m_entityID);
		var1->setShort("Delay", (short)m_delay);
}
