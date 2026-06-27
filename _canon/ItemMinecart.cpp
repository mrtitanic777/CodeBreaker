#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemMinecart.h"
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
#include "EntityMinecart.h"
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

ItemMinecart::ItemMinecart(int var1, int var2)
	: Item(var1)
	, minecartType(0)
{
	this->maxStackSize = 1;
	this->minecartType = var2;
}

ItemMinecart::~ItemMinecart()
{
}

bool ItemMinecart::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
{
	int var8 = var3->getBlockId(var4, var5, var6);
	if(var8 == Block::s_minecartTrack->blockID) {
	if(!var3->multiplayerWorld) {
	var3->entityJoinedWorld(new EntityMinecart(var3, (double)((float)var4 + 0.5F), (double)((float)var5 + 0.5F), (double)((float)var6 + 0.5F), this->minecartType));
	}
	--var1->stackSize;
	return true;
	} else {
	return false;
	}
}

void ItemMinecart::initStatics()
{
}
