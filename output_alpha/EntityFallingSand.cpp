#include "stdafx.h"
// Auto-generated from EntityFallingSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFallingSand.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "Entity.h"

EntityFallingSand::EntityFallingSand()
	: m_field_799_a(0)
	, m_field_798_b(0)
{
}

EntityFallingSand::EntityFallingSand(World* var1)
	: Entity(var1)
	, m_field_799_a(0)
	, m_field_798_b(0)
{
}

EntityFallingSand::EntityFallingSand(World* var1, float var2, float var3, float var4, int var5)
	: Entity(var1)
	, m_field_799_a(0)
	, m_field_798_b(0)
{
		m_field_799_a = var5;
		m_field_618_ad = true;
		setSize(0.98F, 0.98F);
		yOffset = height / 2.0F;
		setPosition((double)var2, (double)var3, (double)var4);
		motionX = 0.0f;
		motionY = 0.0f;
		motionZ = 0.0f;
		m_entityWalks = false;
		prevPosX = (double)var2;
		prevPosY = (double)var3;
		prevPosZ = (double)var4;
}

EntityFallingSand::~EntityFallingSand() {
	// No dynamic resources
}

bool EntityFallingSand::canBeCollidedWith() {
		return !isDead;
}

void EntityFallingSand::onUpdate() {
		if(m_field_799_a == 0) {
			setEntityDead();
		} else {
			prevPosX = posX;
			prevPosY = posY;
			prevPosZ = posZ;
			++m_field_798_b;
			motionY -= (double)0.04F;
			moveEntity(motionX, motionY, motionZ);
			motionX *= (double)0.98F;
			motionY *= (double)0.98F;
			motionZ *= (double)0.98F;
			int var1 = (int)floor(posX);
			int var2 = (int)floor(posY);
			int var3 = (int)floor(posZ);
			if(worldObj->getBlockId(var1, var2, var3) == m_field_799_a) {
				worldObj->setBlockWithNotify(var1, var2, var3, 0);
			}

			if(onGround) {
				motionX *= (double)0.7F;
				motionZ *= (double)0.7F;
				motionY *= -0.5f;
				setEntityDead();
				if(!worldObj->canBlockBePlacedAt(m_field_799_a, var1, var2, var3, true) || !worldObj->setBlockWithNotify(var1, var2, var3, m_field_799_a)) {
					dropItem(m_field_799_a, 1);
				}
			} else if(m_field_798_b > 100) {
				dropItem(m_field_799_a, 1);
				setEntityDead();
			}

		}
}

void EntityFallingSand::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setByte("Tile", (unsigned char)m_field_799_a);
}

void EntityFallingSand::readEntityFromNBT(NBTTagCompound* var1) {
		m_field_799_a = var1->getByte("Tile") & 255;
}

float EntityFallingSand::func_392_h_() {
		return 0.0F;
}

World* EntityFallingSand::func_465_i() {
		return worldObj;
}
