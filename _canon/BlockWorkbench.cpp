#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockWorkbench.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockWorkbench.h"
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

BlockWorkbench::BlockWorkbench(int var1)
	: Block(var1, Material::s_wood)
{
	this->blockIndexInTexture = 59;
}

BlockWorkbench::~BlockWorkbench()
{
}

int BlockWorkbench::getBlockTextureFromSide(int var1)
{
	return var1 == 1 ? this->blockIndexInTexture - 16 : (var1 == 0 ? Block::s_planks->getBlockTextureFromSide(0) : (var1 != 2 && var1 != 4 ? this->blockIndexInTexture : this->blockIndexInTexture + 1));
}

bool BlockWorkbench::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5)
{
	var5->displayWorkbenchGUI();
	return true;
}

void BlockWorkbench::initStatics()
{
}
