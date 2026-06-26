#include "stdafx.h"
// Auto-generated from EntitySkeleton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySkeleton.h"
#include "Common.h"
#include "Entity.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityArrow.h"
#include "EntityMob.h"

EntitySkeleton::EntitySkeleton()
{
}

EntitySkeleton::EntitySkeleton(World* var1)
	: EntityMob(var1)
{
		texture = "/mob/skeleton.png";
}

EntitySkeleton::~EntitySkeleton() {
	// No dynamic resources
}

void EntitySkeleton::onLivingUpdate() {
		float var1 = getEntityBrightness(1.0F);
		if(var1 > 0.5F && worldObj->canBlockSeeTheSky((int)posX, (int)posY, (int)posZ) && rand->nextFloat() * 40.0F < (var1 - 0.5F) * 2.0F) {
			fire = 300;
		}

		EntityMob::onLivingUpdate();
}

void EntitySkeleton::attackEntity(Entity* var1, float var2) {
		if(var2 < 10.0F) {
			var2 = var1->posX - posX;
			float var3 = var1->posZ - posZ;
			if(attackTime == 0) {
				EntityArrow* var4 = new EntityArrow(worldObj, this);
				++var4->posY;
				float var6 = var1->posY - 0.2F - var4->posY;
				float var5 = sqrtf(var2 * var2 + var3 * var3) * 0.2F;
				worldObj->a(this, "random.bow", 1.0F, 1.0F / (rand->nextFloat() * 0.4F + 0.8F));
				worldObj->spawnEntityInWorld(var4);
				var4->setArrowHeading(var2, var6 + var5, var3, 0.6F, 12.0F);
				attackTime = 30;
			}

			rotationYaw = (float)(atan2f((double)var3, (double)var2) * 180.0f / (double)(3.14159265358979323846f)) - 90.0F;
			m_hasAttacked = true;
		}

}

void EntitySkeleton::writeEntityToNBT(NBTTagCompound* var1) {
		EntityMob::writeEntityToNBT(var1);
}

void EntitySkeleton::readEntityFromNBT(NBTTagCompound* var1) {
		EntityMob::readEntityFromNBT(var1);
}

const std::string& EntitySkeleton::getEntityString() {
		return "Skeleton";
}
