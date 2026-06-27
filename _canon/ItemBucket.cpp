#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemBucket.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemBucket.h"
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
#include "EntityCow.h"
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

ItemBucket::ItemBucket(int var1, int var2)
	: Item(var1)
	, isFull(0)
{
	this->maxStackSize = 1;
	this->maxDamage = 64;
	this->isFull = var2;
}

ItemBucket::~ItemBucket()
{
}

ItemStack* ItemBucket::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3)
{
	float var4 = 1.0F;
	float var5 = var3->prevRotationPitch + (var3->rotationPitch - var3->prevRotationPitch) * var4;
	float var6 = var3->prevRotationYaw + (var3->rotationYaw - var3->prevRotationYaw) * var4;
	double var7 = var3->prevPosX + (var3->posX - var3->prevPosX) * (double)var4;
	double var9 = var3->prevPosY + (var3->posY - var3->prevPosY) * (double)var4 + 1.62 - (double)var3->yOffset;
	double var11 = var3->prevPosZ + (var3->posZ - var3->prevPosZ) * (double)var4;
	Vec3D* var13 = Vec3D::createVector(var7, var9, var11);
	float var14 = MathHelper::cos(-var6 * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	float var15 = MathHelper::sin(-var6 * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	float var16 = -MathHelper::cos(-var5 * ((float)3.1415927F / 180.0F));
	float var17 = MathHelper::sin(-var5 * ((float)3.1415927F / 180.0F));
	float var18 = var15 * var16;
	float var20 = var14 * var16;
	double var21 = 5.0;
	Vec3D* var23 = var13->addVector((double)var18 * var21, (double)var17 * var21, (double)var20 * var21);
	MovingObjectPosition* var24 = var2->rayTraceBlocks(var13, var23, this->isFull == 0);
	if(var24 == NULL) {
	return var1;
	} else {
	if(var24->typeOfHit == 0) {
	int var25 = var24->blockX;
	int var26 = var24->blockY;
	int var27 = var24->blockZ;
	if(!var2->canMineBlock(var3, var25, var26, var27)) {
	return var1;
	}
	if(this->isFull == 0) {
	if(var2->getBlockMaterial(var25, var26, var27) == Material::s_water && var2->getBlockMetadata(var25, var26, var27) == 0) {
	var2->setBlockWithNotify(var25, var26, var27, 0);
	return new ItemStack(Item::s_bucketWater);
	}
	if(var2->getBlockMaterial(var25, var26, var27) == Material::s_lava && var2->getBlockMetadata(var25, var26, var27) == 0) {
	var2->setBlockWithNotify(var25, var26, var27, 0);
	return new ItemStack(Item::s_bucketLava);
	}
	} else {
	if(this->isFull < 0) {
	return new ItemStack(Item::s_bucketEmpty);
	}
	if(var24->sideHit == 0) {
	--var26;
	}
	if(var24->sideHit == 1) {
	++var26;
	}
	if(var24->sideHit == 2) {
	--var27;
	}
	if(var24->sideHit == 3) {
	++var27;
	}
	if(var24->sideHit == 4) {
	--var25;
	}
	if(var24->sideHit == 5) {
	++var25;
	}
	if(var2->getBlockId(var25, var26, var27) == 0 || !var2->getBlockMaterial(var25, var26, var27)->isSolid()) {
	if(var2->worldProvider->doesWaterVaporize && this->isFull == Block::s_waterStill->blockID) {
	var2->playSoundEffect(var7 + 0.5,var9 + 0.5,var11 + 0.5,"random.fizz",0.5F,2.6F + (var2->rand->nextFloat() - var2->rand->nextFloat()) * 0.8F);
	for(int var28 = 0; var28 < 8; ++var28) {
	var2->spawnParticle("largesmoke",(double)var25 + javaMathRandom(),(double)var26 + javaMathRandom(),(double)var27 + javaMathRandom(),0.0,0.0,0.0);
	}
	} else {
	var2->setBlockAndMetadataWithNotify(var25, var26, var27, this->isFull, 0);
	}
	return new ItemStack(Item::s_bucketEmpty);
	}
	}
	} else if(this->isFull == 0 && (dynamic_cast<EntityCow*>(var24->entityHit) != NULL)) {
	return new ItemStack(Item::s_bucketMilk);
	}
	return var1;
	}
}

void ItemBucket::initStatics()
{
}
