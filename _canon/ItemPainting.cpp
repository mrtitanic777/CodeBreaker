#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemPainting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemPainting.h"
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
#include "EntityPainting.h"
#include "EntityPlayer.h"
#include "EnumArt.h"
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

ItemPainting::ItemPainting(int var1)
	: Item(var1)
{
	this->maxDamage = 64;
}

ItemPainting::~ItemPainting()
{
}

bool ItemPainting::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
{
	if(var7 == 0) {
	return false;
	} else if(var7 == 1) {
	return false;
	} else {
	unsigned char var8 = 0;
	if(var7 == 4) {
	var8 = 1;
	}
	if(var7 == 3) {
	var8 = 2;
	}
	if(var7 == 5) {
	var8 = 3;
	}
	EntityPainting* var9 = new EntityPainting(var3, var4, var5, var6, var8);
	if(var9->onValidSurface()) {
	var3->entityJoinedWorld(var9);
	--var1->stackSize;
	}
	return true;
	}
}

void ItemPainting::initStatics()
{
}
