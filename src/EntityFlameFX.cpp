#include "stdafx.h"
// Auto-generated from EntityFlameFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFlameFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"

EntityFlameFX::EntityFlameFX()
	: m_flameScale(0)
{
}

EntityFlameFX::EntityFlameFX(World* var1, float var2, float var3, float var4)
	: EntityFX(var1, var2, var3, var4, 0.0f, 0.0f, 0.0f)
	, m_flameScale(0)
{
		motionX1 *= 0.01F;
		motionY1 *= 0.01F;
		motionZ1 *= 0.01F;
		rand->nextFloat();
		rand->nextFloat();
		rand->nextFloat();
		rand->nextFloat();
		rand->nextFloat();
		rand->nextFloat();
		m_flameScale = particleScale;
		particleRed = particleGreen = particleBlue = 1.0F;
		particleMaxAge = (int)(8.0f / ((rand() / (float)RAND_MAX) * 0.8f + 0.2f)) + 4;
		noClip = true;
		P = 48;
}

EntityFlameFX::~EntityFlameFX() {
	// No dynamic resources
}

void EntityFlameFX::renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)particleAge + var2) / (float)particleMaxAge;
		particleScale = m_flameScale * (1.0F - var8 * var8 * 0.5F);
		EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

float EntityFlameFX::getEntityBrightness(float var1) {
		float var2 = ((float)particleAge + var1) / (float)particleMaxAge;
		if(var2 < 0.0F) {
			var2 = 0.0F;
		}

		if(var2 > 1.0F) {
			var2 = 1.0F;
		}

		var1 = EntityFX::getEntityBrightness(var1);
		return var1 * var2 + (1.0F - var2);
}

void EntityFlameFX::onEntityUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(particleAge++ >= particleMaxAge) {
			setEntityDead();
		}

		moveEntity(motionX1, motionY1, motionZ1);
		motionX1 *= 0.96F;
		motionY1 *= 0.96F;
		motionZ1 *= 0.96F;
		if(onGround) {
			motionX1 *= 0.7F;
			motionZ1 *= 0.7F;
		}

}
