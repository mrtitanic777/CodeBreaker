#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldGenCactus.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenCactus.h"
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

WorldGenCactus::~WorldGenCactus()
{
}

bool WorldGenCactus::generate(World* var1, Random* var2, int var3, int var4, int var5)
{
	for(int var6 = 0; var6 < 10; ++var6) {
	int var7 = var3 + var2->nextInt(8) - var2->nextInt(8);
	int var8 = var4 + var2->nextInt(4) - var2->nextInt(4);
	int var9 = var5 + var2->nextInt(8) - var2->nextInt(8);
	if(var1->getBlockId(var7, var8, var9) == 0) {
	int var10 = 1 + var2->nextInt(var2->nextInt(3) + 1);
	for(int var11 = 0; var11 < var10; ++var11) {
	if(Block::s_cactus->canBlockStay(var1, var7, var8 + var11, var9)) {
	var1->setBlock(var7, var8 + var11, var9, Block::s_cactus->blockID);
	}
	}
	}
	}
	return true;
}

void WorldGenCactus::initStatics()
{
}
