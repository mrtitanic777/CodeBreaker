#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntitySnowball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySnowball.h"
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
#include "EntityLiving.h"
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

EntitySnowball::EntitySnowball(World* var1)
	: Entity(var1)
	, xTile(0)
	, yTile(0)
	, zTile(0)
	, inTile(0)
	, inGround(0)
	, arrowShake(0)
	, thrower(NULL)
	, ticksInGround(0)
	, ticksInAir(0)
{
	this->xTile = -1;
	this->yTile = -1;
	this->zTile = -1;
	this->inTile = 0;
	this->inGround = false;
	this->arrowShake = 0;
	this->ticksInAir = 0;
	this->setSize(0.25F, 0.25F);
}

EntitySnowball::EntitySnowball(World* var1, EntityLiving* var2)
	: Entity(var1)
	, xTile(0)
	, yTile(0)
	, zTile(0)
	, inTile(0)
	, inGround(0)
	, arrowShake(0)
	, thrower(NULL)
	, ticksInGround(0)
	, ticksInAir(0)
{
	this->xTile = -1;
	this->yTile = -1;
	this->zTile = -1;
	this->inTile = 0;
	this->inGround = false;
	this->arrowShake = 0;
	this->ticksInAir = 0;
	this->thrower = var2;
	this->setSize(0.25F, 0.25F);
	this->setLocationAndAngles(var2->posX, var2->posY + (double)var2->getEyeHeight(), var2->posZ, var2->rotationYaw, var2->rotationPitch);
	this->posX -= (double)(MathHelper::cos(this->rotationYaw / 180.0F * (float)3.1415927F) * 0.16F);
	this->posY -= (double)0.1F;
	this->posZ -= (double)(MathHelper::sin(this->rotationYaw / 180.0F * (float)3.1415927F) * 0.16F);
	this->setPosition(this->posX, this->posY, this->posZ);
	this->yOffset = 0.0F;
	float var3 = 0.4F;
	this->motionX = (double)(-MathHelper::sin(this->rotationYaw / 180.0F * (float)3.1415927F) * MathHelper::cos(this->rotationPitch / 180.0F * (float)3.1415927F) * var3);
	this->motionZ = (double)(MathHelper::cos(this->rotationYaw / 180.0F * (float)3.1415927F) * MathHelper::cos(this->rotationPitch / 180.0F * (float)3.1415927F) * var3);
	this->motionY = (double)(-MathHelper::sin(this->rotationPitch / 180.0F * (float)3.1415927F) * var3);
	this->setThrowableHeading(this->motionX, this->motionY, this->motionZ, 1.5F, 1.0F);
}

EntitySnowball::EntitySnowball(World* var1, double var2, double var4, double var6)
	: Entity(var1)
	, xTile(0)
	, yTile(0)
	, zTile(0)
	, inTile(0)
	, inGround(0)
	, arrowShake(0)
	, thrower(NULL)
	, ticksInGround(0)
	, ticksInAir(0)
{
	this->xTile = -1;
	this->yTile = -1;
	this->zTile = -1;
	this->inTile = 0;
	this->inGround = false;
	this->arrowShake = 0;
	this->ticksInAir = 0;
	this->ticksInGround = 0;
	this->setSize(0.25F, 0.25F);
	this->setPosition(var2, var4, var6);
	this->yOffset = 0.0F;
}

EntitySnowball::~EntitySnowball()
{
}

bool EntitySnowball::isInRangeToRenderDist(double var1)
{
	double var3 = this->boundingBox->getAverageEdgeLength() * 4.0;
	var3 *= 64.0;
	return var1 < var3 * var3;
}

void EntitySnowball::setThrowableHeading(double var1, double var3, double var5, float var7, float var8)
{
	float var9 = MathHelper::sqrt_double(var1 * var1 + var3 * var3 + var5 * var5);
	var1 /= (double)var9;
	var3 /= (double)var9;
	var5 /= (double)var9;
	var1 += this->rand->nextGaussian() * (double)0.0075F * (double)var8;
	var3 += this->rand->nextGaussian() * (double)0.0075F * (double)var8;
	var5 += this->rand->nextGaussian() * (double)0.0075F * (double)var8;
	var1 *= (double)var7;
	var3 *= (double)var7;
	var5 *= (double)var7;
	this->motionX = var1;
	this->motionY = var3;
	this->motionZ = var5;
	float var10 = MathHelper::sqrt_double(var1 * var1 + var5 * var5);
	this->prevRotationYaw = this->rotationYaw = (float)(atan2f(var1, var5) * 180.0 / (double)((float)3.1415927F));
	this->prevRotationPitch = this->rotationPitch = (float)(atan2f(var3, (double)var10) * 180.0 / (double)((float)3.1415927F));
	this->ticksInGround = 0;
}

void EntitySnowball::setVelocity(double var1, double var3, double var5)
{
	this->motionX = var1;
	this->motionY = var3;
	this->motionZ = var5;
	if(this->prevRotationPitch == 0.0F && this->prevRotationYaw == 0.0F) {
	float var7 = MathHelper::sqrt_double(var1 * var1 + var5 * var5);
	this->prevRotationYaw = this->rotationYaw = (float)(atan2f(var1, var5) * 180.0 / (double)((float)3.1415927F));
	this->prevRotationPitch = this->rotationPitch = (float)(atan2f(var3, (double)var7) * 180.0 / (double)((float)3.1415927F));
	}
}

void EntitySnowball::onUpdate()
{
	this->lastTickPosX = this->posX;
	this->lastTickPosY = this->posY;
	this->lastTickPosZ = this->posZ;
	Entity::onUpdate();
	if(this->arrowShake > 0) {
	--this->arrowShake;
	}
	if(this->inGround) {
	int var1 = this->worldObj->getBlockId(this->xTile, this->yTile, this->zTile);
	if(var1 == this->inTile) {
	++this->ticksInGround;
	if(this->ticksInGround == 1200) {
	this->setEntityDead();
	}
	return;
	}
	this->inGround = false;
	this->motionX *= (double)(this->rand->nextFloat() * 0.2F);
	this->motionY *= (double)(this->rand->nextFloat() * 0.2F);
	this->motionZ *= (double)(this->rand->nextFloat() * 0.2F);
	this->ticksInGround = 0;
	this->ticksInAir = 0;
	} else {
	++this->ticksInAir;
	}
	Vec3D* var15 = Vec3D::createVector(this->posX, this->posY, this->posZ);
	Vec3D* var2 = Vec3D::createVector(this->posX + this->motionX, this->posY + this->motionY, this->posZ + this->motionZ);
	MovingObjectPosition* var3 = this->worldObj->rayTraceBlocks(var15, var2);
	var15 = Vec3D::createVector(this->posX, this->posY, this->posZ);
	var2 = Vec3D::createVector(this->posX + this->motionX, this->posY + this->motionY, this->posZ + this->motionZ);
	if(var3 != NULL) {
	var2 = Vec3D::createVector(var3->hitVec->xCoord, var3->hitVec->yCoord, var3->hitVec->zCoord);
	}
	if(!this->worldObj->multiplayerWorld) {
	Entity* var4 = NULL;
	JList<void*> var5 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this, this->boundingBox->addCoord(this->motionX, this->motionY, this->motionZ)->expands(1.0, 1.0, 1.0));
	double var6 = 0.0;
	for(int var8 = 0; var8 < var5.size(); ++var8) {
	Entity* var9 = (Entity*)var5[var8];
	if(var9->canBeCollidedWith() && (var9 != this->thrower || this->ticksInAir >= 5)) {
	float var10 = 0.3F;
	AxisAlignedBB* var11 = var9->boundingBox->expands((double)var10, (double)var10, (double)var10);
	MovingObjectPosition* var12 = var11->calculateIntercept(var15, var2);
	if(var12 != NULL) {
	double var13 = var15->distanceTo(var12->hitVec);
	if(var13 < var6 || var6 == 0.0) {
	var4 = var9;
	var6 = var13;
	}
	}
	}
	}
	if(var4 != NULL) {
	var3 = new MovingObjectPosition(var4);
	}
	}
	if(var3 != NULL) {
	if(var3->entityHit != NULL && var3->entityHit->canAttackEntity(this->thrower, 0)) {
	}
	for(int var16 = 0; var16 < 8; ++var16) {
	this->worldObj->spawnParticle("snowballpoof", this->posX, this->posY, this->posZ, 0.0, 0.0, 0.0);
	}
	this->setEntityDead();
	}
	this->posX += this->motionX;
	this->posY += this->motionY;
	this->posZ += this->motionZ;
	float var17 = MathHelper::sqrt_double(this->motionX * this->motionX + this->motionZ * this->motionZ);
	this->rotationYaw = (float)(atan2f(this->motionX, this->motionZ) * 180.0 / (double)((float)3.1415927F));
	for(this->rotationPitch = (float)(atan2f(this->motionY, (double)var17) * 180.0 / (double)((float)3.1415927F)); this->rotationPitch - this->prevRotationPitch < -180.0F; this->prevRotationPitch -= 360.0F) {
	}
	while(this->rotationPitch - this->prevRotationPitch >= 180.0F) {
	this->prevRotationPitch += 360.0F;
	}
	while(this->rotationYaw - this->prevRotationYaw < -180.0F) {
	this->prevRotationYaw -= 360.0F;
	}
	while(this->rotationYaw - this->prevRotationYaw >= 180.0F) {
	this->prevRotationYaw += 360.0F;
	}
	this->rotationPitch = this->prevRotationPitch + (this->rotationPitch - this->prevRotationPitch) * 0.2F;
	this->rotationYaw = this->prevRotationYaw + (this->rotationYaw - this->prevRotationYaw) * 0.2F;
	float var18 = 0.99F;
	float var19 = 0.03F;
	if(this->handleWaterMovement()) {
	for(int var7 = 0; var7 < 4; ++var7) {
	float var20 = 0.25F;
	this->worldObj->spawnParticle("bubble", this->posX - this->motionX * (double)var20, this->posY - this->motionY * (double)var20, this->posZ - this->motionZ * (double)var20, this->motionX, this->motionY, this->motionZ);
	}
	var18 = 0.8F;
	}
	this->motionX *= (double)var18;
	this->motionY *= (double)var18;
	this->motionZ *= (double)var18;
	this->motionY -= (double)var19;
	this->setPosition(this->posX, this->posY, this->posZ);
}

void EntitySnowball::writeEntityToNBT(NBTTagCompound* var1)
{
	var1->setShort("xTile", (short)this->xTile);
	var1->setShort("yTile", (short)this->yTile);
	var1->setShort("zTile", (short)this->zTile);
	var1->setByte("inTile", (unsigned char)this->inTile);
	var1->setByte("shake", (unsigned char)this->arrowShake);
	var1->setByte("inGround", (unsigned char)(this->inGround ? 1 : 0));
}

void EntitySnowball::readEntityFromNBT(NBTTagCompound* var1)
{
	this->xTile = var1->getShort("xTile");
	this->yTile = var1->getShort("yTile");
	this->zTile = var1->getShort("zTile");
	this->inTile = var1->getByte("inTile") & 255;
	this->arrowShake = var1->getByte("shake") & 255;
	this->inGround = var1->getByte("inGround") == 1;
}

void EntitySnowball::onCollideWithPlayer(EntityPlayer* var1)
{
	if(this->inGround && this->thrower == var1 && this->arrowShake <= 0 && var1->inventory->addItemStackToInventory(new ItemStack(Item::s_arrow->shiftedIndex, 1))) {
	this->worldObj->playSoundAtEntity(this,"random.pop",0.2F,((this->rand->nextFloat() - this->rand->nextFloat()) * 0.7F + 1.0F) * 2.0F);
	var1->onItemPickup(this, 1);
	this->setEntityDead();
	}
}

float EntitySnowball::getYOffsetForRender()
{
	return 0.0F;
}

void EntitySnowball::initStatics()
{
}
