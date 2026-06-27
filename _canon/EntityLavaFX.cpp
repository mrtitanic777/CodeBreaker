#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityLavaFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLavaFX.h"
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

EntityLavaFX::EntityLavaFX(World* var1, double var2, double var4, double var6)
	: EntityFX(var1, var2, var4, var6, 0.0, 0.0, 0.0)
	, lavaParticleScale(0)
{
	this->motionX *= (double)0.8F;
	this->motionY *= (double)0.8F;
	this->motionZ *= (double)0.8F;
	this->motionY = (double)(this->rand->nextFloat() * 0.4F + 0.05F);
	this->particleRed = this->particleGreen = this->particleBlue = 1.0F;
	this->particleScale *= this->rand->nextFloat() * 2.0F + 0.2F;
	this->lavaParticleScale = this->particleScale;
	this->particleMaxAge = (int)(16.0 / (javaMathRandom() * 0.8 + 0.2));
	this->noClip = false;
	this->particleTextureIndex = 49;
}

EntityLavaFX::~EntityLavaFX()
{
}

float EntityLavaFX::getEntityBrightness(float var1)
{
	return 1.0F;
}

void EntityLavaFX::renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
	float var8 = ((float)this->e + var2) / (float)this->particleMaxAge;
	this->particleScale = this->lavaParticleScale * (1.0F - var8 * var8);
	EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

void EntityLavaFX::onUpdate()
{
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	if(this->e++ >= this->particleMaxAge) {
	this->setEntityDead();
	}
	float var1 = (float)this->e / (float)this->particleMaxAge;
	if(this->rand->nextFloat() > var1) {
	this->worldObj->spawnParticle("smoke", this->posX, this->posY, this->posZ, this->motionX, this->motionY, this->motionZ);
	}
	this->motionY -= 0.03;
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.999F;
	this->motionY *= (double)0.999F;
	this->motionZ *= (double)0.999F;
	if(this->onGround) {
	this->motionX *= (double)0.7F;
	this->motionZ *= (double)0.7F;
	}
}

void EntityLavaFX::initStatics()
{
}
