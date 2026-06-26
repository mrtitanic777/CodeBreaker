#include "stdafx.h"
// Auto-generated from EntitySkeleton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySkeleton.h"
#include "Common.h"
#include "Entity.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityArrow.h"
#include "Item.h"
#include "EntityMobs.h"

ItemStack* EntitySkeleton::s_defaultHeldItem = new ItemStack(Item::s_bow, 1);

EntitySkeleton::EntitySkeleton()
{
}

EntitySkeleton::EntitySkeleton(World* var1)
	: EntityMobs(var1)
{
		texture = "/mob/skeleton.png";
}

EntitySkeleton::~EntitySkeleton() {
	// No dynamic resources
}

const std::string& EntitySkeleton::getLivingSound() {
		static const std::string _ret = "mob.skeleton"; return _ret;
}

const std::string& EntitySkeleton::getHurtSound() {
		static const std::string _ret = "mob.skeletonhurt"; return _ret;
}

const std::string& EntitySkeleton::getDeathSound() {
		static const std::string _ret = "mob.skeletonhurt"; return _ret;
}

void EntitySkeleton::onLivingUpdate() {
		if(worldObj->isDaytime()) {
			float var1 = getEntityBrightness(1.0F);
			if(var1 > 0.5F && worldObj->canBlockSeeTheSky((int)floor(posX), (int)floor(posY), (int)floor(posZ)) && rand->nextFloat() * 30.0F < (var1 - 0.4F) * 2.0F) {
				fire = 300;
			}
		}

		EntityMobs::onLivingUpdate();
}

void EntitySkeleton::attackEntity(Entity* var1, float var2) {
		if(var2 < 10.0F) {
			double var3 = var1->posX - posX;
			double var5 = var1->posZ - posZ;
			if(attackTime == 0) {
				EntityArrow* var7 = new EntityArrow(worldObj, this);
				var7->posY += (double)1.4F;
				double var8 = var1->posY - (double)0.2F - var7->posY;
				float var10 = sqrt(var3 * var3 + var5 * var5) * 0.2F;
				worldObj->playSoundAtEntity(this, "random.bow", 1.0F, 1.0F / (rand->nextFloat() * 0.4F + 0.8F));
				worldObj->entityJoinedWorld(var7);
				var7->func_408_a(var3, var8 + (double)var10, var5, 0.6F, 12.0F);
				attackTime = 30;
			}

			rotationYaw = (float)(atan2f(var5, var3) * 180.0f / (double)(3.14159265358979323846f)) - 90.0F;
			m_hasAttacked = true;
		}

}

void EntitySkeleton::writeEntityToNBT(NBTTagCompound* var1) {
		EntityMobs::writeEntityToNBT(var1);
}

void EntitySkeleton::readEntityFromNBT(NBTTagCompound* var1) {
		EntityMobs::readEntityFromNBT(var1);
}

int EntitySkeleton::getDropItemId() {
		return Item::s_arrow->shiftedIndex;
}

ItemStack* EntitySkeleton::getHeldItem() {
		return s_defaultHeldItem;
}
