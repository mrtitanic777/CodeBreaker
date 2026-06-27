#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityReddustFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityReddustFX.h"
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

EntityReddustFX::EntityReddustFX(World* var1, double var2, double var4, double var6)
	: EntityFX(var1, var2, var4, var6, 0.0, 0.0, 0.0)
	, baseParticleScale(0)
{
	// inlined this(var1, var2, var4, var6, 1.0F) -- C++03 lacks ctor delegation
	this->motionX *= (double)0.1F;
	this->motionY *= (double)0.1F;
	this->motionZ *= (double)0.1F;
	this->particleRed = (float)(javaMathRandom() * (double)0.3F) + 0.7F;
	this->particleGreen = this->particleBlue = (float)(javaMathRandom() * (double)0.1F);
	this->particleScale *= 12.0F / 16.0F;
	this->particleScale *= 1.0F;
	this->baseParticleScale = this->particleScale;
	this->particleMaxAge = (int)(8.0 / (javaMathRandom() * 0.8 + 0.2));
	this->particleMaxAge = (int)((float)this->particleMaxAge * 1.0F);
	this->noClip = false;
}

EntityReddustFX::EntityReddustFX(World* var1, double var2, double var4, double var6, float var8)
	: EntityFX(var1, var2, var4, var6, 0.0, 0.0, 0.0)
	, baseParticleScale(0)
{
	this->motionX *= (double)0.1F;
	this->motionY *= (double)0.1F;
	this->motionZ *= (double)0.1F;
	this->particleRed = (float)(javaMathRandom() * (double)0.3F) + 0.7F;
	this->particleGreen = this->particleBlue = (float)(javaMathRandom() * (double)0.1F);
	this->particleScale *= 12.0F / 16.0F;
	this->particleScale *= var8;
	this->baseParticleScale = this->particleScale;
	this->particleMaxAge = (int)(8.0 / (javaMathRandom() * 0.8 + 0.2));
	this->particleMaxAge = (int)((float)this->particleMaxAge * var8);
	this->noClip = false;
}

EntityReddustFX::~EntityReddustFX()
{
}

void EntityReddustFX::renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
	float var8 = ((float)this->e + var2) / (float)this->particleMaxAge * 32.0F;
	if(var8 < 0.0F) {
	var8 = 0.0F;
	}
	if(var8 > 1.0F) {
	var8 = 1.0F;
	}
	this->particleScale = this->baseParticleScale * var8;
	EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

void EntityReddustFX::onUpdate()
{
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	if(this->e++ >= this->particleMaxAge) {
	this->setEntityDead();
	}
	this->particleTextureIndex = 7 - this->e * 8 / this->particleMaxAge;
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	if(this->posY == this->prevPosY) {
	this->motionX *= 1.1;
	this->motionZ *= 1.1;
	}
	this->motionX *= (double)0.96F;
	this->motionY *= (double)0.96F;
	this->motionZ *= (double)0.96F;
	if(this->onGround) {
	this->motionX *= (double)0.7F;
	this->motionZ *= (double)0.7F;
	}
}

void EntityReddustFX::initStatics()
{
}
