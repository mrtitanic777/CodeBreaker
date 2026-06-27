#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderSorter.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "ByteBuffer.h"
#include "Chunk.h"
#include "Class.h"
#include "Comparator.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IntBuffer.h"
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
#include "Tessellator.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

RenderSorter::RenderSorter(EntityPlayer* var1)
	: entityPlayer(NULL)
{
	this->entityPlayer = var1;
}

RenderSorter::~RenderSorter()
{
}

int RenderSorter::a(WorldRenderer* var1, WorldRenderer* var2)
{
	bool var3 = var1->isVisibleFromCamera;
	bool var4 = var2->isVisibleFromCamera;
	if(var3 && !var4) {
	return 1;
	} else if(var4 && !var3) {
	return -1;
	} else {
	double var5 = (double)var1->distanceToEntitySquared(this->entityPlayer);
	double var7 = (double)var2->distanceToEntitySquared(this->entityPlayer);
	return var5 < var7 ? 1 : (var5 > var7 ? -1 : (var1->chunkIndex < var2->chunkIndex ? 1 : -1));
	}
}

int RenderSorter::compare(JObject var1, JObject var2)
{
	return this->a((WorldRenderer*)var1, (WorldRenderer*)var2);
}

void RenderSorter::initStatics()
{
}
