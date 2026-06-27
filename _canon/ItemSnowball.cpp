#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemSnowball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSnowball.h"
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
#include "EntitySnowball.h"
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

ItemSnowball::ItemSnowball(int var1)
	: Item(var1)
{
	this->maxStackSize = 16;
}

ItemSnowball::~ItemSnowball()
{
}

ItemStack* ItemSnowball::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3)
{
	--var1->stackSize;
	var2->playSoundAtEntity(var3,"random.bow",0.5F,0.4F / (s_itemRand->nextFloat() * 0.4F + 0.8F));
	if(!var2->multiplayerWorld) {
	var2->entityJoinedWorld(new EntitySnowball(var2, var3));
	}
	return var1;
}

void ItemSnowball::initStatics()
{
}
