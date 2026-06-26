#include "stdafx.h"
// Auto-generated from EntityFireball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFireball.h"
#include "Common.h"
#include "EntityLiving.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "MovingObjectPosition.h"
#include "Vec3D.h"

EntityFireball::EntityFireball()
	: m_field_9402_e(-1)
	, m_field_9401_f(-1)
	, m_field_9400_g(-1)
	, m_field_9399_h(0)
	, m_field_9398_i(false)
	, m_field_9406_a(0)
	, m_field_9397_j(NULL)
	, m_field_9396_k(0)
	, m_field_9395_l(0)
	, m_field_9405_b(0)
	, m_field_9404_c(0)
	, m_field_9403_d(0)
{
}

EntityFireball::EntityFireball(World* var1)
	: Entity(var1)
	, m_field_9402_e(-1)
	, m_field_9401_f(-1)
	, m_field_9400_g(-1)
	, m_field_9399_h(0)
	, m_field_9398_i(false)
	, m_field_9406_a(0)
	, m_field_9397_j(NULL)
	, m_field_9396_k(0)
	, m_field_9395_l(0)
	, m_field_9405_b(0)
	, m_field_9404_c(0)
	, m_field_9403_d(0)
{
		setSize(1.0F, 1.0F);
}

EntityFireball::EntityFireball(World* var1, EntityLiving* var2, double var3, double var5, double var7)
	: Entity(var1)
	, m_field_9402_e(-1)
	, m_field_9401_f(-1)
	, m_field_9400_g(-1)
	, m_field_9399_h(0)
	, m_field_9398_i(false)
	, m_field_9406_a(0)
	, m_field_9397_j(NULL)
	, m_field_9396_k(0)
	, m_field_9395_l(0)
	, m_field_9405_b(0)
	, m_field_9404_c(0)
	, m_field_9403_d(0)
{
		m_field_9397_j = var2;
		setSize(1.0F, 1.0F);
		setLocationAndAngles(var2->posX, var2->posY, var2->posZ, var2->rotationYaw, var2->rotationPitch);
		setPosition(posX, posY, posZ);
		yOffset = 0.0F;
		motionX = motionY = motionZ = 0.0f;
		var3 += rand->nextGaussian() * 0.4f;
		var5 += rand->nextGaussian() * 0.4f;
		var7 += rand->nextGaussian() * 0.4f;
		double var9 = (double)sqrt(var3 * var3 + var5 * var5 + var7 * var7);
		m_field_9405_b = var3 / var9 * 0.1f;
		m_field_9404_c = var5 / var9 * 0.1f;
		m_field_9403_d = var7 / var9 * 0.1f;
}

EntityFireball::~EntityFireball() {
	m_field_9397_j = NULL;
}

bool EntityFireball::func_384_a(double var1) {
		double var3 = boundingBox.getAverageEdgeLength() * 4.0f;
		var3 *= 64.0f;
		return var1 < var3 * var3;
}

void EntityFireball::onUpdate() {
		Entity::onUpdate();
		fire = 10;
		if(m_field_9406_a > 0) {
			--m_field_9406_a;
		}

		if(m_field_9398_i) {
			int var1 = worldObj->getBlockId(m_field_9402_e, m_field_9401_f, m_field_9400_g);
			if(var1 == m_field_9399_h) {
				++m_field_9396_k;
				if(m_field_9396_k == 1200) {
					setEntityDead();
				}

				return;
			}

			m_field_9398_i = false;
			motionX *= (double)(rand->nextFloat() * 0.2F);
			motionY *= (double)(rand->nextFloat() * 0.2F);
			motionZ *= (double)(rand->nextFloat() * 0.2F);
			m_field_9396_k = 0;
			m_field_9395_l = 0;
		} else {
			++m_field_9395_l;
		}

		Vec3D* var15 = Vec3D::createVector(posX, posY, posZ);
		Vec3D* var2 = Vec3D::createVector(posX + motionX, posY + motionY, posZ + motionZ);
		MovingObjectPosition* var3 = worldObj->rayTraceBlocks(var15, var2);
		var15 = Vec3D::createVector(posX, posY, posZ);
		var2 = Vec3D::createVector(posX + motionX, posY + motionY, posZ + motionZ);
		if(var3 != NULL) {
			var2 = Vec3D::createVector(var3->m_hitVec->xCoord, var3->m_hitVec->yCoord, var3->m_hitVec->zCoord);
		}

		Entity* var4 = NULL;
		std::vector<void*> var5 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox.addCoord(motionX, motionY, motionZ).expands(1.0f, 1.0f, 1.0f));
		double var6 = 0.0f;

		for(size_t var8 = 0; var8 < var5.size(); ++var8) {
			Entity* var9 = (Entity*)var5[var8];
			if(var9->canBeCollidedWith() && (var9 != m_field_9397_j || m_field_9395_l >= 25)) {
				float var10 = 0.3F;
				AxisAlignedBB var11 = var9->boundingBox.expands((double)var10, (double)var10, (double)var10);
				MovingObjectPosition* var12 = var11.func_1169_a(var15, var2);
				if(var12 != NULL) {
					double var13 = var15->distanceTo(var12->m_hitVec);
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
			if(var3->m_entityHit != NULL && var3->m_entityHit->canAttackEntity(m_field_9397_j, 0)) {
			}

			worldObj->func_12244_a(NULL, posX, posY, posZ, 1.0F, true);
			setEntityDead();
		}

		posX += motionX;
		posY += motionY;
		posZ += motionZ;
		float var16 = sqrt(motionX * motionX + motionZ * motionZ);
		rotationYaw = (float)(atan2f(motionX, motionZ) * 180.0f / (double)(3.14159265358979323846f));

		for(rotationPitch = (float)(atan2f(motionY, (double)var16) * 180.0f / (double)(3.14159265358979323846f)); rotationPitch - prevRotationPitch < -180.0F; prevRotationPitch -= 360.0F) {
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
		float var17 = 0.95F;
		if(handleWaterMovement()) {
			for(int var18 = 0; var18 < 4; ++var18) {
				float var19 = 0.25F;
				worldObj->spawnParticle("bubble", posX - motionX * (double)var19, posY - motionY * (double)var19, posZ - motionZ * (double)var19, motionX, motionY, motionZ);
			}

			var17 = 0.8F;
		}

		motionX += m_field_9405_b;
		motionY += m_field_9404_c;
		motionZ += m_field_9403_d;
		motionX *= (double)var17;
		motionY *= (double)var17;
		motionZ *= (double)var17;
		worldObj->spawnParticle("smoke", posX, posY + 0.5f, posZ, 0.0f, 0.0f, 0.0f);
		setPosition(posX, posY, posZ);
}

void EntityFireball::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setShort("xTile", (short)m_field_9402_e);
		var1->setShort("yTile", (short)m_field_9401_f);
		var1->setShort("zTile", (short)m_field_9400_g);
		var1->setByte("inTile", (unsigned char)m_field_9399_h);
		var1->setByte("shake", (unsigned char)m_field_9406_a);
		var1->setByte("inGround", (unsigned char)(m_field_9398_i ? 1 : 0));
}

void EntityFireball::readEntityFromNBT(NBTTagCompound* var1) {
		m_field_9402_e = var1->getShort("xTile");
		m_field_9401_f = var1->getShort("yTile");
		m_field_9400_g = var1->getShort("zTile");
		m_field_9399_h = var1->getByte("inTile") & 255;
		m_field_9406_a = var1->getByte("shake") & 255;
		m_field_9398_i = var1->getByte("inGround") == 1;
}

bool EntityFireball::canBeCollidedWith() {
		return true;
}

float EntityFireball::func_4035_j_() {
		return 1.0F;
}

bool EntityFireball::canAttackEntity(Entity* var1, int var2) {
		func_9281_M();
		if(var1 != NULL) {
			Vec3D* var3 = var1->func_4037_H();
			if(var3 != NULL) {
				motionX = var3->xCoord;
				motionY = var3->yCoord;
				motionZ = var3->zCoord;
				m_field_9405_b = motionX * 0.1f;
				m_field_9404_c = motionY * 0.1f;
				m_field_9403_d = motionZ * 0.1f;
			}

			return true;
		} else {
			return false;
		}
}

float EntityFireball::func_392_h_() {
		return 0.0F;
}
