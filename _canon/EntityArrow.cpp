#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityArrow.h"
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

EntityArrow::EntityArrow(World* var1)
	: Entity(var1)
	, xTile(0)
	, yTile(0)
	, zTile(0)
	, inTile(0)
	, inGround(0)
	, arrowShake(0)
	, shootingEntity(NULL)
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
	this->setSize(0.5F, 0.5F);
}

EntityArrow::EntityArrow(World* var1, double var2, double var4, double var6)
	: Entity(var1)
	, xTile(0)
	, yTile(0)
	, zTile(0)
	, inTile(0)
	, inGround(0)
	, arrowShake(0)
	, shootingEntity(NULL)
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
	this->setSize(0.5F, 0.5F);
	this->setPosition(var2, var4, var6);
	this->yOffset = 0.0F;
}

EntityArrow::EntityArrow(World* var1, EntityLiving* var2)
	: Entity(var1)
	, xTile(0)
	, yTile(0)
	, zTile(0)
	, inTile(0)
	, inGround(0)
	, arrowShake(0)
	, shootingEntity(NULL)
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
	this->shootingEntity = var2;
	this->setSize(0.5F, 0.5F);
	this->setLocationAndAngles(var2->posX, var2->posY + (double)var2->getEyeHeight(), var2->posZ, var2->rotationYaw, var2->rotationPitch);
	this->posX -= (double)(MathHelper::cos(this->rotationYaw / 180.0F * (float)3.1415927F) * 0.16F);
	this->posY -= (double)0.1F;
	this->posZ -= (double)(MathHelper::sin(this->rotationYaw / 180.0F * (float)3.1415927F) * 0.16F);
	this->setPosition(this->posX, this->posY, this->posZ);
	this->yOffset = 0.0F;
	this->motionX = (double)(-MathHelper::sin(this->rotationYaw / 180.0F * (float)3.1415927F) * MathHelper::cos(this->rotationPitch / 180.0F * (float)3.1415927F));
	this->motionZ = (double)(MathHelper::cos(this->rotationYaw / 180.0F * (float)3.1415927F) * MathHelper::cos(this->rotationPitch / 180.0F * (float)3.1415927F));
	this->motionY = (double)(-MathHelper::sin(this->rotationPitch / 180.0F * (float)3.1415927F));
	this->setArrowHeading(this->motionX, this->motionY, this->motionZ, 1.5F, 1.0F);
}

EntityArrow::~EntityArrow()
{
}

void EntityArrow::setArrowHeading(double var1, double var3, double var5, float var7, float var8)
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

void EntityArrow::setVelocity(double var1, double var3, double var5)
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

void EntityArrow::onUpdate()
{
	Entity::onUpdate();
	if(this->prevRotationPitch == 0.0F && this->prevRotationYaw == 0.0F) {
	float var1 = MathHelper::sqrt_double(this->motionX * this->motionX + this->motionZ * this->motionZ);
	this->prevRotationYaw = this->rotationYaw = (float)(atan2f(this->motionX, this->motionZ) * 180.0 / (double)((float)3.1415927F));
	this->prevRotationPitch = this->rotationPitch = (float)(atan2f(this->motionY, (double)var1) * 180.0 / (double)((float)3.1415927F));
	}
	if(this->arrowShake > 0) {
	--this->arrowShake;
	}
	if(this->inGround) {
	int var15 = this->worldObj->getBlockId(this->xTile, this->yTile, this->zTile);
	if(var15 == this->inTile) {
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
	Vec3D* var16 = Vec3D::createVector(this->posX, this->posY, this->posZ);
	Vec3D* var2 = Vec3D::createVector(this->posX + this->motionX, this->posY + this->motionY, this->posZ + this->motionZ);
	MovingObjectPosition* var3 = this->worldObj->rayTraceBlocks(var16, var2);
	var16 = Vec3D::createVector(this->posX, this->posY, this->posZ);
	var2 = Vec3D::createVector(this->posX + this->motionX, this->posY + this->motionY, this->posZ + this->motionZ);
	if(var3 != NULL) {
	var2 = Vec3D::createVector(var3->hitVec->xCoord, var3->hitVec->yCoord, var3->hitVec->zCoord);
	}
	Entity* var4 = NULL;
	JList<void*> var5 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this, this->boundingBox->addCoord(this->motionX, this->motionY, this->motionZ)->expands(1.0, 1.0, 1.0));
	double var6 = 0.0;
	float var10;
	for(int var8 = 0; var8 < var5.size(); ++var8) {
	Entity* var9 = (Entity*)var5[var8];
	if(var9->canBeCollidedWith() && (var9 != this->shootingEntity || this->ticksInAir >= 5)) {
	var10 = 0.3F;
	AxisAlignedBB* var11 = var9->boundingBox->expands((double)var10, (double)var10, (double)var10);
	MovingObjectPosition* var12 = var11->calculateIntercept(var16, var2);
	if(var12 != NULL) {
	double var13 = var16->distanceTo(var12->hitVec);
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
	float var17;
	if(var3 != NULL) {
	if(var3->entityHit != NULL) {
	if(var3->entityHit->canAttackEntity(this->shootingEntity, 4)) {
	this->worldObj->playSoundAtEntity(this,"random.drr",1.0F,1.2F / (this->rand->nextFloat() * 0.2F + 0.9F));
	this->setEntityDead();
	} else {
	this->motionX *= (double)-0.1F;
	this->motionY *= (double)-0.1F;
	this->motionZ *= (double)-0.1F;
	this->rotationYaw += 180.0F;
	this->prevRotationYaw += 180.0F;
	this->ticksInAir = 0;
	}
	} else {
	this->xTile = var3->blockX;
	this->yTile = var3->blockY;
	this->zTile = var3->blockZ;
	this->inTile = this->worldObj->getBlockId(this->xTile, this->yTile, this->zTile);
	this->motionX = (double)((float)(var3->hitVec->xCoord - this->posX));
	this->motionY = (double)((float)(var3->hitVec->yCoord - this->posY));
	this->motionZ = (double)((float)(var3->hitVec->zCoord - this->posZ));
	var17 = MathHelper::sqrt_double(this->motionX * this->motionX + this->motionY * this->motionY + this->motionZ * this->motionZ);
	this->posX -= this->motionX / (double)var17 * (double)0.05F;
	this->posY -= this->motionY / (double)var17 * (double)0.05F;
	this->posZ -= this->motionZ / (double)var17 * (double)0.05F;
	this->worldObj->playSoundAtEntity(this,"random.drr",1.0F,1.2F / (this->rand->nextFloat() * 0.2F + 0.9F));
	this->inGround = true;
	this->arrowShake = 7;
	}
	}
	this->posX += this->motionX;
	this->posY += this->motionY;
	this->posZ += this->motionZ;
	var17 = MathHelper::sqrt_double(this->motionX * this->motionX + this->motionZ * this->motionZ);
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
	var10 = 0.03F;
	if(this->handleWaterMovement()) {
	for(int var19 = 0; var19 < 4; ++var19) {
	float var20 = 0.25F;
	this->worldObj->spawnParticle("bubble", this->posX - this->motionX * (double)var20, this->posY - this->motionY * (double)var20, this->posZ - this->motionZ * (double)var20, this->motionX, this->motionY, this->motionZ);
	}
	var18 = 0.8F;
	}
	this->motionX *= (double)var18;
	this->motionY *= (double)var18;
	this->motionZ *= (double)var18;
	this->motionY -= (double)var10;
	this->setPosition(this->posX, this->posY, this->posZ);
}

void EntityArrow::writeEntityToNBT(NBTTagCompound* var1)
{
	var1->setShort("xTile", (short)this->xTile);
	var1->setShort("yTile", (short)this->yTile);
	var1->setShort("zTile", (short)this->zTile);
	var1->setByte("inTile", (unsigned char)this->inTile);
	var1->setByte("shake", (unsigned char)this->arrowShake);
	var1->setByte("inGround", (unsigned char)(this->inGround ? 1 : 0));
}

void EntityArrow::readEntityFromNBT(NBTTagCompound* var1)
{
	this->xTile = var1->getShort("xTile");
	this->yTile = var1->getShort("yTile");
	this->zTile = var1->getShort("zTile");
	this->inTile = var1->getByte("inTile") & 255;
	this->arrowShake = var1->getByte("shake") & 255;
	this->inGround = var1->getByte("inGround") == 1;
}

void EntityArrow::onCollideWithPlayer(EntityPlayer* var1)
{
	if(!this->worldObj->multiplayerWorld) {
	if(this->inGround && this->shootingEntity == var1 && this->arrowShake <= 0 && var1->inventory->addItemStackToInventory(new ItemStack(Item::s_arrow->shiftedIndex, 1))) {
	this->worldObj->playSoundAtEntity(this,"random.pop",0.2F,((this->rand->nextFloat() - this->rand->nextFloat()) * 0.7F + 1.0F) * 2.0F);
	var1->onItemPickup(this, 1);
	this->setEntityDead();
	}
	}
}

float EntityArrow::getYOffsetForRender()
{
	return 0.0F;
}

void EntityArrow::initStatics()
{
}
