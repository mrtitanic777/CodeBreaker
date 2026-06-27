#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Frustrum.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Frustrum.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "ClippingHelper.h"
#include "ClippingHelperImplementation.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "ICamera.h"
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

Frustrum::Frustrum()
	: clippingHelper(NULL)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
{
	this->clippingHelper = ClippingHelperImplementation::getInstance();
}

Frustrum::~Frustrum()
{
}

void Frustrum::setPosition(double var1, double var3, double var5)
{
	this->xPosition = var1;
	this->yPosition = var3;
	this->zPosition = var5;
}

bool Frustrum::isBoxInFrustum(double var1, double var3, double var5, double var7, double var9, double var11)
{
	return this->clippingHelper->isBoxInFrustum(var1 - this->xPosition, var3 - this->yPosition, var5 - this->zPosition, var7 - this->xPosition, var9 - this->yPosition, var11 - this->zPosition);
}

bool Frustrum::isBoundingBoxInFrustum(AxisAlignedBB* var1)
{
	return this->isBoxInFrustum(var1->minX, var1->minY, var1->minZ, var1->maxX, var1->maxY, var1->maxZ);
}

void Frustrum::initStatics()
{
}
