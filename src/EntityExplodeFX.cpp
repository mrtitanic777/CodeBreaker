#include "stdafx.h"
// Auto-generated from EntityExplodeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityExplodeFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"

EntityExplodeFX::EntityExplodeFX()
{
}

EntityExplodeFX::EntityExplodeFX(World* var1, float var2, float var3, float var4, float var5, float var6, float var7)
	: EntityFX(var1, var2, var3, var4, var5, var6, var7)
{
		motionX1 = var5 + (float)((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.05F;
		motionY1 = var6 + (float)((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.05F;
		motionZ1 = var7 + (float)((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.05F;
		particleRed = particleGreen = particleBlue = rand->nextFloat() * 0.3F + 0.7F;
		particleScale = rand->nextFloat() * rand->nextFloat() * 6.0F + 1.0F;
		particleMaxAge = (int)(16.0f / ((double)rand->nextFloat() * 0.8f + 0.2f)) + 2;
}

EntityExplodeFX::~EntityExplodeFX() {
	// No dynamic resources
}

void EntityExplodeFX::renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

void EntityExplodeFX::onEntityUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(particleAge++ >= particleMaxAge) {
			setEntityDead();
		}

		P = 7 - (particleAge << 3) / particleMaxAge;
		motionY1 = (float)((double)motionY1 + 0.004f);
		moveEntity(motionX1, motionY1, motionZ1);
		motionX1 *= 0.9F;
		motionY1 *= 0.9F;
		motionZ1 *= 0.9F;
		if(onGround) {
			motionX1 *= 0.7F;
			motionZ1 *= 0.7F;
		}

}
