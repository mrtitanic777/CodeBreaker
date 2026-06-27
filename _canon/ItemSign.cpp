#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSign.h"
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
#include "TileEntitySign.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

ItemSign::ItemSign(int var1)
	: Item(var1)
{
	this->maxDamage = 64;
	this->maxStackSize = 1;
}

ItemSign::~ItemSign()
{
}

bool ItemSign::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
{
	if(var7 == 0) {
	return false;
	} else if(!var3->getBlockMaterial(var4, var5, var6)->isSolid()) {
	return false;
	} else {
	if(var7 == 1) {
	++var5;
	}
	if(var7 == 2) {
	--var6;
	}
	if(var7 == 3) {
	++var6;
	}
	if(var7 == 4) {
	--var4;
	}
	if(var7 == 5) {
	++var4;
	}
	if(!Block::s_signPost->canPlaceBlockAt(var3, var4, var5, var6)) {
	return false;
	} else {
	if(var7 == 1) {
	var3->setBlockAndMetadataWithNotify(var4, var5, var6, Block::s_signPost->blockID, MathHelper::floor_double((double)((var2->rotationYaw + 180.0F) * 16.0F / 360.0F) + 0.5) & 15);
	} else {
	var3->setBlockAndMetadataWithNotify(var4, var5, var6, Block::s_signWall->blockID, var7);
	}
	--var1->stackSize;
	TileEntitySign* var8 = (TileEntitySign*)var3->getBlockTileEntity(var4, var5, var6);
	if(var8 != NULL) {
	var2->displayGUIEditSign(var8);
	}
	return true;
	}
	}
}

void ItemSign::initStatics()
{
}
