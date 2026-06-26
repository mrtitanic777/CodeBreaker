#include "stdafx.h"
// Auto-generated from EntityTNT.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityTNT.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "Entity.h"

EntityTNT::EntityTNT()
	: m_fuse(0)
{
}

EntityTNT::EntityTNT(World* var1, float var2, float var3, float var4)
	: Entity(var1)
	, m_fuse(0)
{
		preventEntitySpawning = true;
		setSize(0.98F, 0.98F);
		yOffset = height / 2.0F;
		setPosition(var2, var3, var4);
		float var5 = (float)(((float)::rand() / (float)RAND_MAX) * (double)(3.14159265358979323846f) * 2.0f);
		motionX = -sinf(var5 * 3.14159265358979323846f / 180.0F) * 0.02F;
		motionY = 0.2F;
		motionZ = -cosf(var5 * 3.14159265358979323846f / 180.0F) * 0.02F;
		canTriggerWalking = false;
		m_fuse = 80;
		prevPosX = var2;
		prevPosY = var3;
		prevPosZ = var4;
}

EntityTNT::~EntityTNT() {
	// No dynamic resources
}

bool EntityTNT::canBeCollidedWith() {
		return !isDead;
}

void EntityTNT::onEntityUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		motionY -= 0.04F;
		moveEntity(motionX, motionY, motionZ);
		motionX *= 0.98F;
		motionY *= 0.98F;
		motionZ *= 0.98F;
		if(onGround) {
			motionX *= 0.7F;
			motionZ *= 0.7F;
			motionY *= -0.5F;
		}

		if(m_fuse-- <= 0) {
			setEntityDead();
			worldObj->createExplosion((Entity)NULL, posX, posY, posZ, 4.0F);
		} else {
			worldObj->spawnParticle("smoke", posX, posY + 0.5F, posZ, 0.0F, 0.0F, 0.0F);
		}
}

void EntityTNT::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setByte("Fuse", (unsigned char)m_fuse);
}

void EntityTNT::readEntityFromNBT(NBTTagCompound* var1) {
		m_fuse = var1->getByte("Fuse");
}

const std::string& EntityTNT::getEntityString() {
		return "PrimedTnt";
}

float EntityTNT::getShadowSize() {
		return 0.0F;
}
