#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemSaddle.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSaddle.h"
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
#include "EntityPig.h"
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

ItemSaddle::ItemSaddle(int var1)
	: Item(var1)
{
	this->maxStackSize = 1;
	this->maxDamage = 64;
}

ItemSaddle::~ItemSaddle()
{
}

void ItemSaddle::useItemOnEntity(ItemStack* var1, EntityLiving* var2)
{
	if((dynamic_cast<EntityPig*>(var2) != NULL)) {
	EntityPig* var3 = (EntityPig*)var2;
	if(!var3->rideable) {
	var3->rideable = true;
	--var1->stackSize;
	}
	}
}

void ItemSaddle::hitEntity(ItemStack* var1, EntityLiving* var2)
{
	this->useItemOnEntity(var1, var2);
}

void ItemSaddle::initStatics()
{
}
