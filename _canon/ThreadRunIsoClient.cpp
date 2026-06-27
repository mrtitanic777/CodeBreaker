#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ThreadRunIsoClient.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ThreadRunIsoClient.h"
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
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EnumOS1.h"
#include "Exception.h"
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
#include "Thread.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

ThreadRunIsoClient::ThreadRunIsoClient(CanvasIsomPreview* var1)
	: canvas(NULL)
{
	this->canvas = var1;
}

ThreadRunIsoClient::~ThreadRunIsoClient()
{
}

void ThreadRunIsoClient::run()
{
	while(CanvasIsomPreview::getRunning(this->canvas)) {
	this->canvas->render();
	try {
	Thread::sleep(1LL);
	} catch (Exception*) {
	}
	}
}

void ThreadRunIsoClient::initStatics()
{
}
