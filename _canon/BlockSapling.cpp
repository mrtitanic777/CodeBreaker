#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockSapling.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSapling.h"
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
#include "WorldGenBigTree.h"
#include "WorldGenTrees.h"
#include "WorldGenerator.h"
#include "WorldProvider.h"

BlockSapling::BlockSapling(int var1, int var2)
	: BlockFlower(var1, var2)
{
	float var3 = 0.4F;
	this->setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, var3 * 2.0F, 0.5F + var3);
}

BlockSapling::~BlockSapling()
{
}

void BlockSapling::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	BlockFlower::updateTick(var1, var2, var3, var4, var5);
	if(var1->getBlockLightValue(var2, var3 + 1, var4) >= 9 && var5->nextInt(5) == 0) {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if(var6 < 15) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, var6 + 1);
	} else {
	var1->setBlock(var2, var3, var4, 0);
	JObject var7 = new WorldGenTrees();
	if(var5->nextInt(10) == 0) {
	var7 = new WorldGenBigTree();
	}
	if(!((WorldGenerator*)var7)->generate(var1, var5, var2, var3, var4)) {
	var1->setBlock(var2, var3, var4, this->blockID);
	}
	}
	}
}

void BlockSapling::initStatics()
{
}
