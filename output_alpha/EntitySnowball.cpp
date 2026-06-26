#include "stdafx.h"
// Auto-generated from EntitySnowball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySnowball.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "Item.h"
#include "ItemStack.h"
#include "MovingObjectPosition.h"
#include "Vec3D.h"
#include "InventoryPlayer.h"

EntitySnowball::EntitySnowball()
	: m_field_816_b(-1)
	, m_field_815_c(-1)
	, m_field_814_d(-1)
	, m_field_813_e(0)
	, m_field_812_f(false)
	, m_field_817_a(0)
	, m_field_811_g(NULL)
	, m_field_810_h(0)
	, m_field_809_i(0)
{
}

EntitySnowball::EntitySnowball(World* var1)
	: Entity(var1)
	, m_field_816_b(-1)
	, m_field_815_c(-1)
	, m_field_814_d(-1)
	, m_field_813_e(0)
	, m_field_812_f(false)
	, m_field_817_a(0)
	, m_field_811_g(NULL)
	, m_field_810_h(0)
	, m_field_809_i(0)
{
		setSize(0.25F, 0.25F);
}

EntitySnowball::EntitySnowball(World* var1, EntityLiving* var2)
	: Entity(var1)
	, m_field_816_b(-1)
	, m_field_815_c(-1)
	, m_field_814_d(-1)
	, m_field_813_e(0)
	, m_field_812_f(false)
	, m_field_817_a(0)
	, m_field_811_g(NULL)
	, m_field_810_h(0)
	, m_field_809_i(0)
{
		m_field_811_g = var2;
		setSize(0.25F, 0.25F);
		setLocationAndAngles(var2->posX, var2->posY + (double)var2->func_373_s(), var2->posZ, var2->rotationYaw, var2->rotationPitch);
		posX -= (double)(cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * 0.16F);
		posY -= (double)0.1F;
		posZ -= (double)(sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * 0.16F);
		setPosition(posX, posY, posZ);
		yOffset = 0.0F;
		float var3 = 0.4F;
		motionX = (double)(-sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f) * var3);
		motionZ = (double)(cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f) * var3);
		motionY = (double)(-sinf(rotationPitch / 180.0F * 3.14159265358979323846f) * var3);
		func_467_a(motionX, motionY, motionZ, 1.5F, 1.0F);
}

EntitySnowball::EntitySnowball(World* var1, double var2, double var4, double var6)
	: Entity(var1)
	, m_field_816_b(-1)
	, m_field_815_c(-1)
	, m_field_814_d(-1)
	, m_field_813_e(0)
	, m_field_812_f(false)
	, m_field_817_a(0)
	, m_field_811_g(NULL)
	, m_field_810_h(0)
	, m_field_809_i(0)
{
		m_field_810_h = 0;
		setSize(0.25F, 0.25F);
		setPosition(var2, var4, var6);
		yOffset = 0.0F;
}

EntitySnowball::~EntitySnowball() {
	m_field_811_g = NULL;
}

bool EntitySnowball::func_384_a(double var1) {
		double var3 = boundingBox.getAverageEdgeLength() * 4.0f;
		var3 *= 64.0f;
		return var1 < var3 * var3;
}

void EntitySnowball::func_467_a(double var1, double var3, double var5, float var7, float var8) {
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
		m_field_810_h = 0;
}

void EntitySnowball::setVelocity(double var1, double var3, double var5) {
		motionX = var1;
		motionY = var3;
		motionZ = var5;
		if(prevRotationPitch == 0.0F && prevRotationYaw == 0.0F) {
			float var7 = sqrt(var1 * var1 + var5 * var5);
			prevRotationYaw = rotationYaw = (float)(atan2f(var1, var5) * 180.0f / (double)(3.14159265358979323846f));
			prevRotationPitch = rotationPitch = (float)(atan2f(var3, (double)var7) * 180.0f / (double)(3.14159265358979323846f));
		}

}

void EntitySnowball::onUpdate() {
		lastTickPosX = posX;
		lastTickPosY = posY;
		lastTickPosZ = posZ;
		Entity::onUpdate();
		if(m_field_817_a > 0) {
			--m_field_817_a;
		}

		if(m_field_812_f) {
			int var1 = worldObj->getBlockId(m_field_816_b, m_field_815_c, m_field_814_d);
			if(var1 == m_field_813_e) {
				++m_field_810_h;
				if(m_field_810_h == 1200) {
					setEntityDead();
				}

				return;
			}

			m_field_812_f = false;
			motionX *= (double)(rand->nextFloat() * 0.2F);
			motionY *= (double)(rand->nextFloat() * 0.2F);
			motionZ *= (double)(rand->nextFloat() * 0.2F);
			m_field_810_h = 0;
			m_field_809_i = 0;
		} else {
			++m_field_809_i;
		}

		Vec3D* var15 = Vec3D::createVector(posX, posY, posZ);
		Vec3D* var2 = Vec3D::createVector(posX + motionX, posY + motionY, posZ + motionZ);
		MovingObjectPosition* var3 = worldObj->rayTraceBlocks(var15, var2);
		var15 = Vec3D::createVector(posX, posY, posZ);
		var2 = Vec3D::createVector(posX + motionX, posY + motionY, posZ + motionZ);
		if(var3 != NULL) {
			var2 = Vec3D::createVector(var3->m_hitVec->xCoord, var3->m_hitVec->yCoord, var3->m_hitVec->zCoord);
		}

		if(!worldObj->m_multiplayerWorld) {
			Entity* var4 = NULL;
			std::vector<void*> var5 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox.addCoord(motionX, motionY, motionZ).expands(1.0f, 1.0f, 1.0f));
			double var6 = 0.0f;

			for(size_t var8 = 0; var8 < var5.size(); ++var8) {
				Entity* var9 = (Entity*)var5[var8];
				if(var9->canBeCollidedWith() && (var9 != m_field_811_g || m_field_809_i >= 5)) {
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
		}

		if(var3 != NULL) {
			if(var3->m_entityHit != NULL && var3->m_entityHit->canAttackEntity(m_field_811_g, 0)) {
			}

			for(int var16 = 0; var16 < 8; ++var16) {
				worldObj->spawnParticle("snowballpoof", posX, posY, posZ, 0.0f, 0.0f, 0.0f);
			}

			setEntityDead();
		}

		posX += motionX;
		posY += motionY;
		posZ += motionZ;
		float var17 = sqrt(motionX * motionX + motionZ * motionZ);
		rotationYaw = (float)(atan2f(motionX, motionZ) * 180.0f / (double)(3.14159265358979323846f));

		for(rotationPitch = (float)(atan2f(motionY, (double)var17) * 180.0f / (double)(3.14159265358979323846f)); rotationPitch - prevRotationPitch < -180.0F; prevRotationPitch -= 360.0F) {
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
		float var18 = 0.99F;
		float var19 = 0.03F;
		if(handleWaterMovement()) {
			for(int var7 = 0; var7 < 4; ++var7) {
				float var20 = 0.25F;
				worldObj->spawnParticle("bubble", posX - motionX * (double)var20, posY - motionY * (double)var20, posZ - motionZ * (double)var20, motionX, motionY, motionZ);
			}

			var18 = 0.8F;
		}

		motionX *= (double)var18;
		motionY *= (double)var18;
		motionZ *= (double)var18;
		motionY -= (double)var19;
		setPosition(posX, posY, posZ);
}

void EntitySnowball::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setShort("xTile", (short)m_field_816_b);
		var1->setShort("yTile", (short)m_field_815_c);
		var1->setShort("zTile", (short)m_field_814_d);
		var1->setByte("inTile", (unsigned char)m_field_813_e);
		var1->setByte("shake", (unsigned char)m_field_817_a);
		var1->setByte("inGround", (unsigned char)(m_field_812_f ? 1 : 0));
}

void EntitySnowball::readEntityFromNBT(NBTTagCompound* var1) {
		m_field_816_b = var1->getShort("xTile");
		m_field_815_c = var1->getShort("yTile");
		m_field_814_d = var1->getShort("zTile");
		m_field_813_e = var1->getByte("inTile") & 255;
		m_field_817_a = var1->getByte("shake") & 255;
		m_field_812_f = var1->getByte("inGround") == 1;
}

void EntitySnowball::onCollideWithPlayer(EntityPlayer* var1) {
		if(m_field_812_f && m_field_811_g == var1 && m_field_817_a <= 0 && var1->m_inventory->addItemStackToInventory(new ItemStack(Item::s_arrow->shiftedIndex, 1))) {
			worldObj->playSoundAtEntity(this, "random.pop", 0.2F, ((rand->nextFloat() - rand->nextFloat()) * 0.7F + 1.0F) * 2.0F);
			var1->func_443_a_(this, 1);
			setEntityDead();
		}

}

float EntitySnowball::func_392_h_() {
		return 0.0F;
}
