#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldIso.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldIso.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BufferedImage.h"
#include "CanvasIsomPreview.h"
#include "Chunk.h"
#include "ChunkLoader.h"
#include "ChunkProviderIso.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EnumOS1.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "IsoImageBuffer.h"
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

WorldIso::WorldIso(CanvasIsomPreview* var1, File* var2, const JavaString& var3)
	: World(var2, var3)
	, A(NULL)
{
	this->A = var1;
}

WorldIso::~WorldIso()
{
}

IChunkProvider* WorldIso::createChunkProvider(File* var1)
{
	return new ChunkProviderIso(this, new ChunkLoader(var1, false));
}

void WorldIso::initStatics()
{
}
