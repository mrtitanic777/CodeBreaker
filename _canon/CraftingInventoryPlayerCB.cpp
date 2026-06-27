#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from CraftingInventoryPlayerCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingInventoryPlayerCB.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "CraftingInventoryCB.h"
#include "CraftingManager.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IInventory.h"
#include "InventoryCraftResult.h"
#include "InventoryCrafting.h"
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
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

CraftingInventoryPlayerCB::CraftingInventoryPlayerCB(JArray<ItemStack* > var1)
	: craftMatrix(NULL)
	, craftResult(NULL)
{
	this->craftResult = new InventoryCraftResult();
	this->craftMatrix = new InventoryCrafting(this, var1);
	this->onCraftMatrixChanged(this->craftMatrix);
}

CraftingInventoryPlayerCB::~CraftingInventoryPlayerCB()
{
	delete craftMatrix;
	craftMatrix = NULL;
}

void CraftingInventoryPlayerCB::onCraftMatrixChanged(IInventory* var1)
{
	JArray<int > var2 = JArray<int >(9);
	for(int var3 = 0; var3 < 3; ++var3) {
	for(int var4 = 0; var4 < 3; ++var4) {
	int var5 = -1;
	if(var3 < 2 && var4 < 2) {
	ItemStack* var6 = this->craftMatrix->getStackInSlot(var3 + var4 * 2);
	if(var6 != NULL) {
	var5 = var6->itemID;
	}
	}
	var2[var3 + var4 * 3] = var5;
	}
	}
	this->craftResult->setInventorySlotContents(0, CraftingManager::getInstance()->craft(var2));
}

void CraftingInventoryPlayerCB::onCraftGuiClosed(EntityPlayer* var1)
{
	CraftingInventoryCB::onCraftGuiClosed(var1);
	for(int var2 = 0; var2 < 9; ++var2) {
	ItemStack* var3 = this->craftMatrix->getStackInSlot(var2);
	if(var3 != NULL) {
	var1->dropPlayerItem(var3);
	}
	}
}

void CraftingInventoryPlayerCB::initStatics()
{
}
