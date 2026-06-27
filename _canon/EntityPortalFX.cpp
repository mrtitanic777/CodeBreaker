#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityPortalFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPortalFX.h"
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

EntityPortalFX::EntityPortalFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: EntityFX(var1, var2, var4, var6, var8, var10, var12)
	, portalParticleScale(0)
	, portalPosX(0)
	, portalPosY(0)
	, portalPosZ(0)
{
	this->motionX = var8;
	this->motionY = var10;
	this->motionZ = var12;
	this->portalPosX = this->posX = var2;
	this->portalPosY = this->posY = var4;
	this->portalPosZ = this->posZ = var6;
	float var14 = this->rand->nextFloat() * 0.6F + 0.4F;
	this->portalParticleScale = this->particleScale = this->rand->nextFloat() * 0.2F + 0.5F;
	this->particleRed = this->particleGreen = this->particleBlue = 1.0F * var14;
	this->particleGreen *= 0.3F;
	this->particleRed *= 0.9F;
	this->particleMaxAge = (int)(javaMathRandom() * 10.0) + 40;
	this->noClip = true;
	this->particleTextureIndex = (int)(javaMathRandom() * 8.0);
}

EntityPortalFX::~EntityPortalFX()
{
}

void EntityPortalFX::renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
	float var8 = ((float)this->e + var2) / (float)this->particleMaxAge;
	var8 = 1.0F - var8;
	var8 *= var8;
	var8 = 1.0F - var8;
	this->particleScale = this->portalParticleScale * var8;
	EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

float EntityPortalFX::getEntityBrightness(float var1)
{
	float var2 = EntityFX::getEntityBrightness(var1);
	float var3 = (float)this->e / (float)this->particleMaxAge;
	var3 *= var3;
	var3 *= var3;
	return var2 * (1.0F - var3) + var3;
}

void EntityPortalFX::onUpdate()
{
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	float var1 = (float)this->e / (float)this->particleMaxAge;
	float var2 = var1;
	var1 = -var1 + var1 * var1 * 2.0F;
	var1 = 1.0F - var1;
	this->posX = this->portalPosX + this->motionX * (double)var1;
	this->posY = this->portalPosY + this->motionY * (double)var1 + (double)(1.0F - var2);
	this->posZ = this->portalPosZ + this->motionZ * (double)var1;
	if(this->e++ >= this->particleMaxAge) {
	this->setEntityDead();
	}
}

void EntityPortalFX::initStatics()
{
}
