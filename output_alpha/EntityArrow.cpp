#include "stdafx.h"
// Auto-generated from EntityArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityArrow.h"
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

EntityArrow::EntityArrow()
	: m_xTile(-1)
	, m_yTile(-1)
	, m_zTile(-1)
	, m_inTile(0)
	, m_inGround(false)
	, m_arrowShake(0)
	, m_field_682_g(NULL)
	, m_field_681_h(0)
	, m_field_680_i(0)
{
}

EntityArrow::EntityArrow(World* var1)
	: Entity(var1)
	, m_xTile(-1)
	, m_yTile(-1)
	, m_zTile(-1)
	, m_inTile(0)
	, m_inGround(false)
	, m_arrowShake(0)
	, m_field_682_g(NULL)
	, m_field_681_h(0)
	, m_field_680_i(0)
{
		setSize(0.5F, 0.5F);
}

EntityArrow::EntityArrow(World* var1, double var2, double var4, double var6)
	: Entity(var1)
	, m_xTile(-1)
	, m_yTile(-1)
	, m_zTile(-1)
	, m_inTile(0)
	, m_inGround(false)
	, m_arrowShake(0)
	, m_field_682_g(NULL)
	, m_field_681_h(0)
	, m_field_680_i(0)
{
		setSize(0.5F, 0.5F);
		setPosition(var2, var4, var6);
		yOffset = 0.0F;
}

EntityArrow::EntityArrow(World* var1, EntityLiving* var2)
	: Entity(var1)
	, m_xTile(-1)
	, m_yTile(-1)
	, m_zTile(-1)
	, m_inTile(0)
	, m_inGround(false)
	, m_arrowShake(0)
	, m_field_682_g(NULL)
	, m_field_681_h(0)
	, m_field_680_i(0)
{
		m_field_682_g = var2;
		setSize(0.5F, 0.5F);
		setLocationAndAngles(var2->posX, var2->posY + (double)var2->func_373_s(), var2->posZ, var2->rotationYaw, var2->rotationPitch);
		posX -= (double)(cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * 0.16F);
		posY -= (double)0.1F;
		posZ -= (double)(sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * 0.16F);
		setPosition(posX, posY, posZ);
		yOffset = 0.0F;
		motionX = (double)(-sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f));
		motionZ = (double)(cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f));
		motionY = (double)(-sinf(rotationPitch / 180.0F * 3.14159265358979323846f));
		func_408_a(motionX, motionY, motionZ, 1.5F, 1.0F);
}

EntityArrow::~EntityArrow() {
	m_field_682_g = NULL;
}

void EntityArrow::func_408_a(double var1, double var3, double var5, float var7, float var8) {
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
		m_field_681_h = 0;
}

void EntityArrow::setVelocity(double var1, double var3, double var5) {
		motionX = var1;
		motionY = var3;
		motionZ = var5;
		if(prevRotationPitch == 0.0F && prevRotationYaw == 0.0F) {
			float var7 = sqrt(var1 * var1 + var5 * var5);
			prevRotationYaw = rotationYaw = (float)(atan2f(var1, var5) * 180.0f / (double)(3.14159265358979323846f));
			prevRotationPitch = rotationPitch = (float)(atan2f(var3, (double)var7) * 180.0f / (double)(3.14159265358979323846f));
		}

}

void EntityArrow::onUpdate() {
		Entity::onUpdate();
		if(prevRotationPitch == 0.0F && prevRotationYaw == 0.0F) {
			float var1 = sqrt(motionX * motionX + motionZ * motionZ);
			prevRotationYaw = rotationYaw = (float)(atan2f(motionX, motionZ) * 180.0f / (double)(3.14159265358979323846f));
			prevRotationPitch = rotationPitch = (float)(atan2f(motionY, (double)var1) * 180.0f / (double)(3.14159265358979323846f));
		}

		if(m_arrowShake > 0) {
			--m_arrowShake;
		}

		if(m_inGround) {
			int var15 = worldObj->getBlockId(m_xTile, m_yTile, m_zTile);
			if(var15 == m_inTile) {
				++m_field_681_h;
				if(m_field_681_h == 1200) {
					setEntityDead();
				}

				return;
			}

			m_inGround = false;
			motionX *= (double)(rand->nextFloat() * 0.2F);
			motionY *= (double)(rand->nextFloat() * 0.2F);
			motionZ *= (double)(rand->nextFloat() * 0.2F);
			m_field_681_h = 0;
			m_field_680_i = 0;
		} else {
			++m_field_680_i;
		}

		Vec3D* var16 = Vec3D::createVector(posX, posY, posZ);
		Vec3D* var2 = Vec3D::createVector(posX + motionX, posY + motionY, posZ + motionZ);
		MovingObjectPosition* var3 = worldObj->rayTraceBlocks(var16, var2);
		var16 = Vec3D::createVector(posX, posY, posZ);
		var2 = Vec3D::createVector(posX + motionX, posY + motionY, posZ + motionZ);
		if(var3 != NULL) {
			var2 = Vec3D::createVector(var3->m_hitVec->xCoord, var3->m_hitVec->yCoord, var3->m_hitVec->zCoord);
		}

		Entity* var4 = NULL;
		std::vector<void*> var5 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox.addCoord(motionX, motionY, motionZ).expands(1.0f, 1.0f, 1.0f));
		double var6 = 0.0f;

		float var10;
		for(size_t var8 = 0; var8 < var5.size(); ++var8) {
			Entity* var9 = (Entity*)var5[var8];
			if(var9->canBeCollidedWith() && (var9 != m_field_682_g || m_field_680_i >= 5)) {
				var10 = 0.3F;
				AxisAlignedBB var11 = var9->boundingBox.expands((double)var10, (double)var10, (double)var10);
				MovingObjectPosition* var12 = var11.func_1169_a(var16, var2);
				if(var12 != NULL) {
					double var13 = var16->distanceTo(var12->m_hitVec);
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

		float var17;
		if(var3 != NULL) {
			if(var3->m_entityHit != NULL) {
				if(var3->m_entityHit->canAttackEntity(m_field_682_g, 4)) {
					worldObj->playSoundAtEntity(this, "random.drr", 1.0F, 1.2F / (rand->nextFloat() * 0.2F + 0.9F));
					setEntityDead();
				} else {
					motionX *= (double)-0.1F;
					motionY *= (double)-0.1F;
					motionZ *= (double)-0.1F;
					rotationYaw += 180.0F;
					prevRotationYaw += 180.0F;
					m_field_680_i = 0;
				}
			} else {
				m_xTile = var3->m_blockX;
				m_yTile = var3->m_blockY;
				m_zTile = var3->m_blockZ;
				m_inTile = worldObj->getBlockId(m_xTile, m_yTile, m_zTile);
				motionX = (double)((float)(var3->m_hitVec->xCoord - posX));
				motionY = (double)((float)(var3->m_hitVec->yCoord - posY));
				motionZ = (double)((float)(var3->m_hitVec->zCoord - posZ));
				var17 = sqrt(motionX * motionX + motionY * motionY + motionZ * motionZ);
				posX -= motionX / (double)var17 * (double)0.05F;
				posY -= motionY / (double)var17 * (double)0.05F;
				posZ -= motionZ / (double)var17 * (double)0.05F;
				worldObj->playSoundAtEntity(this, "random.drr", 1.0F, 1.2F / (rand->nextFloat() * 0.2F + 0.9F));
				m_inGround = true;
				m_arrowShake = 7;
			}
		}

		posX += motionX;
		posY += motionY;
		posZ += motionZ;
		var17 = sqrt(motionX * motionX + motionZ * motionZ);
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
		var10 = 0.03F;
		if(handleWaterMovement()) {
			for(int var19 = 0; var19 < 4; ++var19) {
				float var20 = 0.25F;
				worldObj->spawnParticle("bubble", posX - motionX * (double)var20, posY - motionY * (double)var20, posZ - motionZ * (double)var20, motionX, motionY, motionZ);
			}

			var18 = 0.8F;
		}

		motionX *= (double)var18;
		motionY *= (double)var18;
		motionZ *= (double)var18;
		motionY -= (double)var10;
		setPosition(posX, posY, posZ);
}

void EntityArrow::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setShort("m_xTile", (short)m_xTile);
		var1->setShort("m_yTile", (short)m_yTile);
		var1->setShort("m_zTile", (short)m_zTile);
		var1->setByte("m_inTile", (unsigned char)m_inTile);
		var1->setByte("shake", (unsigned char)m_arrowShake);
		var1->setByte("m_inGround", (unsigned char)(m_inGround ? 1 : 0));
}

void EntityArrow::readEntityFromNBT(NBTTagCompound* var1) {
		m_xTile = var1->getShort("m_xTile");
		m_yTile = var1->getShort("m_yTile");
		m_zTile = var1->getShort("m_zTile");
		m_inTile = var1->getByte("m_inTile") & 255;
		m_arrowShake = var1->getByte("shake") & 255;
		m_inGround = var1->getByte("m_inGround") == 1;
}

void EntityArrow::onCollideWithPlayer(EntityPlayer* var1) {
		if(!worldObj->m_multiplayerWorld) {
			if(m_inGround && m_field_682_g == var1 && m_arrowShake <= 0 && var1->m_inventory->addItemStackToInventory(new ItemStack(Item::s_arrow->shiftedIndex, 1))) {
				worldObj->playSoundAtEntity(this, "random.pop", 0.2F, ((rand->nextFloat() - rand->nextFloat()) * 0.7F + 1.0F) * 2.0F);
				var1->func_443_a_(this, 1);
				setEntityDead();
			}

		}
}

float EntityArrow::func_392_h_() {
		return 0.0F;
}
