#include "stdafx.h"
// Auto-generated from EntityDiggingFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityDiggingFX.h"
#include "Common.h"
#include "Block.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"

EntityDiggingFX::EntityDiggingFX()
{
}

EntityDiggingFX::EntityDiggingFX(World* var1, float var2, float var3, float var4, float var5, float var6, float var7, Block* var8)
	: EntityFX(var1, var2, var3, var4, var5, var6, var7)
{
		P = var8->blockIndexInTexture;
		particleGravity = var8->blockParticleGravity;
		particleRed = particleGreen = particleBlue = 0.6F;
		particleScale /= 2.0F;
}

EntityDiggingFX::~EntityDiggingFX() {
	// No dynamic resources
}

int EntityDiggingFX::getFXLayer() {
		return 1;
}

void EntityDiggingFX::renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)(P % 16) + particleTextureJitterX / 4.0F) / 16.0F;
		float var9 = var8 + 0.999F / 64.0F;
		float var10 = ((float)(P / 16) + particleTextureJitterY / 4.0F) / 16.0F;
		float var11 = var10 + 0.999F / 64.0F;
		float var12 = 0.1F * particleScale;
		float var13 = prevPosX + (posX - prevPosX) * var2;
		float var14 = prevPosY + (posY - prevPosY) * var2;
		float var15 = prevPosZ + (posZ - prevPosZ) * var2;
		var2 = getEntityBrightness(var2);
		var1->color3f(var2 * particleRed, var2 * particleGreen, var2 * particleBlue);
		var1->vertexUV(var13 - var3 * var12 - var6 * var12, var14 - var4 * var12, var15 - var5 * var12 - var7 * var12, var8, var11);
		var1->vertexUV(var13 - var3 * var12 + var6 * var12, var14 + var4 * var12, var15 - var5 * var12 + var7 * var12, var8, var10);
		var1->vertexUV(var13 + var3 * var12 + var6 * var12, var14 + var4 * var12, var15 + var5 * var12 + var7 * var12, var9, var10);
		var1->vertexUV(var13 + var3 * var12 - var6 * var12, var14 - var4 * var12, var15 + var5 * var12 - var7 * var12, var9, var11);
}
