#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldGenMinable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenMinable.h"
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
#include "WorldGenerator.h"
#include "WorldProvider.h"

WorldGenMinable::WorldGenMinable(int var1, int var2)
	: minableBlockId(0)
	, numberOfBlocks(0)
{
	this->minableBlockId = var1;
	this->numberOfBlocks = var2;
}

WorldGenMinable::~WorldGenMinable()
{
}

bool WorldGenMinable::generate(World* var1, Random* var2, int var3, int var4, int var5)
{
	float var6 = var2->nextFloat() * (float)3.1415927F;
	double var7 = (double)((float)(var3 + 8) + MathHelper::sin(var6) * (float)this->numberOfBlocks / 8.0F);
	double var9 = (double)((float)(var3 + 8) - MathHelper::sin(var6) * (float)this->numberOfBlocks / 8.0F);
	double var11 = (double)((float)(var5 + 8) + MathHelper::cos(var6) * (float)this->numberOfBlocks / 8.0F);
	double var13 = (double)((float)(var5 + 8) - MathHelper::cos(var6) * (float)this->numberOfBlocks / 8.0F);
	double var15 = (double)(var4 + var2->nextInt(3) + 2);
	double var17 = (double)(var4 + var2->nextInt(3) + 2);
	for(int var19 = 0; var19 <= this->numberOfBlocks; ++var19) {
	double var20 = var7 + (var9 - var7) * (double)var19 / (double)this->numberOfBlocks;
	double var22 = var15 + (var17 - var15) * (double)var19 / (double)this->numberOfBlocks;
	double var24 = var11 + (var13 - var11) * (double)var19 / (double)this->numberOfBlocks;
	double var26 = var2->nextDouble() * (double)this->numberOfBlocks / 16.0;
	double var28 = (double)(MathHelper::sin((float)var19 * (float)3.1415927F / (float)this->numberOfBlocks) + 1.0F) * var26 + 1.0;
	double var30 = (double)(MathHelper::sin((float)var19 * (float)3.1415927F / (float)this->numberOfBlocks) + 1.0F) * var26 + 1.0;
	for(int var32 = (int)(var20 - var28 / 2.0); var32 <= (int)(var20 + var28 / 2.0); ++var32) {
	for(int var33 = (int)(var22 - var30 / 2.0); var33 <= (int)(var22 + var30 / 2.0); ++var33) {
	for(int var34 = (int)(var24 - var28 / 2.0); var34 <= (int)(var24 + var28 / 2.0); ++var34) {
	double var35 = ((double)var32 + 0.5 - var20) / (var28 / 2.0);
	double var37 = ((double)var33 + 0.5 - var22) / (var30 / 2.0);
	double var39 = ((double)var34 + 0.5 - var24) / (var28 / 2.0);
	if(var35 * var35 + var37 * var37 + var39 * var39 < 1.0 && var1->getBlockId(var32, var33, var34) == Block::s_stone->blockID) {
	var1->setBlock(var32, var33, var34, this->minableBlockId);
	}
	}
	}
	}
	}
	return true;
}

void WorldGenMinable::initStatics()
{
}
