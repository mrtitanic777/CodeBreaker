#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockGrass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockGrass.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "ColorizerGrass.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IBlockAccess.h"
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

BlockGrass::BlockGrass(int var1)
	: Block(var1, Material::s_ground)
{
	this->blockIndexInTexture = 3;
	this->setTickOnLoad(true);
}

BlockGrass::~BlockGrass()
{
}

int BlockGrass::getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	if(var5 == 1) {
	return 0;
	} else if(var5 == 0) {
	return 2;
	} else {
	Material* var6 = var1->getBlockMaterial(var2, var3 + 1, var4);
	return var6 != Material::s_snow && var6 != Material::s_builtSnow ? 3 : 68;
	}
}

int BlockGrass::colorMultiplier(IBlockAccess* var1, int var2, int var3, int var4)
{
	var1->getWorldChunkManager()->getBiomesGrid(var2, var4, 1, 1);
	double var5 = var1->getWorldChunkManager()->temperature[0];
	double var7 = var1->getWorldChunkManager()->humidity[0];
	return ColorizerGrass::getGrassColor(var5, var7);
}

void BlockGrass::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(var1->getBlockLightValue(var2, var3 + 1, var4) < 4 && var1->getBlockMaterial(var2, var3 + 1, var4)->getCanBlockGrass()) {
	if(var5->nextInt(4) != 0) {
	return;
	}
	var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
	} else if(var1->getBlockLightValue(var2, var3 + 1, var4) >= 9) {
	int var6 = var2 + var5->nextInt(3) - 1;
	int var7 = var3 + var5->nextInt(5) - 3;
	int var8 = var4 + var5->nextInt(3) - 1;
	if(var1->getBlockId(var6, var7, var8) == Block::s_dirt->blockID && var1->getBlockLightValue(var6, var7 + 1, var8) >= 4 && !var1->getBlockMaterial(var6, var7 + 1, var8)->getCanBlockGrass()) {
	var1->setBlockWithNotify(var6, var7, var8, Block::s_grass->blockID);
	}
	}
}

int BlockGrass::idDropped(int var1, Random* var2)
{
	return Block::s_dirt->idDropped(0, var2);
}

void BlockGrass::initStatics()
{
}
