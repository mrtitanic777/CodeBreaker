#include "stdafx.h"
// Auto-generated from EntityRainFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityRainFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "Material.h"

EntityRainFX::EntityRainFX()
{
}

EntityRainFX::EntityRainFX(World* var1, float var2, float var3, float var4)
	: EntityFX(var1, var2, var3, var4, 0.0f, 0.0f, 0.0f)
{
		motionX1 *= 0.3F;
		motionY1 = (float)((float)::rand() / (float)RAND_MAX) * 0.2F + 0.1F;
		motionZ1 *= 0.3F;
		particleRed = 1.0F;
		particleGreen = 1.0F;
		particleBlue = 1.0F;
		P = 16;
		setSize(0.01F, 0.01F);
		particleGravity = 0.06F;
		particleMaxAge = (int)(8.0f / (((float)::rand() / (float)RAND_MAX) * 0.8f + 0.2f));
}

EntityRainFX::~EntityRainFX() {
	// No dynamic resources
}

void EntityRainFX::renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

void EntityRainFX::onEntityUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		motionY1 -= particleGravity;
		moveEntity(motionX1, motionY1, motionZ1);
		motionX1 *= 0.98F;
		motionY1 *= 0.98F;
		motionZ1 *= 0.98F;
		if(particleMaxAge-- <= 0) {
			setEntityDead();
		}

		if(onGround) {
			if(((float)::rand() / (float)RAND_MAX) < 0.5f) {
				setEntityDead();
			}

			motionX1 *= 0.7F;
			motionZ1 *= 0.7F;
		}

		Material* var1 = worldObj->getBlockMaterial((int)posX, (int)posY, (int)posZ);
		if(var1->getIsLiquid() || var1->isSolid()) {
			setEntityDead();
		}

}
