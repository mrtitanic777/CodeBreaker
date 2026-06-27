#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldGenDungeons.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenDungeons.h"
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
#include "TileEntityChest.h"
#include "TileEntityMobSpawner.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldGenerator.h"
#include "WorldProvider.h"

WorldGenDungeons::~WorldGenDungeons()
{
}

bool WorldGenDungeons::generate(World* var1, Random* var2, int var3, int var4, int var5)
{
	unsigned char var6 = 3;
	int var7 = var2->nextInt(2) + 2;
	int var8 = var2->nextInt(2) + 2;
	int var9 = 0;
	int var10;
	int var11;
	int var12;
	for(var10 = var3 - var7 - 1; var10 <= var3 + var7 + 1; ++var10) {
	for(var11 = var4 - 1; var11 <= var4 + var6 + 1; ++var11) {
	for(var12 = var5 - var8 - 1; var12 <= var5 + var8 + 1; ++var12) {
	Material* var13 = var1->getBlockMaterial(var10, var11, var12);
	if(var11 == var4 - 1 && !var13->isSolid()) {
	return false;
	}
	if(var11 == var4 + var6 + 1 && !var13->isSolid()) {
	return false;
	}
	if((var10 == var3 - var7 - 1 || var10 == var3 + var7 + 1 || var12 == var5 - var8 - 1 || var12 == var5 + var8 + 1) && var11 == var4 && var1->getBlockId(var10, var11, var12) == 0 && var1->getBlockId(var10, var11 + 1, var12) == 0) {
	++var9;
	}
	}
	}
	}
	if(var9 >= 1 && var9 <= 5) {
	for(var10 = var3 - var7 - 1; var10 <= var3 + var7 + 1; ++var10) {
	for(var11 = var4 + var6; var11 >= var4 - 1; --var11) {
	for(var12 = var5 - var8 - 1; var12 <= var5 + var8 + 1; ++var12) {
	if(var10 != var3 - var7 - 1 && var11 != var4 - 1 && var12 != var5 - var8 - 1 && var10 != var3 + var7 + 1 && var11 != var4 + var6 + 1 && var12 != var5 + var8 + 1) {
	var1->setBlockWithNotify(var10, var11, var12, 0);
	} else if(var11 >= 0 && !var1->getBlockMaterial(var10, var11 - 1, var12)->isSolid()) {
	var1->setBlockWithNotify(var10, var11, var12, 0);
	} else if(var1->getBlockMaterial(var10, var11, var12)->isSolid()) {
	if(var11 == var4 - 1 && var2->nextInt(4) != 0) {
	var1->setBlockWithNotify(var10, var11, var12, Block::s_cobblestoneMossy->blockID);
	} else {
	var1->setBlockWithNotify(var10, var11, var12, Block::s_cobblestone->blockID);
	}
	}
	}
	}
	}
	for(var10 = 0; var10 < 2; ++var10) {
	for(var11 = 0; var11 < 3; ++var11) {
	var12 = var3 + var2->nextInt(var7 * 2 + 1) - var7;
	int var14 = var5 + var2->nextInt(var8 * 2 + 1) - var8;
	if(var1->getBlockId(var12, var4, var14) == 0) {
	int var15 = 0;
	if(var1->getBlockMaterial(var12 - 1, var4, var14)->isSolid()) {
	++var15;
	}
	if(var1->getBlockMaterial(var12 + 1, var4, var14)->isSolid()) {
	++var15;
	}
	if(var1->getBlockMaterial(var12, var4, var14 - 1)->isSolid()) {
	++var15;
	}
	if(var1->getBlockMaterial(var12, var4, var14 + 1)->isSolid()) {
	++var15;
	}
	if(var15 == 1) {
	var1->setBlockWithNotify(var12, var4, var14, Block::s_crate->blockID);
	TileEntityChest* var16 = (TileEntityChest*)var1->getBlockTileEntity(var12, var4, var14);
	int var17 = 0;
	while(true) {
	if(var17 >= 8) {
	goto label110_cont;
	}
	ItemStack* var18 = this->pickCheckLootItem(var2);
	if(var18 != NULL) {
	var16->setInventorySlotContents(var2->nextInt(var16->getSizeInventory()), var18);
	}
	++var17;
	}
	}
	}
	}
	label110_cont:;
	}
	var1->setBlockWithNotify(var3, var4, var5, Block::s_mobSpawner->blockID);
	TileEntityMobSpawner* var19 = (TileEntityMobSpawner*)var1->getBlockTileEntity(var3, var4, var5);
	var19->entityID = this->pickMobSpawner(var2);
	return true;
	} else {
	return false;
	}
}

ItemStack* WorldGenDungeons::pickCheckLootItem(Random* var1)
{
	int var2 = var1->nextInt(11);
	return var2 == 0 ? new ItemStack(Item::s_saddle) : (var2 == 1 ? new ItemStack(Item::s_ingotIron, var1->nextInt(4) + 1) : (var2 == 2 ? new ItemStack(Item::s_bread) : (var2 == 3 ? new ItemStack(Item::s_wheat, var1->nextInt(4) + 1) : (var2 == 4 ? new ItemStack(Item::s_gunpowder, var1->nextInt(4) + 1) : (var2 == 5 ? new ItemStack(Item::s_silk, var1->nextInt(4) + 1) : (var2 == 6 ? new ItemStack(Item::s_bucketEmpty) : (var2 == 7 && var1->nextInt(100) == 0 ? new ItemStack(Item::s_appleGold) : (var2 == 8 && var1->nextInt(2) == 0 ? new ItemStack(Item::s_redstone, var1->nextInt(4) + 1) : (var2 == 9 && var1->nextInt(10) == 0 ? new ItemStack(Item::s_itemsList[Item::s_record13->shiftedIndex + var1->nextInt(2)]) : NULL)))))))));
}

JavaString WorldGenDungeons::pickMobSpawner(Random* var1)
{
	int var2 = var1->nextInt(4);
	return var2 == 0 ? "Skeleton" : (var2 == 1 ? "Zombie" : (var2 == 2 ? "Zombie" : (var2 == 3 ? "Spider" : "")));
}

void WorldGenDungeons::initStatics()
{
}
