#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemBlock.h"
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

ItemBlock::ItemBlock(int var1)
	: Item(var1)
	, blockID(0)
{
	this->blockID = var1 + 256;
	this->setIconIndex(Block::s_blocksList[var1 + 256]->getBlockTextureFromSide(2));
}

ItemBlock::~ItemBlock()
{
}

bool ItemBlock::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
{
	if(var3->getBlockId(var4, var5, var6) == Block::s_snow->blockID) {
	var7 = 0;
	} else {
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
	}
	if(var1->stackSize == 0) {
	return false;
	} else {
	if(var3->canBlockBePlacedAt(this->blockID, var4, var5, var6, false)) {
	Block* var8 = Block::s_blocksList[this->blockID];
	if(var3->setBlockWithNotify(var4, var5, var6, this->blockID)) {
	if (Block::s_blocksList[this->blockID] != NULL) { Block::s_blocksList[this->blockID]->onBlockPlaced(var3, var4, var5, var6, var7); }
	if (Block::s_blocksList[this->blockID] != NULL) { Block::s_blocksList[this->blockID]->onBlockPlacedBy(var3, var4, var5, var6, var2); }
	var3->playSoundEffect((double)((float)var4 + 0.5F), (double)((float)var5 + 0.5F), (double)((float)var6 + 0.5F), var8->stepSound->getStepSound(), (var8->stepSound->getVolume() + 1.0F) / 2.0F, var8->stepSound->getPitch() * 0.8F);
	--var1->stackSize;
	}
	}
	return true;
	}
}

void ItemBlock::initStatics()
{
}
