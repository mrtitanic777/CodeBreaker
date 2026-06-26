#include "stdafx.h"
// Auto-generated from EntityLavaFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLavaFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"

EntityLavaFX::EntityLavaFX()
	: m_lavaParticleScale(0)
{
}

EntityLavaFX::EntityLavaFX(World* var1, float var2, float var3, float var4)
	: EntityFX(var1, var2, var3, var4, 0.0f, 0.0f, 0.0f)
	, m_lavaParticleScale(0)
{
		motionX1 *= 0.8F;
		motionY1 *= 0.8F;
		motionZ1 *= 0.8F;
		motionY1 = rand->nextFloat() * 0.4F + 0.05F;
		particleRed = particleGreen = particleBlue = 1.0F;
		particleScale *= rand->nextFloat() * 2.0F + 0.2F;
		m_lavaParticleScale = particleScale;
		particleMaxAge = (int)(16.0f / (((float)::rand() / (float)RAND_MAX) * 0.8f + 0.2f));
		noClip = false;
		P = 49;
}

EntityLavaFX::~EntityLavaFX() {
	// No dynamic resources
}

float EntityLavaFX::getEntityBrightness(float var1) {
		return 1.0F;
}

void EntityLavaFX::renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)particleAge + var2) / (float)particleMaxAge;
		particleScale = m_lavaParticleScale * (1.0F - var8 * var8);
		EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

void EntityLavaFX::onEntityUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(particleAge++ >= particleMaxAge) {
			setEntityDead();
		}

		float var1 = (float)particleAge / (float)particleMaxAge;
		if(rand->nextFloat() > var1) {
			worldObj->spawnParticle("smoke", posX, posY, posZ, motionX1, motionY1, motionZ1);
		}

		motionY1 = (float)((double)motionY1 - 0.03f);
		moveEntity(motionX1, motionY1, motionZ1);
		motionX1 *= 0.999F;
		motionY1 *= 0.999F;
		motionZ1 *= 0.999F;
		if(onGround) {
			motionX1 *= 0.7F;
			motionZ1 *= 0.7F;
		}

}
