#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MovementInput.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MovementInput.h"
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

MovementInput::MovementInput()
	: moveStrafe(0)
	, moveForward(0)
	, field_1177_c(0)
	, jump(0)
	, sneak(0)
{
	this->moveStrafe = 0.0F;
	this->moveForward = 0.0F;
	this->field_1177_c = false;
	this->jump = false;
	this->sneak = false;
}

MovementInput::~MovementInput()
{
}

void MovementInput::updatePlayerMoveState(EntityPlayer* var1)
{
}

void MovementInput::resetKeyState()
{
}

void MovementInput::setKeyState(int var1, bool var2)
{
}

void MovementInput::initStatics()
{
}
