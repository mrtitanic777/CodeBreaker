#include "stdafx.h"
// Auto-generated from EntityBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityBoat.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "Block.h"
#include "Entity.h"
#include "Item.h"
#include "Material.h"

EntityBoat::EntityBoat()
	: m_field_807_a(0)
	, m_field_806_b(0)
	, m_field_808_c(0)
	, m_field_9394_d(0)
	, m_field_9393_e(0)
	, m_field_9392_f(0)
	, m_field_9391_g(0)
	, m_field_9390_h(0)
	, m_field_9389_i(0)
	, m_field_9388_j(0)
	, m_field_9387_k(0)
	, m_field_9386_l(0)
{
}

EntityBoat::EntityBoat(World* var1)
	: Entity(var1)
	, m_field_807_a(0)
	, m_field_806_b(0)
	, m_field_808_c(0)
	, m_field_9394_d(0)
	, m_field_9393_e(0)
	, m_field_9392_f(0)
	, m_field_9391_g(0)
	, m_field_9390_h(0)
	, m_field_9389_i(0)
	, m_field_9388_j(0)
	, m_field_9387_k(0)
	, m_field_9386_l(0)
{
		m_field_807_a = 0;
		m_field_806_b = 0;
		m_field_808_c = 1;
		m_field_618_ad = true;
		setSize(1.5F, 0.6F);
		yOffset = height / 2.0F;
		m_entityWalks = false;
}

EntityBoat::EntityBoat(World* var1, double var2, double var4, double var6)
	: Entity(var1)
	, m_field_807_a(0)
	, m_field_806_b(0)
	, m_field_808_c(0)
	, m_field_9394_d(0)
	, m_field_9393_e(0)
	, m_field_9392_f(0)
	, m_field_9391_g(0)
	, m_field_9390_h(0)
	, m_field_9389_i(0)
	, m_field_9388_j(0)
	, m_field_9387_k(0)
	, m_field_9386_l(0)
{
		// PORT: Java did this(var1); replicate the World* ctor setup (no C++03 delegation).
		m_field_807_a = 0;
		m_field_806_b = 0;
		m_field_808_c = 1;
		m_field_618_ad = true;
		setSize(1.5F, 0.6F);
		yOffset = height / 2.0F;
		m_entityWalks = false;

		setPosition(var2, var4 + (double)yOffset, var6);
		motionX = 0.0f;
		motionY = 0.0f;
		motionZ = 0.0f;
		prevPosX = var2;
		prevPosY = var4;
		prevPosZ = var6;
}

EntityBoat::~EntityBoat() {
	// No dynamic resources
}

AxisAlignedBB EntityBoat::func_383_b_(Entity* var1) {
		return var1->boundingBox;
}

AxisAlignedBB EntityBoat::func_372_f_() {
		return boundingBox;
}

bool EntityBoat::canBePushed() {
		return true;
}

double EntityBoat::func_402_h() {
		return (double)height * 0.0f - (double)0.3F;
}

bool EntityBoat::canAttackEntity(Entity* var1, int var2) {
		if(!worldObj->m_multiplayerWorld && !isDead) {
			m_field_808_c = -m_field_808_c;
			m_field_806_b = 10;
			m_field_807_a += var2 * 10;
			func_9281_M();
			if(m_field_807_a > 40) {
				int var3;
				for(var3 = 0; var3 < 3; ++var3) {
					dropItemWithOffset(Block::s_planks->blockID, 1, 0.0F);
				}

				for(var3 = 0; var3 < 2; ++var3) {
					dropItemWithOffset(Item::s_stick->shiftedIndex, 1, 0.0F);
				}

				setEntityDead();
			}

			return true;
		} else {
			return true;
		}
}

void EntityBoat::func_9280_g() {
		m_field_808_c = -m_field_808_c;
		m_field_806_b = 10;
		m_field_807_a += m_field_807_a * 10;
}

bool EntityBoat::canBeCollidedWith() {
		return !isDead;
}

void EntityBoat::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9) {
		m_field_9393_e = var1;
		m_field_9392_f = var3;
		m_field_9391_g = var5;
		m_field_9390_h = (double)var7;
		m_field_9389_i = (double)var8;
		m_field_9394_d = var9 + 4;
		motionX = m_field_9388_j;
		motionY = m_field_9387_k;
		motionZ = m_field_9386_l;
}

void EntityBoat::setVelocity(double var1, double var3, double var5) {
		m_field_9388_j = motionX = var1;
		m_field_9387_k = motionY = var3;
		m_field_9386_l = motionZ = var5;
}

void EntityBoat::onUpdate() {
		Entity::onUpdate();
		if(m_field_806_b > 0) {
			--m_field_806_b;
		}

		if(m_field_807_a > 0) {
			--m_field_807_a;
		}

		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		unsigned char var1 = 5;
		double var2 = 0.0f;

		for(int var4 = 0; var4 < var1; ++var4) {
			double var5 = boundingBox.minY + (boundingBox.maxY - boundingBox.minY) * (double)(var4 + 0) / (double)var1 - 0.125f;
			double var7 = boundingBox.minY + (boundingBox.maxY - boundingBox.minY) * (double)(var4 + 1) / (double)var1 - 0.125f;
			AxisAlignedBB var9 = AxisAlignedBB::getBoundingBoxFromPool(boundingBox.minX, var5, boundingBox.minZ, boundingBox.maxX, var7, boundingBox.maxZ);
			if(worldObj->func_707_b(var9, Material::s_water)) {
				var2 += 1.0f / (double)var1;
			}
		}

		double var6;
		double var8;
		double var10;
		double var23;
		if(worldObj->m_multiplayerWorld) {
			if(m_field_9394_d > 0) {
				var23 = posX + (m_field_9393_e - posX) / (double)m_field_9394_d;
				var6 = posY + (m_field_9392_f - posY) / (double)m_field_9394_d;
				var8 = posZ + (m_field_9391_g - posZ) / (double)m_field_9394_d;

				for(var10 = m_field_9390_h - (double)rotationYaw; var10 < -180.0f; var10 += 360.0f) {
				}

				while(var10 >= 180.0f) {
					var10 -= 360.0f;
				}

				rotationYaw = (float)((double)rotationYaw + var10 / (double)m_field_9394_d);
				rotationPitch = (float)((double)rotationPitch + (m_field_9389_i - (double)rotationPitch) / (double)m_field_9394_d);
				--m_field_9394_d;
				setPosition(var23, var6, var8);
				setRotation(rotationYaw, rotationPitch);
			} else {
				var23 = posX + motionX;
				var6 = posY + motionY;
				var8 = posZ + motionZ;
				setPosition(var23, var6, var8);
				if(onGround) {
					motionX *= 0.5f;
					motionY *= 0.5f;
					motionZ *= 0.5f;
				}

				motionX *= (double)0.99F;
				motionY *= (double)0.95F;
				motionZ *= (double)0.99F;
			}

		} else {
			var23 = var2 * 2.0f - 1.0f;
			motionY += (double)0.04F * var23;
			if(m_riddenByEntity != NULL) {
				motionX += m_riddenByEntity->motionX * 0.2f;
				motionZ += m_riddenByEntity->motionZ * 0.2f;
			}

			var6 = 0.4f;
			if(motionX < -var6) {
				motionX = -var6;
			}

			if(motionX > var6) {
				motionX = var6;
			}

			if(motionZ < -var6) {
				motionZ = -var6;
			}

			if(motionZ > var6) {
				motionZ = var6;
			}

			if(onGround) {
				motionX *= 0.5f;
				motionY *= 0.5f;
				motionZ *= 0.5f;
			}

			moveEntity(motionX, motionY, motionZ);
			var8 = sqrtf(motionX * motionX + motionZ * motionZ);
			double var12;
			if(var8 > 0.15f) {
				var10 = cosf(rotationYaw * (3.14159265358979323846f / 180.0f));
				var12 = sinf(rotationYaw * (3.14159265358979323846f / 180.0f));

				for(int var14 = 0; (double)var14 < 1.0f + var8 * 60.0f; ++var14) {
					double var15 = (double)(rand->nextFloat() * 2.0F - 1.0F);
					double var17 = (double)(rand->nextInt(2) * 2 - 1) * 0.7f;
					double var19;
					double var21;
					if(rand->nextBoolean()) {
						var19 = posX - var10 * var15 * 0.8f + var12 * var17;
						var21 = posZ - var12 * var15 * 0.8f - var10 * var17;
						worldObj->spawnParticle("splash", var19, posY - 0.125f, var21, motionX, motionY, motionZ);
					} else {
						var19 = posX + var10 + var12 * var15 * 0.7f;
						var21 = posZ + var12 - var10 * var15 * 0.7f;
						worldObj->spawnParticle("splash", var19, posY - 0.125f, var21, motionX, motionY, motionZ);
					}
				}
			}

			if(m_field_9297_aI && var8 > 0.15f) {
				if(!worldObj->m_multiplayerWorld) {
					setEntityDead();

					int var24;
					for(var24 = 0; var24 < 3; ++var24) {
						dropItemWithOffset(Block::s_planks->blockID, 1, 0.0F);
					}

					for(var24 = 0; var24 < 2; ++var24) {
						dropItemWithOffset(Item::s_stick->shiftedIndex, 1, 0.0F);
					}
				}
			} else {
				motionX *= (double)0.99F;
				motionY *= (double)0.95F;
				motionZ *= (double)0.99F;
			}

			rotationPitch = 0.0F;
			var10 = (double)rotationYaw;
			var12 = prevPosX - posX;
			double var25 = prevPosZ - posZ;
			if(var12 * var12 + var25 * var25 > 0.001f) {
				var10 = (double)((float)(atan2f(var25, var12) * 180.0f / 3.14159265358979323846f));
			}

			double var16;
			for(var16 = var10 - (double)rotationYaw; var16 >= 180.0f; var16 -= 360.0f) {
			}

			while(var16 < -180.0f) {
				var16 += 360.0f;
			}

			if(var16 > 20.0f) {
				var16 = 20.0f;
			}

			if(var16 < -20.0f) {
				var16 = -20.0f;
			}

			rotationYaw = (float)((double)rotationYaw + var16);
			setRotation(rotationYaw, rotationPitch);
			std::vector<void*> var18 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox.expands((double)0.2F, 0.0f, (double)0.2F));
			if(true && var18.size() > 0) {
				for(size_t var26 = 0; var26 < var18.size(); ++var26) {
					Entity* var20 = (Entity*)var18[var26];
					if(var20 != m_riddenByEntity && var20->canBePushed() && dynamic_cast<EntityBoat*>(var20) != NULL) {
						var20->applyEntityCollision(this);
					}
				}
			}

			if(m_riddenByEntity != NULL && m_riddenByEntity->isDead) {
				m_riddenByEntity = NULL;
			}

		}
}

void EntityBoat::func_366_i_() {
		if(m_riddenByEntity != NULL) {
			double var1 = cosf(rotationYaw * (3.14159265358979323846f / 180.0f)) * 0.4f;
			double var3 = sinf(rotationYaw * (3.14159265358979323846f / 180.0f)) * 0.4f;
			m_riddenByEntity->setPosition(posX + var1, posY + func_402_h() + m_riddenByEntity->func_388_v(), posZ + var3);
		}
}

void EntityBoat::writeEntityToNBT(NBTTagCompound* var1) {
	// TODO: implement
}

void EntityBoat::readEntityFromNBT(NBTTagCompound* var1) {
	// TODO: implement
}

float EntityBoat::func_392_h_() {
		return 0.0F;
}

bool EntityBoat::interact(EntityPlayer* var1) {
		if(m_riddenByEntity != NULL && dynamic_cast<EntityPlayer*>(m_riddenByEntity) != NULL && m_riddenByEntity != var1) {
			return true;
		} else {
			if(!worldObj->m_multiplayerWorld) {
				var1->mountEntity(this);
			}

			return true;
		}
}
