#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityFlying.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFlying.h"
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
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityLiving.h"
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

EntityFlying::EntityFlying(World* var1)
	: EntityLiving(var1)
{
}

EntityFlying::~EntityFlying()
{
}

void EntityFlying::fall(float var1)
{
}

void EntityFlying::moveEntityWithHeading(float var1, float var2)
{
	if(this->handleWaterMovement()) {
	this->moveFlying(var1, var2, 0.02F);
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.8F;
	this->motionY *= (double)0.8F;
	this->motionZ *= (double)0.8F;
	} else if(this->handleLavaMovement()) {
	this->moveFlying(var1, var2, 0.02F);
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= 0.5;
	this->motionY *= 0.5;
	this->motionZ *= 0.5;
	} else {
	float var3 = 0.91F;
	if(this->onGround) {
	var3 = 546.0F * 0.1F * 0.1F * 0.1F;
	int var4 = this->worldObj->getBlockId(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->boundingBox->minY) - 1, MathHelper::floor_double(this->posZ));
	if(var4 > 0) {
	var3 = Block::s_blocksList[var4]->slipperiness * 0.91F;
	}
	}
	float var8 = 0.16277136F / (var3 * var3 * var3);
	this->moveFlying(var1, var2, this->onGround ? 0.1F * var8 : 0.02F);
	var3 = 0.91F;
	if(this->onGround) {
	var3 = 546.0F * 0.1F * 0.1F * 0.1F;
	int var5 = this->worldObj->getBlockId(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->boundingBox->minY) - 1, MathHelper::floor_double(this->posZ));
	if(var5 > 0) {
	var3 = Block::s_blocksList[var5]->slipperiness * 0.91F;
	}
	}
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)var3;
	this->motionY *= (double)var3;
	this->motionZ *= (double)var3;
	}
	this->prevLimbSwingAmount = this->limbSwingAmount;
	double var10 = this->posX - this->prevPosX;
	double var9 = this->posZ - this->prevPosZ;
	float var7 = MathHelper::sqrt_double(var10 * var10 + var9 * var9) * 4.0F;
	if(var7 > 1.0F) {
	var7 = 1.0F;
	}
	this->limbSwingAmount += (var7 - this->limbSwingAmount) * 0.4F;
	this->limbSwing += this->limbSwingAmount;
}

bool EntityFlying::isOnLadder()
{
	return false;
}

void EntityFlying::initStatics()
{
}
