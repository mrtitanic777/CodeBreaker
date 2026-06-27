#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemDoor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemDoor.h"
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
#include "WorldProvider.h"

ItemDoor::ItemDoor(int var1, Material* var2)
	: Item(var1)
	, blockMaterial(NULL)
{
	this->blockMaterial = var2;
	this->maxDamage = 64;
	this->maxStackSize = 1;
}

ItemDoor::~ItemDoor()
{
}

bool ItemDoor::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
{
	if(var7 != 1) {
	return false;
	} else {
	++var5;
	Block* var8;
	if(this->blockMaterial == Material::s_wood) {
	var8 = Block::s_doorWood;
	} else {
	var8 = Block::s_doorSteel;
	}
	if(!var8->canPlaceBlockAt(var3, var4, var5, var6)) {
	return false;
	} else {
	int var9 = MathHelper::floor_double((double)((var2->rotationYaw + 180.0F) * 4.0F / 360.0F) - 0.5) & 3;
	unsigned char var10 = 0;
	unsigned char var11 = 0;
	if(var9 == 0) {
	var11 = 1;
	}
	if(var9 == 1) {
	var10 = -1;
	}
	if(var9 == 2) {
	var11 = -1;
	}
	if(var9 == 3) {
	var10 = 1;
	}
	int var12 = (var3->isBlockOpaqueCube(var4 - var10, var5, var6 - var11) ? 1 : 0) + (var3->isBlockOpaqueCube(var4 - var10, var5 + 1, var6 - var11) ? 1 : 0);
	int var13 = (var3->isBlockOpaqueCube(var4 + var10, var5, var6 + var11) ? 1 : 0) + (var3->isBlockOpaqueCube(var4 + var10, var5 + 1, var6 + var11) ? 1 : 0);
	bool var14 = var3->getBlockId(var4 - var10, var5, var6 - var11) == var8->blockID || var3->getBlockId(var4 - var10, var5 + 1, var6 - var11) == var8->blockID;
	bool var15 = var3->getBlockId(var4 + var10, var5, var6 + var11) == var8->blockID || var3->getBlockId(var4 + var10, var5 + 1, var6 + var11) == var8->blockID;
	bool var16 = false;
	if(var14 && !var15) {
	var16 = true;
	} else if(var13 > var12) {
	var16 = true;
	}
	if(var16) {
	var9 = (var9 - 1) & 3;
	var9 += 4;
	}
	var3->setBlockWithNotify(var4, var5, var6, var8->blockID);
	var3->setBlockMetadataWithNotify(var4, var5, var6, var9);
	var3->setBlockWithNotify(var4, var5 + 1, var6, var8->blockID);
	var3->setBlockMetadataWithNotify(var4, var5 + 1, var6, var9 + 8);
	--var1->stackSize;
	return true;
	}
	}
}

void ItemDoor::initStatics()
{
}
