#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityFlameFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFlameFX.h"
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

EntityFlameFX::EntityFlameFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: EntityFX(var1, var2, var4, var6, var8, var10, var12)
	, initialParticleScale(0)
{
	this->motionX = this->motionX * (double)0.01F + var8;
	this->motionY = this->motionY * (double)0.01F + var10;
	this->motionZ = this->motionZ * (double)0.01F + var12;
	double var10000 = var2 + (double)((this->rand->nextFloat() - this->rand->nextFloat()) * 0.05F);
	var10000 = var4 + (double)((this->rand->nextFloat() - this->rand->nextFloat()) * 0.05F);
	var10000 = var6 + (double)((this->rand->nextFloat() - this->rand->nextFloat()) * 0.05F);
	this->initialParticleScale = this->particleScale;
	this->particleRed = this->particleGreen = this->particleBlue = 1.0F;
	this->particleMaxAge = (int)(8.0 / (javaMathRandom() * 0.8 + 0.2)) + 4;
	this->noClip = true;
	this->particleTextureIndex = 48;
}

EntityFlameFX::~EntityFlameFX()
{
}

void EntityFlameFX::renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
	float var8 = ((float)this->e + var2) / (float)this->particleMaxAge;
	this->particleScale = this->initialParticleScale * (1.0F - var8 * var8 * 0.5F);
	EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

float EntityFlameFX::getEntityBrightness(float var1)
{
	float var2 = ((float)this->e + var1) / (float)this->particleMaxAge;
	if(var2 < 0.0F) {
	var2 = 0.0F;
	}
	if(var2 > 1.0F) {
	var2 = 1.0F;
	}
	float var3 = EntityFX::getEntityBrightness(var1);
	return var3 * var2 + (1.0F - var2);
}

void EntityFlameFX::onUpdate()
{
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	if(this->e++ >= this->particleMaxAge) {
	this->setEntityDead();
	}
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.96F;
	this->motionY *= (double)0.96F;
	this->motionZ *= (double)0.96F;
	if(this->onGround) {
	this->motionX *= (double)0.7F;
	this->motionZ *= (double)0.7F;
	}
}

void EntityFlameFX::initStatics()
{
}
