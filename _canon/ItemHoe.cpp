#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemHoe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemHoe.h"
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

ItemHoe::ItemHoe(int var1, int var2)
	: Item(var1)
{
	this->maxStackSize = 1;
	this->maxDamage = 32 << var2;
}

ItemHoe::~ItemHoe()
{
}

bool ItemHoe::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7)
{
	int var8 = var3->getBlockId(var4, var5, var6);
	Material* var9 = var3->getBlockMaterial(var4, var5 + 1, var6);
	if((var9->isSolid() || var8 != Block::s_grass->blockID) && var8 != Block::s_dirt->blockID) {
	return false;
	} else {
	Block* var10 = Block::s_tilledField;
	var3->playSoundEffect((double)((float)var4 + 0.5F), (double)((float)var5 + 0.5F), (double)((float)var6 + 0.5F), var10->stepSound->getStepSound(), (var10->stepSound->getVolume() + 1.0F) / 2.0F, var10->stepSound->getPitch() * 0.8F);
	if(var3->multiplayerWorld) {
	return true;
	} else {
	var3->setBlockWithNotify(var4, var5, var6, var10->blockID);
	var1->damageItem(1);
	if(var3->rand->nextInt(8) == 0 && var8 == Block::s_grass->blockID) {
	unsigned char var11 = 1;
	for(int var12 = 0; var12 < var11; ++var12) {
	float var13 = 0.7F;
	float var14 = var3->rand->nextFloat() * var13 + (1.0F - var13) * 0.5F;
	float var15 = 1.2F;
	float var16 = var3->rand->nextFloat() * var13 + (1.0F - var13) * 0.5F;
	EntityItem* var17 = new EntityItem(var3, (double)((float)var4 + var14), (double)((float)var5 + var15), (double)((float)var6 + var16), new ItemStack(Item::s_seeds));
	var17->delayBeforeCanPickup = 10;
	var3->entityJoinedWorld(var17);
	}
	}
	return true;
	}
	}
}

bool ItemHoe::isFull3D()
{
	return true;
}

void ItemHoe::initStatics()
{
}
