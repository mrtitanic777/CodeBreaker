#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemSeeds.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSeeds.h"
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

ItemSeeds::ItemSeeds(int var1, int var2)
	: Item(var1)
	, cropBlockId(0)
{
	this->cropBlockId = var2;
}

ItemSeeds::~ItemSeeds()
{
}

bool ItemSeeds::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
{
	if(var7 != 1) {
	return false;
	} else {
	int var8 = var3->getBlockId(var4, var5, var6);
	if(var8 == Block::s_tilledField->blockID) {
	var3->setBlockWithNotify(var4, var5 + 1, var6, this->cropBlockId);
	--var1->stackSize;
	return true;
	} else {
	return false;
	}
	}
}

void ItemSeeds::initStatics()
{
}
