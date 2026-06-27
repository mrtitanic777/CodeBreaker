#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockTNT.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockTNT.h"
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
#include "EntityTNTPrimed.h"
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

BlockTNT::BlockTNT(int var1, int var2)
	: Block(var1, var2, Material::s_tnt)
{
}

BlockTNT::~BlockTNT()
{
}

int BlockTNT::getBlockTextureFromSide(int var1)
{
	return var1 == 0 ? this->blockIndexInTexture + 2 : (var1 == 1 ? this->blockIndexInTexture + 1 : this->blockIndexInTexture);
}

void BlockTNT::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	if(var5 > 0 && Block::s_blocksList[var5]->canProvidePower() && var1->isBlockIndirectlyGettingPowered(var2, var3, var4)) {
	this->onBlockDestroyedByPlayer(var1, var2, var3, var4, 0);
	var1->setBlockWithNotify(var2, var3, var4, 0);
	}
}

int BlockTNT::quantityDropped(Random* var1)
{
	return 0;
}

void BlockTNT::onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4)
{
	EntityTNTPrimed* var5 = new EntityTNTPrimed(var1, (double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F));
	var5->fuse = var1->rand->nextInt(var5->fuse / 4) + var5->fuse / 8;
	var1->entityJoinedWorld(var5);
}

void BlockTNT::onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5)
{
	if(!var1->multiplayerWorld) {
	EntityTNTPrimed* var6 = new EntityTNTPrimed(var1, (double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F));
	var1->entityJoinedWorld(var6);
	var1->playSoundAtEntity(var6, "random.fuse", 1.0F, 1.0F);
	}
}

void BlockTNT::initStatics()
{
}
