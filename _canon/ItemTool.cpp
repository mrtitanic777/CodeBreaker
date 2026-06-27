#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemTool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemTool.h"
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

ItemTool::ItemTool(int var1, int var2, int var3, JArray<Block* > var4)
	: Item(var1)
	, efficiencyOnProperMaterial(0)
	, damageVsEntity(0)
	, ingredientQuality(0)
{
	this->efficiencyOnProperMaterial = 4.0F;
	this->ingredientQuality = var3;
	this->blocksEffectiveAgainst = var4;
	this->maxStackSize = 1;
	this->maxDamage = 32 << var3;
	if(var3 == 3) {
	this->maxDamage *= 4;
	}
	this->efficiencyOnProperMaterial = (float)((var3 + 1) * 2);
	this->damageVsEntity = var2 + var3;
}

ItemTool::~ItemTool()
{
}

float ItemTool::getStrVsBlock(ItemStack* var1, Block* var2)
{
	for(int var3 = 0; var3 < this->blocksEffectiveAgainst.length; ++var3) {
	if(this->blocksEffectiveAgainst[var3] == var2) {
	return this->efficiencyOnProperMaterial;
	}
	}
	return 1.0F;
}

void ItemTool::hitEntity(ItemStack* var1, EntityLiving* var2)
{
	var1->damageItem(2);
}

void ItemTool::hitBlock(ItemStack* var1, int var2, int var3, int var4, int var5)
{
	var1->damageItem(1);
}

int ItemTool::getDamageVsEntity(Entity* var1)
{
	return this->damageVsEntity;
}

bool ItemTool::isFull3D()
{
	return true;
}

void ItemTool::initStatics()
{
}
