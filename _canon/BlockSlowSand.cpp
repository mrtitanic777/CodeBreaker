#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockSlowSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSlowSand.h"
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

BlockSlowSand::BlockSlowSand(int var1, int var2)
	: Block(var1, var2, Material::s_sand)
{
}

BlockSlowSand::~BlockSlowSand()
{
}

AxisAlignedBB* BlockSlowSand::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	float var5 = 2.0F / 16.0F;
	return AxisAlignedBB::getBoundingBoxFromPool((double)var2, (double)var3, (double)var4, (double)(var2 + 1), (double)((float)(var3 + 1) - var5), (double)(var4 + 1));
}

void BlockSlowSand::onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5)
{
	var5->motionX *= 0.4;
	var5->motionZ *= 0.4;
}

void BlockSlowSand::initStatics()
{
}
