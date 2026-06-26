#include "stdafx.h"
// Auto-generated from EntitySpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySpider.h"
#include "Common.h"
#include "Entity.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityMob.h"

EntitySpider::EntitySpider()
{
}

EntitySpider::EntitySpider(World* var1)
	: EntityMob(var1)
{
		texture = "/mob/spider.png";
		setSize(1.4F, 0.9F);
		moveSpeed = 0.8F;
}

EntitySpider::~EntitySpider() {
	// No dynamic resources
}

Entity* EntitySpider::findPlayerToAttack() {
		float var1 = getEntityBrightness(1.0F);
		if(var1 < 0.5F) {
			var1 = worldObj->m_playerEntity->getDistanceSqToEntity(this);
			if(var1 < 256.0F) {
				return worldObj->m_playerEntity;
			}
		}

		return NULL;
}

void EntitySpider::attackEntity(Entity* var1, float var2) {
		if(var2 > 2.0F && var2 < 6.0F && rand->nextInt(10) == 0) {
			if(onGround) {
				var2 = var1->posX - posX;
				float var4 = var1->posZ - posZ;
				float var3 = sqrtf(var2 * var2 + var4 * var4);
				motionX = var2 / var3 * 0.5F * 0.8F + motionX * 0.2F;
				motionZ = var4 / var3 * 0.5F * 0.8F + motionZ * 0.2F;
				motionY = 0.4F;
				return;
			}
		} else {
			EntityMob::attackEntity(var1, var2);
		}

}

void EntitySpider::writeEntityToNBT(NBTTagCompound* var1) {
		EntityMob::writeEntityToNBT(var1);
}

void EntitySpider::readEntityFromNBT(NBTTagCompound* var1) {
		EntityMob::readEntityFromNBT(var1);
}

const std::string& EntitySpider::getEntityString() {
		return "Spider";
}
