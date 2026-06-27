#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldGenFire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenFire.h"
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
#include "WorldGenerator.h"
#include "WorldProvider.h"

WorldGenFire::~WorldGenFire()
{
}

bool WorldGenFire::generate(World* var1, Random* var2, int var3, int var4, int var5)
{
	for(int var6 = 0; var6 < 64; ++var6) {
	int var7 = var3 + var2->nextInt(8) - var2->nextInt(8);
	int var8 = var4 + var2->nextInt(4) - var2->nextInt(4);
	int var9 = var5 + var2->nextInt(8) - var2->nextInt(8);
	if(var1->getBlockId(var7, var8, var9) == 0 && var1->getBlockId(var7, var8 - 1, var9) == Block::s_bloodStone->blockID) {
	var1->setBlockWithNotify(var7, var8, var9, Block::s_fire->blockID);
	}
	}
	return true;
}

void WorldGenFire::initStatics()
{
}
