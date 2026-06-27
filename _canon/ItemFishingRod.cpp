#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemFishingRod.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemFishingRod.h"
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

ItemFishingRod::ItemFishingRod(int var1)
	: Item(var1)
{
	this->maxDamage = 64;
}

ItemFishingRod::~ItemFishingRod()
{
}

bool ItemFishingRod::isFull3D()
{
	return true;
}

bool ItemFishingRod::shouldRotateAroundWhenRendering()
{
	return true;
}

ItemStack* ItemFishingRod::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3)
{
	if(var3->fishEntity != NULL) {
	int var4 = var3->fishEntity->catchFish();
	var1->damageItem(var4);
	var3->swingItem();
	} else {
	var2->playSoundAtEntity(var3,"random.bow",0.5F,0.4F / (s_itemRand->nextFloat() * 0.4F + 0.8F));
	if(!var2->multiplayerWorld) {
	var2->entityJoinedWorld(new EntityFish(var2, var3));
	}
	var3->swingItem();
	}
	return var1;
}

void ItemFishingRod::initStatics()
{
}
