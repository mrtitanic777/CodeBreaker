#include "stdafx.h"
// Auto-generated from EntityFish.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFish.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "EntityItem.h"
#include "ItemStack.h"
#include "Material.h"
#include "MovingObjectPosition.h"
#include "Vec3D.h"
#include "Item.h"

EntityFish::EntityFish()
	: m_field_4098_a(0)
	, m_field_4097_b(NULL)
	, m_field_4096_c(NULL)
	, m_field_4095_d(0)
	, m_field_4094_e(0)
	, m_field_4093_f(0)
	, m_field_4092_g(0)
	, m_field_4091_h(false)
	, m_field_4090_i(0)
	, m_field_4089_j(0)
	, m_field_4088_k(0)
	, m_field_6388_l(0)
	, m_field_6387_m(0)
	, m_field_6386_n(0)
	, m_field_6385_o(0)
	, m_field_6384_p(0)
	, m_field_6383_q(0)
	, m_velocityX(0)
	, m_velocityY(0)
	, m_velocityZ(0)
{
}

EntityFish::EntityFish(World* var1)
	: Entity(var1)
	, m_field_4098_a(0)
	, m_field_4097_b(NULL)
	, m_field_4096_c(NULL)
	, m_field_4095_d(0)
	, m_field_4094_e(0)
	, m_field_4093_f(0)
	, m_field_4092_g(0)
	, m_field_4091_h(false)
	, m_field_4090_i(0)
	, m_field_4089_j(0)
	, m_field_4088_k(0)
	, m_field_6388_l(0)
	, m_field_6387_m(0)
	, m_field_6386_n(0)
	, m_field_6385_o(0)
	, m_field_6384_p(0)
	, m_field_6383_q(0)
	, m_velocityX(0)
	, m_velocityY(0)
	, m_velocityZ(0)
{
		m_field_4095_d = -1;
		m_field_4094_e = -1;
		m_field_4093_f = -1;
		m_field_4092_g = 0;
		m_field_4091_h = false;
		m_field_4098_a = 0;
		m_field_4089_j = 0;
		m_field_4088_k = 0;
		m_field_4096_c = NULL;
		setSize(0.25F, 0.25F);
}

EntityFish::EntityFish(World* var1, double var2, double var4, double var6)
	: Entity(var1)
	, m_field_4098_a(0)
	, m_field_4097_b(NULL)
	, m_field_4096_c(NULL)
	, m_field_4095_d(0)
	, m_field_4094_e(0)
	, m_field_4093_f(0)
	, m_field_4092_g(0)
	, m_field_4091_h(false)
	, m_field_4090_i(0)
	, m_field_4089_j(0)
	, m_field_4088_k(0)
	, m_field_6388_l(0)
	, m_field_6387_m(0)
	, m_field_6386_n(0)
	, m_field_6385_o(0)
	, m_field_6384_p(0)
	, m_field_6383_q(0)
	, m_velocityX(0)
	, m_velocityY(0)
	, m_velocityZ(0)
{
		// PORT: Java did this(var1); replicate the World* ctor setup (no C++03 delegation).
		m_field_4095_d = -1;
		m_field_4094_e = -1;
		m_field_4093_f = -1;
		m_field_4092_g = 0;
		m_field_4091_h = false;
		m_field_4098_a = 0;
		m_field_4089_j = 0;
		m_field_4088_k = 0;
		m_field_4096_c = NULL;
		setSize(0.25F, 0.25F);

		setPosition(var2, var4, var6);
}

EntityFish::EntityFish(World* var1, EntityPlayer* var2)
	: Entity(var1)
	, m_field_4098_a(0)
	, m_field_4097_b(NULL)
	, m_field_4096_c(NULL)
	, m_field_4095_d(0)
	, m_field_4094_e(0)
	, m_field_4093_f(0)
	, m_field_4092_g(0)
	, m_field_4091_h(false)
	, m_field_4090_i(0)
	, m_field_4089_j(0)
	, m_field_4088_k(0)
	, m_field_6388_l(0)
	, m_field_6387_m(0)
	, m_field_6386_n(0)
	, m_field_6385_o(0)
	, m_field_6384_p(0)
	, m_field_6383_q(0)
	, m_velocityX(0)
	, m_velocityY(0)
	, m_velocityZ(0)
{
		m_field_4095_d = -1;
		m_field_4094_e = -1;
		m_field_4093_f = -1;
		m_field_4092_g = 0;
		m_field_4091_h = false;
		m_field_4098_a = 0;
		m_field_4089_j = 0;
		m_field_4088_k = 0;
		m_field_4096_c = NULL;
		m_field_4097_b = var2;
		m_field_4097_b->m_fishEntity = this;
		setSize(0.25F, 0.25F);
		setLocationAndAngles(var2->posX, var2->posY + 1.62f - (double)var2->yOffset, var2->posZ, var2->rotationYaw, var2->rotationPitch);
		posX -= (double)(cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * 0.16F);
		posY -= (double)0.1F;
		posZ -= (double)(sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * 0.16F);
		setPosition(posX, posY, posZ);
		yOffset = 0.0F;
		float var3 = 0.4F;
		motionX = (double)(-sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f) * var3);
		motionZ = (double)(cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f) * var3);
		motionY = (double)(-sinf(rotationPitch / 180.0F * 3.14159265358979323846f) * var3);
		func_4042_a(motionX, motionY, motionZ, 1.5F, 1.0F);
}

EntityFish::~EntityFish() {
	m_field_4097_b = NULL;
	m_field_4096_c = NULL;
}

bool EntityFish::func_384_a(double var1) {
		double var3 = boundingBox.getAverageEdgeLength() * 4.0f;
		var3 *= 64.0f;
		return var1 < var3 * var3;
}

void EntityFish::func_4042_a(double var1, double var3, double var5, float var7, float var8) {
		float var9 = sqrt(var1 * var1 + var3 * var3 + var5 * var5);
		var1 /= (double)var9;
		var3 /= (double)var9;
		var5 /= (double)var9;
		var1 += rand->nextGaussian() * (double)0.0075F * (double)var8;
		var3 += rand->nextGaussian() * (double)0.0075F * (double)var8;
		var5 += rand->nextGaussian() * (double)0.0075F * (double)var8;
		var1 *= (double)var7;
		var3 *= (double)var7;
		var5 *= (double)var7;
		motionX = var1;
		motionY = var3;
		motionZ = var5;
		float var10 = sqrt(var1 * var1 + var5 * var5);
		prevRotationYaw = rotationYaw = (float)(atan2f(var1, var5) * 180.0f / (double)(3.14159265358979323846f));
		prevRotationPitch = rotationPitch = (float)(atan2f(var3, (double)var10) * 180.0f / (double)(3.14159265358979323846f));
		m_field_4090_i = 0;
}

void EntityFish::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9) {
		m_field_6387_m = var1;
		m_field_6386_n = var3;
		m_field_6385_o = var5;
		m_field_6384_p = (double)var7;
		m_field_6383_q = (double)var8;
		m_field_6388_l = var9;
		motionX = m_velocityX;
		motionY = m_velocityY;
		motionZ = m_velocityZ;
}

void EntityFish::setVelocity(double var1, double var3, double var5) {
		m_velocityX = motionX = var1;
		m_velocityY = motionY = var3;
		m_velocityZ = motionZ = var5;
}

void EntityFish::onUpdate() {
		Entity::onUpdate();
		if(m_field_6388_l > 0) {
			double var21 = posX + (m_field_6387_m - posX) / (double)m_field_6388_l;
			double var22 = posY + (m_field_6386_n - posY) / (double)m_field_6388_l;
			double var23 = posZ + (m_field_6385_o - posZ) / (double)m_field_6388_l;

			double var7;
			for(var7 = m_field_6384_p - (double)rotationYaw; var7 < -180.0f; var7 += 360.0f) {
			}

			while(var7 >= 180.0f) {
				var7 -= 360.0f;
			}

			rotationYaw = (float)((double)rotationYaw + var7 / (double)m_field_6388_l);
			rotationPitch = (float)((double)rotationPitch + (m_field_6383_q - (double)rotationPitch) / (double)m_field_6388_l);
			--m_field_6388_l;
			setPosition(var21, var22, var23);
			setRotation(rotationYaw, rotationPitch);
		} else {
			if(!worldObj->m_multiplayerWorld) {
				ItemStack* var1 = m_field_4097_b->getCurrentEquippedItem();
				if(m_field_4097_b->isDead || !m_field_4097_b->isEntityAlive() || var1 == NULL || var1->getItem() != Item::s_fishingRod || getDistanceSqToEntity(m_field_4097_b) > 1024.0f) {
					setEntityDead();
					m_field_4097_b->m_fishEntity = NULL;
					return;
				}

				if(m_field_4096_c != NULL) {
					if(!m_field_4096_c->isDead) {
						posX = m_field_4096_c->posX;
						posY = m_field_4096_c->boundingBox.minY + (double)m_field_4096_c->height * 0.8f;
						posZ = m_field_4096_c->posZ;
						return;
					}

					m_field_4096_c = NULL;
				}
			}

			if(m_field_4098_a > 0) {
				--m_field_4098_a;
			}

			if(m_field_4091_h) {
				int var19 = worldObj->getBlockId(m_field_4095_d, m_field_4094_e, m_field_4093_f);
				if(var19 == m_field_4092_g) {
					++m_field_4090_i;
					if(m_field_4090_i == 1200) {
						setEntityDead();
					}

					return;
				}

				m_field_4091_h = false;
				motionX *= (double)(rand->nextFloat() * 0.2F);
				motionY *= (double)(rand->nextFloat() * 0.2F);
				motionZ *= (double)(rand->nextFloat() * 0.2F);
				m_field_4090_i = 0;
				m_field_4089_j = 0;
			} else {
				++m_field_4089_j;
			}

			Vec3D* var20 = Vec3D::createVector(posX, posY, posZ);
			Vec3D* var2 = Vec3D::createVector(posX + motionX, posY + motionY, posZ + motionZ);
			MovingObjectPosition* var3 = worldObj->rayTraceBlocks(var20, var2);
			var20 = Vec3D::createVector(posX, posY, posZ);
			var2 = Vec3D::createVector(posX + motionX, posY + motionY, posZ + motionZ);
			if(var3 != NULL) {
				var2 = Vec3D::createVector(var3->m_hitVec->xCoord, var3->m_hitVec->yCoord, var3->m_hitVec->zCoord);
			}

			Entity* var4 = NULL;
			std::vector<void*> var5 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox.addCoord(motionX, motionY, motionZ).expands(1.0f, 1.0f, 1.0f));
			double var6 = 0.0f;

			double var13;
			for(size_t var8 = 0; var8 < var5.size(); ++var8) {
				Entity* var9 = (Entity*)var5[var8];
				if(var9->canBeCollidedWith() && (var9 != m_field_4097_b || m_field_4089_j >= 5)) {
					float var10 = 0.3F;
					AxisAlignedBB var11 = var9->boundingBox.expands((double)var10, (double)var10, (double)var10);
					MovingObjectPosition* var12 = var11.func_1169_a(var20, var2);
					if(var12 != NULL) {
						var13 = var20->distanceTo(var12->m_hitVec);
						if(var13 < var6 || var6 == 0.0f) {
							var4 = var9;
							var6 = var13;
						}
					}
				}
			}

			if(var4 != NULL) {
				var3 = new MovingObjectPosition(var4);
			}

			if(var3 != NULL) {
				if(var3->m_entityHit != NULL) {
					if(var3->m_entityHit->canAttackEntity(m_field_4097_b, 0)) {
						m_field_4096_c = var3->m_entityHit;
					}
				} else {
					m_field_4091_h = true;
				}
			}

			if(!m_field_4091_h) {
				moveEntity(motionX, motionY, motionZ);
				float var24 = sqrt(motionX * motionX + motionZ * motionZ);
				rotationYaw = (float)(atan2f(motionX, motionZ) * 180.0f / (double)(3.14159265358979323846f));

				for(rotationPitch = (float)(atan2f(motionY, (double)var24) * 180.0f / (double)(3.14159265358979323846f)); rotationPitch - prevRotationPitch < -180.0F; prevRotationPitch -= 360.0F) {
				}

				while(rotationPitch - prevRotationPitch >= 180.0F) {
					prevRotationPitch += 360.0F;
				}

				while(rotationYaw - prevRotationYaw < -180.0F) {
					prevRotationYaw -= 360.0F;
				}

				while(rotationYaw - prevRotationYaw >= 180.0F) {
					prevRotationYaw += 360.0F;
				}

				rotationPitch = prevRotationPitch + (rotationPitch - prevRotationPitch) * 0.2F;
				rotationYaw = prevRotationYaw + (rotationYaw - prevRotationYaw) * 0.2F;
				float var25 = 0.92F;
				if(onGround || m_field_9297_aI) {
					var25 = 0.5F;
				}

				unsigned char var26 = 5;
				double var27 = 0.0f;

				for(int var28 = 0; var28 < var26; ++var28) {
					double var14 = boundingBox.minY + (boundingBox.maxY - boundingBox.minY) * (double)(var28 + 0) / (double)var26 - 0.125f + 0.125f;
					double var16 = boundingBox.minY + (boundingBox.maxY - boundingBox.minY) * (double)(var28 + 1) / (double)var26 - 0.125f + 0.125f;
					AxisAlignedBB var18 = AxisAlignedBB::getBoundingBoxFromPool(boundingBox.minX, var14, boundingBox.minZ, boundingBox.maxX, var16, boundingBox.maxZ);
					if(worldObj->func_707_b(var18, Material::s_water)) {
						var27 += 1.0f / (double)var26;
					}
				}

				if(var27 > 0.0f) {
					if(m_field_4088_k > 0) {
						--m_field_4088_k;
					} else if(rand->nextInt(500) == 0) {
						m_field_4088_k = rand->nextInt(30) + 10;
						motionY -= (double)0.2F;
						worldObj->playSoundAtEntity(this, "random.splash", 0.25F, 1.0F + (rand->nextFloat() - rand->nextFloat()) * 0.4F);
						float var29 = (float)(int)floor(boundingBox.minY);

						float var15;
						int var30;
						float var31;
						for(var30 = 0; (float)var30 < 1.0F + width * 20.0F; ++var30) {
							var15 = (rand->nextFloat() * 2.0F - 1.0F) * width;
							var31 = (rand->nextFloat() * 2.0F - 1.0F) * width;
							worldObj->spawnParticle("bubble", posX + (double)var15, (double)(var29 + 1.0F), posZ + (double)var31, motionX, motionY - (double)(rand->nextFloat() * 0.2F), motionZ);
						}

						for(var30 = 0; (float)var30 < 1.0F + width * 20.0F; ++var30) {
							var15 = (rand->nextFloat() * 2.0F - 1.0F) * width;
							var31 = (rand->nextFloat() * 2.0F - 1.0F) * width;
							worldObj->spawnParticle("splash", posX + (double)var15, (double)(var29 + 1.0F), posZ + (double)var31, motionX, motionY, motionZ);
						}
					}
				}

				if(m_field_4088_k > 0) {
					motionY -= (double)(rand->nextFloat() * rand->nextFloat() * rand->nextFloat()) * 0.2f;
				}

				var13 = var27 * 2.0f - 1.0f;
				motionY += (double)0.04F * var13;
				if(var27 > 0.0f) {
					var25 = (float)((double)var25 * 0.9f);
					motionY *= 0.8f;
				}

				motionX *= (double)var25;
				motionY *= (double)var25;
				motionZ *= (double)var25;
				setPosition(posX, posY, posZ);
			}
		}
}

void EntityFish::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setShort("xTile", (short)m_field_4095_d);
		var1->setShort("yTile", (short)m_field_4094_e);
		var1->setShort("zTile", (short)m_field_4093_f);
		var1->setByte("inTile", (unsigned char)m_field_4092_g);
		var1->setByte("shake", (unsigned char)m_field_4098_a);
		var1->setByte("inGround", (unsigned char)(m_field_4091_h ? 1 : 0));
}

void EntityFish::readEntityFromNBT(NBTTagCompound* var1) {
		m_field_4095_d = var1->getShort("xTile");
		m_field_4094_e = var1->getShort("yTile");
		m_field_4093_f = var1->getShort("zTile");
		m_field_4092_g = var1->getByte("inTile") & 255;
		m_field_4098_a = var1->getByte("shake") & 255;
		m_field_4091_h = var1->getByte("inGround") == 1;
}

float EntityFish::func_392_h_() {
		return 0.0F;
}

int EntityFish::func_4043_i() {
		unsigned char var1 = 0;
		if(m_field_4096_c != NULL) {
			double var2 = m_field_4097_b->posX - posX;
			double var4 = m_field_4097_b->posY - posY;
			double var6 = m_field_4097_b->posZ - posZ;
			double var8 = (double)sqrt(var2 * var2 + var4 * var4 + var6 * var6);
			double var10 = 0.1f;
			m_field_4096_c->motionX += var2 * var10;
			m_field_4096_c->motionY += var4 * var10 + (double)sqrt(var8) * 0.08f;
			m_field_4096_c->motionZ += var6 * var10;
			var1 = 3;
		} else if(m_field_4088_k > 0) {
			EntityItem* var13 = new EntityItem(worldObj, posX, posY, posZ, new ItemStack(Item::s_fishRaw->shiftedIndex));
			double var3 = m_field_4097_b->posX - posX;
			double var5 = m_field_4097_b->posY - posY;
			double var7 = m_field_4097_b->posZ - posZ;
			double var9 = (double)sqrt(var3 * var3 + var5 * var5 + var7 * var7);
			double var11 = 0.1f;
			var13->motionX = var3 * var11;
			var13->motionY = var5 * var11 + (double)sqrt(var9) * 0.08f;
			var13->motionZ = var7 * var11;
			worldObj->entityJoinedWorld(var13);
			var1 = 1;
		}

		if(m_field_4091_h) {
			var1 = 2;
		}

		setEntityDead();
		m_field_4097_b->m_fishEntity = NULL;
		return var1;
}
