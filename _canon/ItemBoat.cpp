#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemBoat.h"
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
#include "EntityBoat.h"
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
#include "MathHelper.h"
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

ItemBoat::ItemBoat(int var1)
	: Item(var1)
{
	this->maxStackSize = 1;
}

ItemBoat::~ItemBoat()
{
}

ItemStack* ItemBoat::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3)
{
	float var4 = 1.0F;
	float var5 = var3->prevRotationPitch + (var3->rotationPitch - var3->prevRotationPitch) * var4;
	float var6 = var3->prevRotationYaw + (var3->rotationYaw - var3->prevRotationYaw) * var4;
	double var7 = var3->prevPosX + (var3->posX - var3->prevPosX) * (double)var4;
	double var9 = var3->prevPosY + (var3->posY - var3->prevPosY) * (double)var4 + 1.62 - (double)var3->yOffset;
	double var11 = var3->prevPosZ + (var3->posZ - var3->prevPosZ) * (double)var4;
	Vec3D* var13 = Vec3D::createVector(var7, var9, var11);
	float var14 = MathHelper::cos(-var6 * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	float var15 = MathHelper::sin(-var6 * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	float var16 = -MathHelper::cos(-var5 * ((float)3.1415927F / 180.0F));
	float var17 = MathHelper::sin(-var5 * ((float)3.1415927F / 180.0F));
	float var18 = var15 * var16;
	float var20 = var14 * var16;
	double var21 = 5.0;
	Vec3D* var23 = var13->addVector((double)var18 * var21, (double)var17 * var21, (double)var20 * var21);
	MovingObjectPosition* var24 = var2->rayTraceBlocks(var13, var23, true);
	if(var24 == NULL) {
	return var1;
	} else {
	if(var24->typeOfHit == 0) {
	int var25 = var24->blockX;
	int var26 = var24->blockY;
	int var27 = var24->blockZ;
	if(!var2->multiplayerWorld) {
	var2->entityJoinedWorld(new EntityBoat(var2, (double)((float)var25 + 0.5F), (double)((float)var26 + 1.5F), (double)((float)var27 + 0.5F)));
	}
	--var1->stackSize;
	}
	return var1;
	}
}

void ItemBoat::initStatics()
{
}
