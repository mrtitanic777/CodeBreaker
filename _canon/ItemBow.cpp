#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemBow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemBow.h"
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
#include "EntityArrow.h"
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

ItemBow::ItemBow(int var1)
	: Item(var1)
{
	this->maxStackSize = 1;
}

ItemBow::~ItemBow()
{
}

ItemStack* ItemBow::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3)
{
	if(var3->inventory->consumeInventoryItem(Item::s_arrow->shiftedIndex)) {
	var2->playSoundAtEntity(var3,"random.bow",1.0F,1.0F / (s_itemRand->nextFloat() * 0.4F + 0.8F));
	if(!var2->multiplayerWorld) {
	var2->entityJoinedWorld(new EntityArrow(var2, var3));
	}
	}
	return var1;
}

void ItemBow::initStatics()
{
}
