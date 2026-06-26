#include "stdafx.h"
// Auto-generated from EntitySmokeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySmokeFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"

EntitySmokeFX::EntitySmokeFX()
	: m_smokeParticleScale(0)
{
}

EntitySmokeFX::EntitySmokeFX(World* var1, float var2, float var3, float var4)
	: m_smokeParticleScale(0)
{
  // PORT: constructor delegation  // was: this(var1, var2, var3, var4, 1.0F);
}

EntitySmokeFX::EntitySmokeFX(World* var1, float var2, float var3, float var4, float var5)
	: EntityFX(var1, var2, var3, var4, 0.0f, 0.0f, 0.0f)
	, m_smokeParticleScale(0)
{
		motionX1 *= 0.1F;
		motionY1 *= 0.1F;
		motionZ1 *= 0.1F;
		particleRed = particleGreen = particleBlue = (float)((rand() / (float)RAND_MAX) * (double)0.3F);
		particleScale *= 12.0F / 16.0F;
		particleScale *= var5;
		m_smokeParticleScale = particleScale;
		particleMaxAge = (int)(8.0f / ((rand() / (float)RAND_MAX) * 0.8f + 0.2f));
		particleMaxAge = (int)((float)particleMaxAge * var5);
		noClip = false;
}

EntitySmokeFX::~EntitySmokeFX() {
	// No dynamic resources
}

void EntitySmokeFX::renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)particleAge + var2) / (float)particleMaxAge * 32.0F;
		if(var8 < 0.0F) {
			var8 = 0.0F;
		}

		if(var8 > 1.0F) {
			var8 = 1.0F;
		}

		particleScale = m_smokeParticleScale * var8;
		EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

void EntitySmokeFX::onEntityUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(particleAge++ >= particleMaxAge) {
			setEntityDead();
		}

		P = 7 - (particleAge << 3) / particleMaxAge;
		motionY1 = (float)((double)motionY1 + 0.004f);
		moveEntity(motionX1, motionY1, motionZ1);
		motionX1 *= 0.96F;
		motionY1 *= 0.96F;
		motionZ1 *= 0.96F;
		if(onGround) {
			motionX1 *= 0.7F;
			motionZ1 *= 0.7F;
		}

}
