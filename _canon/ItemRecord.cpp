#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemRecord.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemRecord.h"
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

ItemRecord::ItemRecord(int var1, const JavaString& var2)
	: Item(var1)
{
	this->recordName = var2;
	this->maxStackSize = 1;
}

ItemRecord::~ItemRecord()
{
}

bool ItemRecord::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
{
	if(var3->getBlockId(var4, var5, var6) == Block::s_jukebox->blockID && var3->getBlockMetadata(var4, var5, var6) == 0) {
	var3->setBlockMetadataWithNotify(var4, var5, var6, this->shiftedIndex - Item::s_record13->shiftedIndex + 1);
	var3->playRecord(this->recordName, var4, var5, var6);
	--var1->stackSize;
	return true;
	} else {
	return false;
	}
}

void ItemRecord::initStatics()
{
}
