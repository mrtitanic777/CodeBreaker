#include "stdafx.h"
// Auto-generated from EntityTNTPrimed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityTNTPrimed.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "Entity.h"

EntityTNTPrimed::EntityTNTPrimed()
	: m_fuse(0)
{
}

EntityTNTPrimed::EntityTNTPrimed(World* var1)
	: Entity(var1)
	, m_fuse(0)
{
		m_fuse = 0;
		m_field_618_ad = true;
		setSize(0.98F, 0.98F);
		yOffset = height / 2.0F;
}

EntityTNTPrimed::EntityTNTPrimed(World* var1, double var2, double var4, double var6)
	: Entity(var1)
	, m_fuse(0)
{
		// PORT: Java did this(var1); C++03 has no delegating ctors, so replicate
		// the World* ctor setup here.
		m_field_618_ad = true;
		setSize(0.98F, 0.98F);
		yOffset = height / 2.0F;

		setPosition(var2, var4, var6);
		float var8 = (float)(((float)::rand() / (float)RAND_MAX) * (double)(3.14159265358979323846f) * 2.0f);
		motionX = (double)(-sinf(var8 * 3.14159265358979323846f / 180.0F) * 0.02F);
		motionY = (double)0.2F;
		motionZ = (double)(-cosf(var8 * 3.14159265358979323846f / 180.0F) * 0.02F);
		m_entityWalks = false;
		m_fuse = 80;
		prevPosX = var2;
		prevPosY = var4;
		prevPosZ = var6;
}

EntityTNTPrimed::~EntityTNTPrimed() {
	// No dynamic resources
}

bool EntityTNTPrimed::canBeCollidedWith() {
		return !isDead;
}

void EntityTNTPrimed::onUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		motionY -= (double)0.04F;
		moveEntity(motionX, motionY, motionZ);
		motionX *= (double)0.98F;
		motionY *= (double)0.98F;
		motionZ *= (double)0.98F;
		if(onGround) {
			motionX *= (double)0.7F;
			motionZ *= (double)0.7F;
			motionY *= -0.5f;
		}

		if(m_fuse-- <= 0) {
			setEntityDead();
			explode();
		} else {
			worldObj->spawnParticle("smoke", posX, posY + 0.5f, posZ, 0.0f, 0.0f, 0.0f);
		}

}

void EntityTNTPrimed::explode() {
		float var1 = 4.0F;
		worldObj->func_12243_a(NULL, posX, posY, posZ, var1);
}

void EntityTNTPrimed::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setByte("Fuse", (unsigned char)m_fuse);
}

void EntityTNTPrimed::readEntityFromNBT(NBTTagCompound* var1) {
		m_fuse = var1->getByte("Fuse");
}

float EntityTNTPrimed::func_392_h_() {
		return 0.0F;
}
