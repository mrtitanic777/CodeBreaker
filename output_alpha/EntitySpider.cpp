#include "stdafx.h"
// Auto-generated from EntitySpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySpider.h"
#include "Common.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityMobs.h"
#include "Item.h"

EntitySpider::EntitySpider()
{
}

EntitySpider::EntitySpider(World* var1)
	: EntityMobs(var1)
{
		texture = "/mob/spider.png";
		setSize(1.4F, 0.9F);
		m_field_9333_am = 0.8F;
}

EntitySpider::~EntitySpider() {
	// No dynamic resources
}

double EntitySpider::func_402_h() {
		return (double)height * 0.75f - 0.5f;
}

Entity* EntitySpider::findPlayerToAttack() {
		float var1 = getEntityBrightness(1.0F);
		if(var1 < 0.5F) {
			double var2 = 16.0f;
			return (Entity*)worldObj->getClosestPlayerToEntity(this, var2);
		} else {
			return NULL;
		}
}

const std::string& EntitySpider::getLivingSound() {
		static const std::string s("mob.spider");
		return s;
}

const std::string& EntitySpider::getHurtSound() {
		static const std::string s("mob.spider");
		return s;
}

const std::string& EntitySpider::getDeathSound() {
		static const std::string s("mob.spiderdeath");
		return s;
}

void EntitySpider::attackEntity(Entity* var1, float var2) {
		float var3 = getEntityBrightness(1.0F);
		if(var3 > 0.5F && rand->nextInt(100) == 0) {
			m_playerToAttack = NULL;
		} else {
			if(var2 > 2.0F && var2 < 6.0F && rand->nextInt(10) == 0) {
				if(onGround) {
					double var4 = var1->posX - posX;
					double var6 = var1->posZ - posZ;
					float var8 = sqrt(var4 * var4 + var6 * var6);
					motionX = var4 / (double)var8 * 0.5f * (double)0.8F + motionX * (double)0.2F;
					motionZ = var6 / (double)var8 * 0.5f * (double)0.8F + motionZ * (double)0.2F;
					motionY = (double)0.4F;
				}
			} else {
				EntityMobs::attackEntity(var1, var2);
			}

		}
}

void EntitySpider::writeEntityToNBT(NBTTagCompound* var1) {
		EntityMobs::writeEntityToNBT(var1);
}

void EntitySpider::readEntityFromNBT(NBTTagCompound* var1) {
		EntityMobs::readEntityFromNBT(var1);
}

int EntitySpider::getDropItemId() {
		return Item::s_silk->shiftedIndex;
}
