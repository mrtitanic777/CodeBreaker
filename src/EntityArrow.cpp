#include "stdafx.h"
// Auto-generated from EntityArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityArrow.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "Item.h"
#include "ItemStack.h"
#include "MovingObjectPosition.h"
#include "Vec3.h"
#include "EntityMap.h"
#include "InventoryPlayer.h"

EntityArrow::EntityArrow()
	: m_xTile(-1)
	, m_yTile(-1)
	, m_zTile(-1)
	, m_inTile(0)
	, m_inGround(false)
	, m_arrowShake(0)
	, m_owner(NULL)
	, m_ticksInGround(0)
	, m_ticksInAir(0)
{
}

EntityArrow::EntityArrow(World* var1, EntityLiving* var2)
	: Entity(var1)
	, m_xTile(-1)
	, m_yTile(-1)
	, m_zTile(-1)
	, m_inTile(0)
	, m_inGround(false)
	, m_arrowShake(0)
	, m_owner(NULL)
	, m_ticksInGround(0)
	, m_ticksInAir(0)
{
		m_owner = var2;
		setSize(0.5F, 0.5F);
		setPositionAndRotation(var2->posX, var2->posY, var2->posZ, var2->rotationYaw, var2->rotationPitch);
		posX += cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * 0.16F;
		posY -= 0.1F;
		posZ += sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * 0.16F;
		setPosition(posX, posY, posZ);
		yOffset = 0.0F;
		motionX = sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f);
		motionZ = -cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f);
		motionY = -sinf(rotationPitch / 180.0F * 3.14159265358979323846f);
		setArrowHeading(motionX, motionY, motionZ, 1.5F, 1.0F);
}

EntityArrow::~EntityArrow() {
	m_owner = NULL;
}

void EntityArrow::setArrowHeading(float var1, float var2, float var3, float var4, float var5) {
		float var6 = sqrtf(var1 * var1 + var2 * var2 + var3 * var3);
		var1 /= var6;
		var2 /= var6;
		var3 /= var6;
		var1 = (float)((double)var1 + rand->nextGaussian() * (double)0.0075F * (double)var5);
		var2 = (float)((double)var2 + rand->nextGaussian() * (double)0.0075F * (double)var5);
		var3 = (float)((double)var3 + rand->nextGaussian() * (double)0.0075F * (double)var5);
		var1 *= var4;
		var2 *= var4;
		var3 *= var4;
		motionX = var1;
		motionY = var2;
		motionZ = var3;
		var4 = sqrtf(var1 * var1 + var3 * var3);
		prevRotationYaw = rotationYaw = (float)(atan2f((double)var1, (double)var3) * 180.0f / (double)(3.14159265358979323846f));
		prevRotationPitch = rotationPitch = (float)(atan2f((double)var2, (double)var4) * 180.0f / (double)(3.14159265358979323846f));
		m_ticksInGround = 0;
}

void EntityArrow::onEntityUpdate() {
		Entity::onEntityUpdate();
		if(m_arrowShake > 0) {
			--m_arrowShake;
		}

		if(m_inGround) {
			int var1 = worldObj->getBlockId(m_xTile, m_yTile, m_zTile);
			if(var1 == m_inTile) {
				++m_ticksInGround;
				if(m_ticksInGround == 1200) {
					setEntityDead();
				}

				return;
			}

			m_inGround = false;
			motionX *= rand->nextFloat() * 0.2F;
			motionY *= rand->nextFloat() * 0.2F;
			motionZ *= rand->nextFloat() * 0.2F;
			m_ticksInGround = 0;
			m_ticksInAir = 0;
		} else {
			++m_ticksInAir;
		}

		Vec3D var10 = new Vec3D(posX, posY, posZ);
		Vec3D var2 = new Vec3D(posX + motionX, posY + motionY, posZ + motionZ);
		MovingObjectPosition var3 = worldObj->rayTraceBlocks(var10, var2);
		var10 = new Vec3D(posX, posY, posZ);
		var2 = new Vec3D(posX + motionX, posY + motionY, posZ + motionZ);
		if(var3 != NULL) {
			var2 = new Vec3D(var3.m_hitVec.m_xCoord, var3.m_hitVec.m_yCoord, var3.m_hitVec.m_zCoord);
		}

		Entity var4 = NULL;
		std::vector<Entity> var5 = worldObj->m_entityMap.a(this, boundingBox.addCoord(motionX, motionY, motionZ).expand(1.0F, 1.0F, 1.0F));
		float var6 = 0.0F;

		for(size_t var7 = 0; var7 < var5.size(); ++var7) {
			Entity var8 = (Entity*)var5[var7];
			if(var8.canBeCollidedWith() && (var8 != m_owner || m_ticksInAir >= 5)) {
				AxisAlignedBB var9 = var8.boundingBox.expand(0.3F, 0.3F, 0.3F);
				MovingObjectPosition var13 = var9.calculateIntercept(var10, var2);
				if(var13 != NULL) {
					float var14 = var10.distance(var13.m_hitVec);
					if(var14 < var6 || var6 == 0.0F) {
						var4 = var8;
						var6 = var14;
					}
				}
			}
		}

		if(var4 != NULL) {
			var3 = MovingObjectPosition(var4);
		}

		float var11;
		if(var3 != NULL) {
			if(var3.m_entityHit != NULL) {
				if(var3.m_entityHit.attackEntityFrom(this, 4)) {
					worldObj->a(this, "random.drr", 1.0F, 1.2F / (rand->nextFloat() * 0.2F + 0.9F));
					setEntityDead();
				} else {
					motionX *= -0.1F;
					motionY *= -0.1F;
					motionZ *= -0.1F;
					rotationYaw += 180.0F;
					prevRotationYaw += 180.0F;
					m_ticksInAir = 0;
				}
			} else {
				m_xTile = var3.m_blockX;
				m_yTile = var3.m_blockY;
				m_zTile = var3.m_blockZ;
				m_inTile = worldObj->getBlockId(m_xTile, m_yTile, m_zTile);
				motionX = var3.m_hitVec.m_xCoord - posX;
				motionY = var3.m_hitVec.m_yCoord - posY;
				motionZ = var3.m_hitVec.m_zCoord - posZ;
				var11 = sqrtf(motionX * motionX + motionY * motionY + motionZ * motionZ);
				posX -= motionX / var11 * 0.05F;
				posY -= motionY / var11 * 0.05F;
				posZ -= motionZ / var11 * 0.05F;
				worldObj->a(this, "random.drr", 1.0F, 1.2F / (rand->nextFloat() * 0.2F + 0.9F));
				m_inGround = true;
				m_arrowShake = 7;
			}
		}

		posX += motionX;
		posY += motionY;
		posZ += motionZ;
		var11 = sqrtf(motionX * motionX + motionZ * motionZ);
		rotationYaw = (float)(atan2f((double)motionX, (double)motionZ) * 180.0f / (double)(3.14159265358979323846f));

		for(rotationPitch = (float)(atan2f((double)motionY, (double)var11) * 180.0f / (double)(3.14159265358979323846f)); rotationPitch - prevRotationPitch < -180.0F; prevRotationPitch -= 360.0F) {
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
		float var12 = 0.99F;
		if(handleWaterMovement()) {
			for(int var15 = 0; var15 < 4; ++var15) {
				worldObj->spawnParticle("bubble", posX - motionX * 0.25F, posY - motionY * 0.25F, posZ - motionZ * 0.25F, motionX, motionY, motionZ);
			}

			var12 = 0.8F;
		}

		motionX *= var12;
		motionY *= var12;
		motionZ *= var12;
		motionY -= 0.03F;
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

const std::string& EntityArrow::getEntityString() {
		return "Arrow";
}

void EntityArrow::onCollideWithPlayer(EntityPlayer* var1) {
		if(m_inGround && m_owner == var1 && m_arrowShake <= 0 && var1->m_inventory.storePartialItemStack(ItemStack(Item::s_arrow->shiftedIndex, 1))) {
			worldObj->a(this, "random.pop", 0.2F, ((rand->nextFloat() - rand->nextFloat()) * 0.7F + 1.0F) * 2.0F);
			var1->onItemPickup(this);
			setEntityDead();
		}

}

float EntityArrow::getShadowSize() {
		return 0.0F;
}
