#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Teleporter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Teleporter.h"
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

Teleporter::Teleporter()
	: random(NULL)
{
	this->random = new Random();
}

Teleporter::~Teleporter()
{
}

void Teleporter::placeInPortal(World* var1, Entity* var2)
{
	if(!this->placeInExistingPortal(var1, var2)) {
	this->makePortal(var1, var2);
	this->placeInExistingPortal(var1, var2);
	}
}

bool Teleporter::placeInExistingPortal(World* var1, Entity* var2)
{
	short var3 = 128;
	double var4 = -1.0;
	int var6 = 0;
	int var7 = 0;
	int var8 = 0;
	int var9 = MathHelper::floor_double(var2->posX);
	int var10 = MathHelper::floor_double(var2->posZ);
	double var18;
	for(int var11 = var9 - var3; var11 <= var9 + var3; ++var11) {
	double var12 = (double)var11 + 0.5 - var2->posX;
	for(int var14 = var10 - var3; var14 <= var10 + var3; ++var14) {
	double var15 = (double)var14 + 0.5 - var2->posZ;
	for(int var17 = 127; var17 >= 0; --var17) {
	if(var1->getBlockId(var11, var17, var14) == Block::s_portal->blockID) {
	while(var1->getBlockId(var11, var17 - 1, var14) == Block::s_portal->blockID) {
	--var17;
	}
	var18 = (double)var17 + 0.5 - var2->posY;
	double var20 = var12 * var12 + var18 * var18 + var15 * var15;
	if(var4 < 0.0 || var20 < var4) {
	var4 = var20;
	var6 = var11;
	var7 = var17;
	var8 = var14;
	}
	}
	}
	}
	}
	if(var4 >= 0.0) {
	double var22 = (double)var6 + 0.5;
	double var16 = (double)var7 + 0.5;
	var18 = (double)var8 + 0.5;
	if(var1->getBlockId(var6 - 1, var7, var8) == Block::s_portal->blockID) {
	var22 -= 0.5;
	}
	if(var1->getBlockId(var6 + 1, var7, var8) == Block::s_portal->blockID) {
	var22 += 0.5;
	}
	if(var1->getBlockId(var6, var7, var8 - 1) == Block::s_portal->blockID) {
	var18 -= 0.5;
	}
	if(var1->getBlockId(var6, var7, var8 + 1) == Block::s_portal->blockID) {
	var18 += 0.5;
	}
	System::out.println((JCat() + "Teleporting to " + var22 + ", " + var16 + ", " + var18));
	var2->setLocationAndAngles(var22, var16, var18, var2->rotationYaw, 0.0F);
	var2->motionX = var2->motionY = var2->motionZ = 0.0;
	return true;
	} else {
	return false;
	}
}

bool Teleporter::makePortal(World* var1, Entity* var2)
{
	unsigned char var3 = 16;
	double var4 = -1.0;
	int var6 = MathHelper::floor_double(var2->posX);
	int var7 = MathHelper::floor_double(var2->posY);
	int var8 = MathHelper::floor_double(var2->posZ);
	int var9 = var6;
	int var10 = var7;
	int var11 = var8;
	int var12 = 0;
	int var13 = this->random->nextInt(4);
	int var14;
	double var15;
	int var17;
	double var18;
	int var20;
	int var21;
	int var22;
	int var23;
	int var24;
	int var25;
	int var26;
	int var27;
	int var28;
	double var32;
	double var33;
	for(var14 = var6 - var3; var14 <= var6 + var3; ++var14) {
	var15 = (double)var14 + 0.5 - var2->posX;
	for(var17 = var8 - var3; var17 <= var8 + var3; ++var17) {
	var18 = (double)var17 + 0.5 - var2->posZ;
	for(var20 = 127; var20 >= 0; --var20) {
	if(var1->getBlockId(var14, var20, var17) == 0) {
	while(var20 > 0 && var1->getBlockId(var14, var20 - 1, var17) == 0) {
	--var20;
	}
	for(var21 = var13; var21 < var13 + 4; ++var21) {
	var22 = var21 % 2;
	var23 = 1 - var22;
	if(var21 % 4 >= 2) {
	var22 = -var22;
	var23 = -var23;
	}
	for(var24 = 0; var24 < 3; ++var24) {
	for(var25 = 0; var25 < 4; ++var25) {
	for(var26 = -1; var26 < 4; ++var26) {
	var27 = var14 + (var25 - 1) * var22 + var24 * var23;
	var28 = var20 + var26;
	int var29 = var17 + (var25 - 1) * var23 - var24 * var22;
	if(var26 < 0 && !var1->getBlockMaterial(var27, var28, var29)->isSolid() || var26 >= 0 && var1->getBlockId(var27, var28, var29) != 0) {
	goto label293_cont;
	}
	}
	}
	}
	var32 = (double)var20 + 0.5 - var2->posY;
	var33 = var15 * var15 + var32 * var32 + var18 * var18;
	if(var4 < 0.0 || var33 < var4) {
	var4 = var33;
	var9 = var14;
	var10 = var20;
	var11 = var17;
	var12 = var21 % 4;
	}
	}
	}
	label293_cont:;
	}
	}
	}
	if(var4 < 0.0) {
	for(var14 = var6 - var3; var14 <= var6 + var3; ++var14) {
	var15 = (double)var14 + 0.5 - var2->posX;
	for(var17 = var8 - var3; var17 <= var8 + var3; ++var17) {
	var18 = (double)var17 + 0.5 - var2->posZ;
	for(var20 = 127; var20 >= 0; --var20) {
	if(var1->getBlockId(var14, var20, var17) == 0) {
	while(var1->getBlockId(var14, var20 - 1, var17) == 0) {
	--var20;
	}
	for(var21 = var13; var21 < var13 + 2; ++var21) {
	var22 = var21 % 2;
	var23 = 1 - var22;
	for(var24 = 0; var24 < 4; ++var24) {
	for(var25 = -1; var25 < 4; ++var25) {
	var26 = var14 + (var24 - 1) * var22;
	var27 = var20 + var25;
	var28 = var17 + (var24 - 1) * var23;
	if(var25 < 0 && !var1->getBlockMaterial(var26, var27, var28)->isSolid() || var25 >= 0 && var1->getBlockId(var26, var27, var28) != 0) {
	goto label231_cont;
	}
	}
	}
	var32 = (double)var20 + 0.5 - var2->posY;
	var33 = var15 * var15 + var32 * var32 + var18 * var18;
	if(var4 < 0.0 || var33 < var4) {
	var4 = var33;
	var9 = var14;
	var10 = var20;
	var11 = var17;
	var12 = var21 % 2;
	}
	}
	}
	label231_cont:;
	}
	}
	}
	}
	int var30 = var9;
	int var16 = var10;
	var17 = var11;
	int var31 = var12 % 2;
	int var19 = 1 - var31;
	if(var12 % 4 >= 2) {
	var31 = -var31;
	var19 = -var19;
	}
	bool var34;
	if(var4 < 0.0) {
	if(var10 < 70) {
	var10 = 70;
	}
	if(var10 > 118) {
	var10 = 118;
	}
	var16 = var10;
	for(var20 = -1; var20 <= 1; ++var20) {
	for(var21 = 1; var21 < 3; ++var21) {
	for(var22 = -1; var22 < 3; ++var22) {
	var23 = var30 + (var21 - 1) * var31 + var20 * var19;
	var24 = var16 + var22;
	var25 = var17 + (var21 - 1) * var19 - var20 * var31;
	var34 = var22 < 0;
	var1->setBlockWithNotify(var23, var24, var25, var34 ? Block::s_obsidian->blockID : 0);
	}
	}
	}
	}
	for(var20 = 0; var20 < 4; ++var20) {
	var1->editingBlocks = true;
	for(var21 = 0; var21 < 4; ++var21) {
	for(var22 = -1; var22 < 4; ++var22) {
	var23 = var30 + (var21 - 1) * var31;
	var24 = var16 + var22;
	var25 = var17 + (var21 - 1) * var19;
	var34 = var21 == 0 || var21 == 3 || var22 == -1 || var22 == 3;
	var1->setBlockWithNotify(var23, var24, var25, var34 ? Block::s_obsidian->blockID : Block::s_portal->blockID);
	}
	}
	var1->editingBlocks = false;
	for(var21 = 0; var21 < 4; ++var21) {
	for(var22 = -1; var22 < 4; ++var22) {
	var23 = var30 + (var21 - 1) * var31;
	var24 = var16 + var22;
	var25 = var17 + (var21 - 1) * var19;
	var1->notifyBlocksOfNeighborChange(var23, var24, var25, var1->getBlockId(var23, var24, var25));
	}
	}
	}
	return true;
}

void Teleporter::initStatics()
{
}
