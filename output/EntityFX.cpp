#include "stdafx.h"
// Auto-generated from EntityFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "Tesselator.h"
#include "World.h"
#include <string>
#include "Entity.h"

EntityFX::EntityFX()
	: motionX1(0)
	, motionY1(0)
	, motionZ1(0)
	, P(0)
	, m_particleTextureJitterX(0)
	, m_particleTextureJitterY(0)
	, particleAge(0)
	, particleMaxAge(0)
	, particleScale(0)
	, particleGravity(0)
	, particleRed(0)
	, particleGreen(0)
	, particleBlue(0)
{
}

EntityFX::EntityFX(World* var1, float var2, float var3, float var4, float var5, float var6, float var7)
	: Entity(var1)
	, motionX1(0)
	, motionY1(0)
	, motionZ1(0)
	, P(0)
	, m_particleTextureJitterX(0)
	, m_particleTextureJitterY(0)
	, particleAge(0)
	, particleMaxAge(0)
	, particleScale(0)
	, particleGravity(0)
	, particleRed(0)
	, particleGreen(0)
	, particleBlue(0)
{
		setSize(0.2F, 0.2F);
		yOffset = height / 2.0F;
		setPosition(var2, var3, var4);
		particleRed = particleGreen = particleBlue = 1.0F;
		motionX1 = var5 + (float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.4F;
		motionY1 = var6 + (float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.4F;
		motionZ1 = var7 + (float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.4F;
		float var8 = (float)(((float)::rand() / (float)RAND_MAX) + ((float)::rand() / (float)RAND_MAX) + 1.0f) * 0.15F;
		var2 = sqrtf(motionX1 * motionX1 + motionY1 * motionY1 + motionZ1 * motionZ1);
		motionX1 = motionX1 / var2 * var8 * 0.4F;
		motionY1 = motionY1 / var2 * var8 * 0.4F + 0.1F;
		motionZ1 = motionZ1 / var2 * var8 * 0.4F;
		m_particleTextureJitterX = rand->nextFloat() * 3.0F;
		m_particleTextureJitterY = rand->nextFloat() * 3.0F;
		particleScale = (rand->nextFloat() * 0.5F + 0.5F) * 2.0F;
		particleMaxAge = (int)(4.0F / (rand->nextFloat() * 0.9F + 0.1F));
		particleAge = 0;
		canTriggerWalking = false;
}

EntityFX::~EntityFX() {
	// No dynamic resources
}

EntityFX* EntityFX::multiplyVelocity(float var1) {
		motionX1 *= 0.2F;
		motionY1 = (motionY1 - 0.1F) * 0.2F + 0.1F;
		motionZ1 *= 0.2F;
		return this;
}

EntityFX* EntityFX::multipleParticleScaleBy(float var1) {
		setSize(0.120000005F, 0.120000005F);
		particleScale *= 0.6F;
		return this;
}

void EntityFX::onEntityUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(particleAge++ >= particleMaxAge) {
			setEntityDead();
		}

		motionY1 = (float)((double)motionY1 - 0.04f * (double)particleGravity);
		moveEntity(motionX1, motionY1, motionZ1);
		motionX1 *= 0.98F;
		motionY1 *= 0.98F;
		motionZ1 *= 0.98F;
		if(onGround) {
			motionX1 *= 0.7F;
			motionZ1 *= 0.7F;
		}

}

void EntityFX::renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = (float)(P % 16) / 16.0F;
		float var9 = var8 + 0.999F / 16.0F;
		float var10 = (float)(P / 16) / 16.0F;
		float var11 = var10 + 0.999F / 16.0F;
		float var12 = 0.1F * particleScale;
		float var13 = prevPosX + (posX - prevPosX) * var2;
		float var14 = prevPosY + (posY - prevPosY) * var2;
		float var15 = prevPosZ + (posZ - prevPosZ) * var2;
		var2 = getEntityBrightness(var2);
		var1->color3f(particleRed * var2, particleGreen * var2, particleBlue * var2);
		var1->vertexUV(var13 - var3 * var12 - var6 * var12, var14 - var4 * var12, var15 - var5 * var12 - var7 * var12, var8, var11);
		var1->vertexUV(var13 - var3 * var12 + var6 * var12, var14 + var4 * var12, var15 - var5 * var12 + var7 * var12, var8, var10);
		var1->vertexUV(var13 + var3 * var12 + var6 * var12, var14 + var4 * var12, var15 + var5 * var12 + var7 * var12, var9, var10);
		var1->vertexUV(var13 + var3 * var12 - var6 * var12, var14 - var4 * var12, var15 + var5 * var12 - var7 * var12, var9, var11);
}

int EntityFX::getFXLayer() {
		return 0;
}

void EntityFX::writeEntityToNBT(NBTTagCompound* var1) {
	// TODO: implement
}

const std::string& EntityFX::getEntityString() {
		return NULL;
}

void EntityFX::readEntityFromNBT(NBTTagCompound* var1) {
	// TODO: implement
}
