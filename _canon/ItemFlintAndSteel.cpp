#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemFlintAndSteel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemFlintAndSteel.h"
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

ItemFlintAndSteel::ItemFlintAndSteel(int var1)
	: Item(var1)
{
	this->maxStackSize = 1;
	this->maxDamage = 64;
}

ItemFlintAndSteel::~ItemFlintAndSteel()
{
}

bool ItemFlintAndSteel::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
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
	int var8 = var3->getBlockId(var4, var5, var6);
	if(var8 == 0) {
	var3->playSoundEffect((double)var4 + 0.5,(double)var5 + 0.5,(double)var6 + 0.5,"fire.ignite",1.0F,s_itemRand->nextFloat() * 0.4F + 0.8F);
	var3->setBlockWithNotify(var4, var5, var6, Block::s_fire->blockID);
	}
	var1->damageItem(1);
	return true;
}

void ItemFlintAndSteel::initStatics()
{
}
