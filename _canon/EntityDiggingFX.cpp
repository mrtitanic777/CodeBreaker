#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityDiggingFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityDiggingFX.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "ByteBuffer.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFX.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IntBuffer.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "Tessellator.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

EntityDiggingFX::EntityDiggingFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12, Block* var14)
	: EntityFX(var1, var2, var4, var6, var8, var10, var12)
	, block(NULL)
{
	this->block = var14;
	this->particleTextureIndex = var14->blockIndexInTexture;
	this->particleGravity = var14->blockParticleGravity;
	this->particleRed = this->particleGreen = this->particleBlue = 0.6F;
	this->particleScale /= 2.0F;
}

EntityDiggingFX::~EntityDiggingFX()
{
}

EntityDiggingFX* EntityDiggingFX::applyColorMultiplier(int var1, int var2, int var3)
{
	if(this->block == Block::s_grass) {
	return this;
	} else {
	int var4 = this->block->colorMultiplier(this->worldObj, var1, var2, var3);
	this->particleRed *= (float)(var4 >> 16 & 255) / 255.0F;
	this->particleGreen *= (float)(var4 >> 8 & 255) / 255.0F;
	this->particleBlue *= (float)(var4 & 255) / 255.0F;
	return this;
	}
}

int EntityDiggingFX::getFXLayer()
{
	return 1;
}

void EntityDiggingFX::renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
	float var8 = ((float)(this->particleTextureIndex % 16) + this->particleTextureJitterX / 4.0F) / 16.0F;
	float var9 = var8 + 0.999F / 64.0F;
	float var10 = ((float)(this->particleTextureIndex / 16) + this->particleTextureJitterY / 4.0F) / 16.0F;
	float var11 = var10 + 0.999F / 64.0F;
	float var12 = 0.1F * this->particleScale;
	float var13 = (float)(this->prevPosX + (this->posX - this->prevPosX) * (double)var2 - s_interpPosX);
	float var14 = (float)(this->prevPosY + (this->posY - this->prevPosY) * (double)var2 - s_interpPosY);
	float var15 = (float)(this->prevPosZ + (this->posZ - this->prevPosZ) * (double)var2 - s_interpPosZ);
	float var16 = this->getEntityBrightness(var2);
	var1->setColorOpaque_F(var16 * this->particleRed, var16 * this->particleGreen, var16 * this->particleBlue);
	var1->addVertexWithUV((double)(var13 - var3 * var12 - var6 * var12), (double)(var14 - var4 * var12), (double)(var15 - var5 * var12 - var7 * var12), (double)var8, (double)var11);
	var1->addVertexWithUV((double)(var13 - var3 * var12 + var6 * var12), (double)(var14 + var4 * var12), (double)(var15 - var5 * var12 + var7 * var12), (double)var8, (double)var10);
	var1->addVertexWithUV((double)(var13 + var3 * var12 + var6 * var12), (double)(var14 + var4 * var12), (double)(var15 + var5 * var12 + var7 * var12), (double)var9, (double)var10);
	var1->addVertexWithUV((double)(var13 + var3 * var12 - var6 * var12), (double)(var14 - var4 * var12), (double)(var15 + var5 * var12 - var7 * var12), (double)var9, (double)var11);
}

void EntityDiggingFX::initStatics()
{
}
