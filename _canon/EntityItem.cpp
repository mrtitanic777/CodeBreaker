#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityItem.h"
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

EntityItem::EntityItem(World* var1, double var2, double var4, double var6, ItemStack* var8)
	: Entity(var1)
	, item(NULL)
	, bobTimer(0)
	, age(0)
	, delayBeforeCanPickup(0)
	, health(0)
	, bobOffset(0)
{
	this->age = 0;
	this->health = 5;
	this->bobOffset = (float)(javaMathRandom() * 3.1415927F * 2.0);
	this->setSize(0.25F, 0.25F);
	this->yOffset = this->height / 2.0F;
	this->setPosition(var2, var4, var6);
	this->item = var8;
	this->rotationYaw = (float)(javaMathRandom() * 360.0);
	this->motionX = (double)((float)(javaMathRandom() * (double)0.2F - (double)0.1F));
	this->motionY = (double)0.2F;
	this->motionZ = (double)((float)(javaMathRandom() * (double)0.2F - (double)0.1F));
	this->entityWalks = false;
}

EntityItem::EntityItem(World* var1)
	: Entity(var1)
	, item(NULL)
	, bobTimer(0)
	, age(0)
	, delayBeforeCanPickup(0)
	, health(0)
	, bobOffset(0)
{
	this->age = 0;
	this->health = 5;
	this->bobOffset = (float)(javaMathRandom() * 3.1415927F * 2.0);
	this->setSize(0.25F, 0.25F);
	this->yOffset = this->height / 2.0F;
}

EntityItem::~EntityItem()
{
}

void EntityItem::onUpdate()
{
	Entity::onUpdate();
	if(this->delayBeforeCanPickup > 0) {
	--this->delayBeforeCanPickup;
	}
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	this->motionY -= (double)0.04F;
	if(this->worldObj->getBlockMaterial(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->posY), MathHelper::floor_double(this->posZ)) == Material::s_lava) {
	this->motionY = (double)0.2F;
	this->motionX = (double)((this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F);
	this->motionZ = (double)((this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F);
	this->worldObj->playSoundAtEntity(this,"random.fizz",0.4F,2.0F + this->rand->nextFloat() * 0.4F);
	}
	this->pushOutOfBlock(this->posX, this->posY, this->posZ);
	this->handleWaterMovement();
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	float var1 = 0.98F;
	if(this->onGround) {
	var1 = 0.1F * 0.1F * 58.8F;
	int var2 = this->worldObj->getBlockId(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->boundingBox->minY) - 1, MathHelper::floor_double(this->posZ));
	if(var2 > 0) {
	var1 = Block::s_blocksList[var2]->slipperiness * 0.98F;
	}
	}
	this->motionX *= (double)var1;
	this->motionY *= (double)0.98F;
	this->motionZ *= (double)var1;
	if(this->onGround) {
	this->motionY *= -0.5;
	}
	++this->bobTimer;
	++this->age;
	if(this->age >= 6000) {
	this->setEntityDead();
	}
}

bool EntityItem::handleWaterMovement()
{
	return this->worldObj->handleMaterialAcceleration(this->boundingBox, Material::s_water, this);
}

bool EntityItem::pushOutOfBlock(double var1, double var3, double var5)
{
	int var7 = MathHelper::floor_double(var1);
	int var8 = MathHelper::floor_double(var3);
	int var9 = MathHelper::floor_double(var5);
	double var10 = var1 - (double)var7;
	double var12 = var3 - (double)var8;
	double var14 = var5 - (double)var9;
	if(Block::s_opaqueCubeLookup[this->worldObj->getBlockId(var7, var8, var9)]) {
	bool var16 = !Block::s_opaqueCubeLookup[this->worldObj->getBlockId(var7 - 1, var8, var9)];
	bool var17 = !Block::s_opaqueCubeLookup[this->worldObj->getBlockId(var7 + 1, var8, var9)];
	bool var18 = !Block::s_opaqueCubeLookup[this->worldObj->getBlockId(var7, var8 - 1, var9)];
	bool var19 = !Block::s_opaqueCubeLookup[this->worldObj->getBlockId(var7, var8 + 1, var9)];
	bool var20 = !Block::s_opaqueCubeLookup[this->worldObj->getBlockId(var7, var8, var9 - 1)];
	bool var21 = !Block::s_opaqueCubeLookup[this->worldObj->getBlockId(var7, var8, var9 + 1)];
	unsigned char var22 = -1;
	double var23 = 9999.0;
	if(var16 && var10 < var23) {
	var23 = var10;
	var22 = 0;
	}
	if(var17 && 1.0 - var10 < var23) {
	var23 = 1.0 - var10;
	var22 = 1;
	}
	if(var18 && var12 < var23) {
	var23 = var12;
	var22 = 2;
	}
	if(var19 && 1.0 - var12 < var23) {
	var23 = 1.0 - var12;
	var22 = 3;
	}
	if(var20 && var14 < var23) {
	var23 = var14;
	var22 = 4;
	}
	if(var21 && 1.0 - var14 < var23) {
	var23 = 1.0 - var14;
	var22 = 5;
	}
	float var25 = this->rand->nextFloat() * 0.2F + 0.1F;
	if(var22 == 0) {
	this->motionX = (double)(-var25);
	}
	if(var22 == 1) {
	this->motionX = (double)var25;
	}
	if(var22 == 2) {
	this->motionY = (double)(-var25);
	}
	if(var22 == 3) {
	this->motionY = (double)var25;
	}
	if(var22 == 4) {
	this->motionZ = (double)(-var25);
	}
	if(var22 == 5) {
	this->motionZ = (double)var25;
	}
	}
	return false;
}

void EntityItem::dealFireDamage(int var1)
{
	this->canAttackEntity((Entity*)NULL, var1);
}

bool EntityItem::canAttackEntity(Entity* var1, int var2)
{
	this->setBeenAttacked();
	this->health -= var2;
	if(this->health <= 0) {
	this->setEntityDead();
	}
	return false;
}

void EntityItem::writeEntityToNBT(NBTTagCompound* var1)
{
	var1->setShort("Health", (short)((unsigned char)this->health));
	var1->setShort("Age", (short)this->age);
	var1->setCompoundTag("Item", this->item->writeToNBT(new NBTTagCompound()));
}

void EntityItem::readEntityFromNBT(NBTTagCompound* var1)
{
	this->health = var1->getShort("Health") & 255;
	this->age = var1->getShort("Age");
	NBTTagCompound* var2 = var1->getCompoundTag("Item");
	this->item = new ItemStack(var2);
}

void EntityItem::onCollideWithPlayer(EntityPlayer* var1)
{
	if(!this->worldObj->multiplayerWorld) {
	int var2 = this->item->stackSize;
	if(this->delayBeforeCanPickup == 0 && var1->inventory->addItemStackToInventory(this->item)) {
	this->worldObj->playSoundAtEntity(this,"random.pop",0.2F,((this->rand->nextFloat() - this->rand->nextFloat()) * 0.7F + 1.0F) * 2.0F);
	var1->onItemPickup(this, var2);
	this->setEntityDead();
	}
	}
}

void EntityItem::initStatics()
{
}
