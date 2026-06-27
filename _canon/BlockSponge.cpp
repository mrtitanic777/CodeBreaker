#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockSponge.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSponge.h"
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

BlockSponge::BlockSponge(int var1)
	: Block(var1, Material::s_sponge)
{
	this->blockIndexInTexture = 48;
}

BlockSponge::~BlockSponge()
{
}

void BlockSponge::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	unsigned char var5 = 2;
	for(int var6 = var2 - var5; var6 <= var2 + var5; ++var6) {
	for(int var7 = var3 - var5; var7 <= var3 + var5; ++var7) {
	for(int var8 = var4 - var5; var8 <= var4 + var5; ++var8) {
	if(var1->getBlockMaterial(var6, var7, var8) == Material::s_water) {
	}
	}
	}
	}
}

void BlockSponge::onBlockRemoval(World* var1, int var2, int var3, int var4)
{
	unsigned char var5 = 2;
	for(int var6 = var2 - var5; var6 <= var2 + var5; ++var6) {
	for(int var7 = var3 - var5; var7 <= var3 + var5; ++var7) {
	for(int var8 = var4 - var5; var8 <= var4 + var5; ++var8) {
	var1->notifyBlocksOfNeighborChange(var6, var7, var8, var1->getBlockId(var6, var7, var8));
	}
	}
	}
}

void BlockSponge::initStatics()
{
}
