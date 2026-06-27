#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityPainting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPainting.h"
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
#include "EnumArt.h"
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

EntityPainting::EntityPainting(World* var1)
	: Entity(var1)
	, tickCounter(0)
	, direction(0)
	, tileX(0)
	, tileY(0)
	, tileZ(0)
	, art()
{
	this->tickCounter = 0;
	this->direction = 0;
	this->yOffset = 0.0F;
	this->setSize(0.5F, 0.5F);
}

EntityPainting::EntityPainting(World* var1, int var2, int var3, int var4, int var5)
	: Entity(var1)
	, tickCounter(0)
	, direction(0)
	, tileX(0)
	, tileY(0)
	, tileZ(0)
	, art()
{
	// inlined this(var1) -- C++03 lacks ctor delegation
	this->tickCounter = 0;
	this->direction = 0;
	this->yOffset = 0.0F;
	this->setSize(0.5F, 0.5F);
	this->tileX = var2;
	this->tileY = var3;
	this->tileZ = var4;
	JList<EnumArt> var6;
	JArray<EnumArt > var7 = EnumArt::values();
	int var8 = var7.length;
	for(int var9 = 0; var9 < var8; ++var9) {
	EnumArt var10 = var7[var9];
	this->art = var10;
	this->setDirection(var5);
	if(this->onValidSurface()) {
	var6.push_back(var10);
	}
	}
	if(var6.size() > 0) {
	this->art = var6[this->rand->nextInt(var6.size())];
	}
	this->setDirection(var5);
}

EntityPainting::~EntityPainting()
{
}

void EntityPainting::setDirection(int var1)
{
	this->direction = var1;
	this->prevRotationYaw = this->rotationYaw = (float)(var1 * 90);
	float var2 = (float)this->art.sizeX;
	float var3 = (float)this->art.sizeY;
	float var4 = (float)this->art.sizeX;
	if(var1 != 0 && var1 != 2) {
	var2 = 0.5F;
	} else {
	var4 = 0.5F;
	}
	var2 /= 32.0F;
	var3 /= 32.0F;
	var4 /= 32.0F;
	float var5 = (float)this->tileX + 0.5F;
	float var6 = (float)this->tileY + 0.5F;
	float var7 = (float)this->tileZ + 0.5F;
	float var8 = 9.0F / 16.0F;
	if(var1 == 0) {
	var7 -= var8;
	}
	if(var1 == 1) {
	var5 -= var8;
	}
	if(var1 == 2) {
	var7 += var8;
	}
	if(var1 == 3) {
	var5 += var8;
	}
	if(var1 == 0) {
	var5 -= this->getOffset(this->art.sizeX);
	}
	if(var1 == 1) {
	var7 += this->getOffset(this->art.sizeX);
	}
	if(var1 == 2) {
	var5 += this->getOffset(this->art.sizeX);
	}
	if(var1 == 3) {
	var7 -= this->getOffset(this->art.sizeX);
	}
	var6 += this->getOffset(this->art.sizeY);
	this->setPosition((double)var5, (double)var6, (double)var7);
	float var9 = -(0.1F / 16.0F);
	this->boundingBox->setBounds((double)(var5 - var2 - var9), (double)(var6 - var3 - var9), (double)(var7 - var4 - var9), (double)(var5 + var2 + var9), (double)(var6 + var3 + var9), (double)(var7 + var4 + var9));
}

float EntityPainting::getOffset(int var1)
{
	return var1 == 32 ? 0.5F : (var1 == 64 ? 0.5F : 0.0F);
}

void EntityPainting::onUpdate()
{
	if(this->tickCounter++ == 100 && !this->onValidSurface()) {
	this->tickCounter = 0;
	this->setEntityDead();
	this->worldObj->entityJoinedWorld(new EntityItem(this->worldObj, this->posX, this->posY, this->posZ, new ItemStack(Item::s_painting)));
	}
}

bool EntityPainting::onValidSurface()
{
	if(this->worldObj->getCollidingBoundingBoxes(this, this->boundingBox).size() > 0) {
	return false;
	} else {
	int var1 = this->art.sizeX / 16;
	int var2 = this->art.sizeY / 16;
	int var3 = this->tileX;
	int var4 = this->tileY;
	int var5 = this->tileZ;
	if(this->direction == 0) {
	var3 = MathHelper::floor_double(this->posX - (double)((float)this->art.sizeX / 32.0F));
	}
	if(this->direction == 1) {
	var5 = MathHelper::floor_double(this->posZ - (double)((float)this->art.sizeX / 32.0F));
	}
	if(this->direction == 2) {
	var3 = MathHelper::floor_double(this->posX - (double)((float)this->art.sizeX / 32.0F));
	}
	if(this->direction == 3) {
	var5 = MathHelper::floor_double(this->posZ - (double)((float)this->art.sizeX / 32.0F));
	}
	var4 = MathHelper::floor_double(this->posY - (double)((float)this->art.sizeY / 32.0F));
	int var7;
	for(int var6 = 0; var6 < var1; ++var6) {
	for(var7 = 0; var7 < var2; ++var7) {
	Material* var8;
	if(this->direction != 0 && this->direction != 2) {
	var8 = this->worldObj->getBlockMaterial(this->tileX, var4 + var7, var5 + var6);
	} else {
	var8 = this->worldObj->getBlockMaterial(var3 + var6, var4 + var7, this->tileZ);
	}
	if(!var8->isSolid()) {
	return false;
	}
	}
	}
	JList<void*> var9 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this, this->boundingBox);
	for(var7 = 0; var7 < var9.size(); ++var7) {
	if((dynamic_cast<EntityPainting*>((Entity*)var9[var7]) != NULL)) {
	return false;
	}
	}
	return true;
	}
}

bool EntityPainting::canBeCollidedWith()
{
	return true;
}

bool EntityPainting::canAttackEntity(Entity* var1, int var2)
{
	this->setEntityDead();
	this->setBeenAttacked();
	this->worldObj->entityJoinedWorld(new EntityItem(this->worldObj, this->posX, this->posY, this->posZ, new ItemStack(Item::s_painting)));
	return true;
}

void EntityPainting::writeEntityToNBT(NBTTagCompound* var1)
{
	var1->setByte("Dir", (unsigned char)this->direction);
	var1->setString("Motive", this->art.title);
	var1->setInteger("TileX", this->tileX);
	var1->setInteger("TileY", this->tileY);
	var1->setInteger("TileZ", this->tileZ);
}

void EntityPainting::readEntityFromNBT(NBTTagCompound* var1)
{
	this->direction = var1->getByte("Dir");
	this->tileX = var1->getInteger("TileX");
	this->tileY = var1->getInteger("TileY");
	this->tileZ = var1->getInteger("TileZ");
	JavaString var2 = var1->getString("Motive");
	JArray<EnumArt > var3 = EnumArt::values();
	int var4 = var3.length;
	bool var4b = false;
	for(int var5 = 0; var5 < var4; ++var5) {
	EnumArt var6 = var3[var5];
	if(var6.title.equals(var2)) {
	this->art = var6;
	var4b = true;
	}
	}
	if(!var4b) {
	this->art = EnumArt::byId(EnumArt::Kebab);
	}
	this->setDirection(this->direction);
}
