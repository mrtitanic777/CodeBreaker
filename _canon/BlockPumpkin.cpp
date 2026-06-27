#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockPumpkin.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockPumpkin.h"
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

BlockPumpkin::BlockPumpkin(int var1, int var2, bool var3)
	: Block(var1, Material::s_pumpkin)
	, facingState(0)
{
	this->blockIndexInTexture = var2;
	this->setTickOnLoad(true);
	this->facingState = var3;
}

BlockPumpkin::~BlockPumpkin()
{
}

int BlockPumpkin::getBlockTextureFromSideAndMetadata(int var1, int var2)
{
	if(var1 == 1) {
	return this->blockIndexInTexture;
	} else if(var1 == 0) {
	return this->blockIndexInTexture;
	} else {
	int var3 = this->blockIndexInTexture + 1 + 16;
	if(this->facingState) {
	++var3;
	}
	return var2 == 0 && var1 == 2 ? var3 : (var2 == 1 && var1 == 5 ? var3 : (var2 == 2 && var1 == 3 ? var3 : (var2 == 3 && var1 == 4 ? var3 : this->blockIndexInTexture + 16)));
	}
}

int BlockPumpkin::getBlockTextureFromSide(int var1)
{
	return var1 == 1 ? this->blockIndexInTexture : (var1 == 0 ? this->blockIndexInTexture : (var1 == 3 ? this->blockIndexInTexture + 1 + 16 : this->blockIndexInTexture + 16));
}

void BlockPumpkin::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	Block::onBlockAdded(var1, var2, var3, var4);
}

bool BlockPumpkin::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockId(var2, var3, var4);
	return (var5 == 0 || Block::s_blocksList[var5]->blockMaterial->getIsLiquid()) && var1->isBlockOpaqueCube(var2, var3 - 1, var4);
}

void BlockPumpkin::onBlockPlacedBy(World* var1, int var2, int var3, int var4, EntityLiving* var5)
{
	int var6 = MathHelper::floor_double((double)(var5->rotationYaw * 4.0F / 360.0F) + 0.5) & 3;
	var1->setBlockMetadataWithNotify(var2, var3, var4, var6);
}

void BlockPumpkin::initStatics()
{
}
