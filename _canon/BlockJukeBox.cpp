#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockJukeBox.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockJukeBox.h"
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

BlockJukeBox::BlockJukeBox(int var1, int var2)
	: Block(var1, var2, Material::s_wood)
{
}

BlockJukeBox::~BlockJukeBox()
{
}

int BlockJukeBox::getBlockTextureFromSide(int var1)
{
	return this->blockIndexInTexture + (var1 == 1 ? 1 : 0);
}

bool BlockJukeBox::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5)
{
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if(var6 > 0) {
	this->ejectRecord(var1, var2, var3, var4, var6);
	return true;
	} else {
	return false;
	}
}

void BlockJukeBox::ejectRecord(World* var1, int var2, int var3, int var4, int var5)
{
	var1->playRecord((JavaString)NULL, var2, var3, var4);
	var1->setBlockMetadataWithNotify(var2, var3, var4, 0);
	int var6 = Item::s_record13->shiftedIndex + var5 - 1;
	float var7 = 0.7F;
	double var8 = (double)(var1->rand->nextFloat() * var7) + (double)(1.0F - var7) * 0.5;
	double var10 = (double)(var1->rand->nextFloat() * var7) + (double)(1.0F - var7) * 0.2 + 0.6;
	double var12 = (double)(var1->rand->nextFloat() * var7) + (double)(1.0F - var7) * 0.5;
	EntityItem* var14 = new EntityItem(var1, (double)var2 + var8, (double)var3 + var10, (double)var4 + var12, new ItemStack(var6));
	var14->delayBeforeCanPickup = 10;
	var1->entityJoinedWorld(var14);
}

void BlockJukeBox::dropBlockAsItemWithChance(World* var1, int var2, int var3, int var4, int var5, float var6)
{
	if(!var1->multiplayerWorld) {
	if(var5 > 0) {
	this->ejectRecord(var1, var2, var3, var4, var5);
	}
	Block::dropBlockAsItemWithChance(var1, var2, var3, var4, var5, var6);
	}
}

void BlockJukeBox::initStatics()
{
}
