#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemRedstone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemRedstone.h"
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
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

ItemRedstone::ItemRedstone(int var1)
	: Item(var1)
{
}

ItemRedstone::~ItemRedstone()
{
}

bool ItemRedstone::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
{
	if(var7 == 0) {
	--var5;
	}
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
	if(var3->getBlockId(var4, var5, var6) != 0) {
	return false;
	} else {
	if(Block::s_redstoneWire->canPlaceBlockAt(var3, var4, var5, var6)) {
	--var1->stackSize;
	var3->setBlockWithNotify(var4, var5, var6, Block::s_redstoneWire->blockID);
	}
	return true;
	}
}

void ItemRedstone::initStatics()
{
}
