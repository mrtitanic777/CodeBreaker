#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntitySorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySorter.h"
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

EntitySorter::EntitySorter(Entity* var1)
	: baseEntity(NULL)
{
	this->baseEntity = var1;
}

EntitySorter::~EntitySorter()
{
}

int EntitySorter::a(WorldRenderer* var1, WorldRenderer* var2)
{
	return var1->distanceToEntitySquared(this->baseEntity) < var2->distanceToEntitySquared(this->baseEntity) ? -1 : 1;
}

int EntitySorter::compare(JObject var1, JObject var2)
{
	return this->a((WorldRenderer*)var1, (WorldRenderer*)var2);
}

void EntitySorter::initStatics()
{
}
