#include "stdafx.h"
// Auto-generated from EntityBubbleFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityBubbleFX.h"
#include "Common.h"
#include "World.h"
#include "EntityFX.h"
#include "Material.h"

EntityBubbleFX::EntityBubbleFX()
{
}

EntityBubbleFX::EntityBubbleFX(World* var1, float var2, float var3, float var4, float var5, float var6, float var7)
	: EntityFX(var1, var2, var3, var4, var5, var6, var7)
{
		particleRed = 1.0F;
		particleGreen = 1.0F;
		particleBlue = 1.0F;
		P = 32;
		setSize(0.02F, 0.02F);
		particleScale *= rand->nextFloat() * 0.6F + 0.2F;
		motionX1 = var5 * 0.2F + (float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.02F;
		motionY1 = var6 * 0.2F + (float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.02F;
		motionZ1 = var7 * 0.2F + (float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.02F;
		particleMaxAge = (int)(8.0f / (((float)::rand() / (float)RAND_MAX) * 0.8f + 0.2f));
}

EntityBubbleFX::~EntityBubbleFX() {
	// No dynamic resources
}

void EntityBubbleFX::onEntityUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		motionY1 = (float)((double)motionY1 + 0.002f);
		moveEntity(motionX1, motionY1, motionZ1);
		motionX1 *= 0.85F;
		motionY1 *= 0.85F;
		motionZ1 *= 0.85F;
		if(worldObj->getBlockMaterial((int)posX, (int)posY, (int)posZ) != Material::s_water) {
			setEntityDead();
		}

		if(particleMaxAge-- <= 0) {
			setEntityDead();
		}

}
