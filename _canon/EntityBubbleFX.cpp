#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityBubbleFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityBubbleFX.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFX.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MathHelper.h"
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
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

EntityBubbleFX::EntityBubbleFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: EntityFX(var1, var2, var4, var6, var8, var10, var12)
{
	this->particleRed = 1.0F;
	this->particleGreen = 1.0F;
	this->particleBlue = 1.0F;
	this->particleTextureIndex = 32;
	this->setSize(0.02F, 0.02F);
	this->particleScale *= this->rand->nextFloat() * 0.6F + 0.2F;
	this->motionX = var8 * (double)0.2F + (double)((float)(javaMathRandom() * 2.0 - 1.0) * 0.02F);
	this->motionY = var10 * (double)0.2F + (double)((float)(javaMathRandom() * 2.0 - 1.0) * 0.02F);
	this->motionZ = var12 * (double)0.2F + (double)((float)(javaMathRandom() * 2.0 - 1.0) * 0.02F);
	this->particleMaxAge = (int)(8.0 / (javaMathRandom() * 0.8 + 0.2));
}

EntityBubbleFX::~EntityBubbleFX()
{
}

void EntityBubbleFX::onUpdate()
{
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	this->motionY += 0.002;
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.85F;
	this->motionY *= (double)0.85F;
	this->motionZ *= (double)0.85F;
	if(this->worldObj->getBlockMaterial(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->posY), MathHelper::floor_double(this->posZ)) != Material::s_water) {
	this->setEntityDead();
	}
	if(this->particleMaxAge-- <= 0) {
	this->setEntityDead();
	}
}

void EntityBubbleFX::initStatics()
{
}
