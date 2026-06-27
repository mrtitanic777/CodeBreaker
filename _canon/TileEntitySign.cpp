#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TileEntitySign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntitySign.h"
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

TileEntitySign::TileEntitySign()
	: lineBeingEdited(0)
{
	this->signText = (JArrayInit<JavaString >(), "", "", "", "");
	this->lineBeingEdited = -1;
}

TileEntitySign::~TileEntitySign()
{
}

void TileEntitySign::writeToNBT(NBTTagCompound* var1)
{
	TileEntity::writeToNBT(var1);
	var1->setString("Text1", this->signText[0]);
	var1->setString("Text2", this->signText[1]);
	var1->setString("Text3", this->signText[2]);
	var1->setString("Text4", this->signText[3]);
}

void TileEntitySign::readFromNBT(NBTTagCompound* var1)
{
	TileEntity::readFromNBT(var1);
	for(int var2 = 0; var2 < 4; ++var2) {
	this->signText[var2] = var1->getString((JCat() + "Text" + (var2 + 1)));
	if(this->signText[var2].length() > 15) {
	this->signText[var2] = this->signText[var2].substring(0, 15);
	}
	}
}

void TileEntitySign::initStatics()
{
}
