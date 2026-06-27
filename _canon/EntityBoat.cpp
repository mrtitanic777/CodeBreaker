#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityBoat.h"
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

EntityBoat::EntityBoat(World* var1)
	: Entity(var1)
	, damageTaken(0)
	, timeSinceHit(0)
	, forwardDirection(0)
	, boatPosRotationIncrements(0)
	, boatX(0)
	, boatY(0)
	, boatZ(0)
	, boatYaw(0)
	, boatPitch(0)
	, velocityX(0)
	, velocityY(0)
	, velocityZ(0)
{
	this->damageTaken = 0;
	this->timeSinceHit = 0;
	this->forwardDirection = 1;
	this->preventEntitySpawning = true;
	this->setSize(1.5F, 0.6F);
	this->yOffset = this->height / 2.0F;
	this->entityWalks = false;
}

EntityBoat::EntityBoat(World* var1, double var2, double var4, double var6)
	: Entity(var1)
	, damageTaken(0)
	, timeSinceHit(0)
	, forwardDirection(0)
	, boatPosRotationIncrements(0)
	, boatX(0)
	, boatY(0)
	, boatZ(0)
	, boatYaw(0)
	, boatPitch(0)
	, velocityX(0)
	, velocityY(0)
	, velocityZ(0)
{
	// inlined this(var1) -- C++03 lacks ctor delegation
	this->damageTaken = 0;
	this->timeSinceHit = 0;
	this->forwardDirection = 1;
	this->preventEntitySpawning = true;
	this->setSize(1.5F, 0.6F);
	this->yOffset = this->height / 2.0F;
	this->entityWalks = false;
	this->setPosition(var2, var4 + (double)this->yOffset, var6);
	this->motionX = 0.0;
	this->motionY = 0.0;
	this->motionZ = 0.0;
	this->prevPosX = var2;
	this->prevPosY = var4;
	this->prevPosZ = var6;
}

EntityBoat::~EntityBoat()
{
}

AxisAlignedBB* EntityBoat::getBoundingBox(Entity* var1)
{
	return var1->boundingBox;
}

AxisAlignedBB* EntityBoat::getCollisionBox()
{
	return this->boundingBox;
}

bool EntityBoat::canBePushed()
{
	return true;
}

double EntityBoat::getMountedYOffset()
{
	return (double)this->height * 0.0 - (double)0.3F;
}

bool EntityBoat::canAttackEntity(Entity* var1, int var2)
{
	if(!this->worldObj->multiplayerWorld && !this->isDead) {
	this->forwardDirection = -this->forwardDirection;
	this->timeSinceHit = 10;
	this->damageTaken += var2 * 10;
	this->setBeenAttacked();
	if(this->damageTaken > 40) {
	int var3;
	for(var3 = 0; var3 < 3; ++var3) {
	this->dropItemWithOffset(Block::s_planks->blockID, 1, 0.0F);
	}
	for(var3 = 0; var3 < 2; ++var3) {
	this->dropItemWithOffset(Item::s_stick->shiftedIndex, 1, 0.0F);
	}
	this->setEntityDead();
	}
	return true;
	} else {
	return true;
	}
}

void EntityBoat::performHurtAnimation()
{
	this->forwardDirection = -this->forwardDirection;
	this->timeSinceHit = 10;
	this->damageTaken += this->damageTaken * 10;
}

bool EntityBoat::canBeCollidedWith()
{
	return !this->isDead;
}

void EntityBoat::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9)
{
	this->boatX = var1;
	this->boatY = var3;
	this->boatZ = var5;
	this->boatYaw = (double)var7;
	this->boatPitch = (double)var8;
	this->boatPosRotationIncrements = var9 + 4;
	this->motionX = this->velocityX;
	this->motionY = this->velocityY;
	this->motionZ = this->velocityZ;
}

void EntityBoat::setVelocity(double var1, double var3, double var5)
{
	this->velocityX = this->motionX = var1;
	this->velocityY = this->motionY = var3;
	this->velocityZ = this->motionZ = var5;
}

void EntityBoat::onUpdate()
{
	Entity::onUpdate();
	if(this->timeSinceHit > 0) {
	--this->timeSinceHit;
	}
	if(this->damageTaken > 0) {
	--this->damageTaken;
	}
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	unsigned char var1 = 5;
	double var2 = 0.0;
	for(int var4 = 0; var4 < var1; ++var4) {
	double var5 = this->boundingBox->minY + (this->boundingBox->maxY - this->boundingBox->minY) * (double)(var4 + 0) / (double)var1 - 0.125;
	double var7 = this->boundingBox->minY + (this->boundingBox->maxY - this->boundingBox->minY) * (double)(var4 + 1) / (double)var1 - 0.125;
	AxisAlignedBB* var9 = AxisAlignedBB::getBoundingBoxFromPool(this->boundingBox->minX, var5, this->boundingBox->minZ, this->boundingBox->maxX, var7, this->boundingBox->maxZ);
	if(this->worldObj->isMaterialInBBExtended(var9, Material::s_water)) {
	var2 += 1.0 / (double)var1;
	}
	}
	double var6;
	double var8;
	double var10;
	double var23;
	if(this->worldObj->multiplayerWorld) {
	if(this->boatPosRotationIncrements > 0) {
	var23 = this->posX + (this->boatX - this->posX) / (double)this->boatPosRotationIncrements;
	var6 = this->posY + (this->boatY - this->posY) / (double)this->boatPosRotationIncrements;
	var8 = this->posZ + (this->boatZ - this->posZ) / (double)this->boatPosRotationIncrements;
	for(var10 = this->boatYaw - (double)this->rotationYaw; var10 < -180.0; var10 += 360.0) {
	}
	while(var10 >= 180.0) {
	var10 -= 360.0;
	}
	this->rotationYaw = (float)((double)this->rotationYaw + var10 / (double)this->boatPosRotationIncrements);
	this->rotationPitch = (float)((double)this->rotationPitch + (this->boatPitch - (double)this->rotationPitch) / (double)this->boatPosRotationIncrements);
	--this->boatPosRotationIncrements;
	this->setPosition(var23, var6, var8);
	this->setRotation(this->rotationYaw, this->rotationPitch);
	} else {
	var23 = this->posX + this->motionX;
	var6 = this->posY + this->motionY;
	var8 = this->posZ + this->motionZ;
	this->setPosition(var23, var6, var8);
	if(this->onGround) {
	this->motionX *= 0.5;
	this->motionY *= 0.5;
	this->motionZ *= 0.5;
	}
	this->motionX *= (double)0.99F;
	this->motionY *= (double)0.95F;
	this->motionZ *= (double)0.99F;
	}
	} else {
	var23 = var2 * 2.0 - 1.0;
	this->motionY += (double)0.04F * var23;
	if(this->riddenByEntity != NULL) {
	this->motionX += this->riddenByEntity->motionX * 0.2;
	this->motionZ += this->riddenByEntity->motionZ * 0.2;
	}
	var6 = 0.4;
	if(this->motionX < -var6) {
	this->motionX = -var6;
	}
	if(this->motionX > var6) {
	this->motionX = var6;
	}
	if(this->motionZ < -var6) {
	this->motionZ = -var6;
	}
	if(this->motionZ > var6) {
	this->motionZ = var6;
	}
	if(this->onGround) {
	this->motionX *= 0.5;
	this->motionY *= 0.5;
	this->motionZ *= 0.5;
	}
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	var8 = sqrtf(this->motionX * this->motionX + this->motionZ * this->motionZ);
	double var12;
	if(var8 > 0.15) {
	var10 = cosf((double)this->rotationYaw * 3.1415927F / 180.0);
	var12 = sinf((double)this->rotationYaw * 3.1415927F / 180.0);
	for(int var14 = 0; (double)var14 < 1.0 + var8 * 60.0; ++var14) {
	double var15 = (double)(this->rand->nextFloat() * 2.0F - 1.0F);
	double var17 = (double)(this->rand->nextInt(2) * 2 - 1) * 0.7;
	double var19;
	double var21;
	if(this->rand->nextBoolean()) {
	var19 = this->posX - var10 * var15 * 0.8 + var12 * var17;
	var21 = this->posZ - var12 * var15 * 0.8 - var10 * var17;
	this->worldObj->spawnParticle("splash", var19, this->posY - 0.125, var21, this->motionX, this->motionY, this->motionZ);
	} else {
	var19 = this->posX + var10 + var12 * var15 * 0.7;
	var21 = this->posZ + var12 - var10 * var15 * 0.7;
	this->worldObj->spawnParticle("splash", var19, this->posY - 0.125, var21, this->motionX, this->motionY, this->motionZ);
	}
	}
	}
	if(this->isCollidedHorizontally && var8 > 0.15) {
	if(!this->worldObj->multiplayerWorld) {
	this->setEntityDead();
	int var24;
	for(var24 = 0; var24 < 3; ++var24) {
	this->dropItemWithOffset(Block::s_planks->blockID, 1, 0.0F);
	}
	for(var24 = 0; var24 < 2; ++var24) {
	this->dropItemWithOffset(Item::s_stick->shiftedIndex, 1, 0.0F);
	}
	}
	} else {
	this->motionX *= (double)0.99F;
	this->motionY *= (double)0.95F;
	this->motionZ *= (double)0.99F;
	}
	this->rotationPitch = 0.0F;
	var10 = (double)this->rotationYaw;
	var12 = this->prevPosX - this->posX;
	double var25 = this->prevPosZ - this->posZ;
	if(var12 * var12 + var25 * var25 > 0.001) {
	var10 = (double)((float)(atan2f(var25, var12) * 180.0 / 3.1415927F));
	}
	double var16;
	for(var16 = var10 - (double)this->rotationYaw; var16 >= 180.0; var16 -= 360.0) {
	}
	while(var16 < -180.0) {
	var16 += 360.0;
	}
	if(var16 > 20.0) {
	var16 = 20.0;
	}
	if(var16 < -20.0) {
	var16 = -20.0;
	}
	this->rotationYaw = (float)((double)this->rotationYaw + var16);
	this->setRotation(this->rotationYaw, this->rotationPitch);
	JList<void*> var18 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this, this->boundingBox->expands((double)0.2F, 0.0, (double)0.2F));
	if(var18 != NULL && var18.size() > 0) {
	for(int var26 = 0; var26 < var18.size(); ++var26) {
	Entity* var20 = (Entity*)var18[var26];
	if(var20 != this->riddenByEntity && var20->canBePushed() && (dynamic_cast<EntityBoat*>(var20) != NULL)) {
	var20->applyEntityCollision(this);
	}
	}
	}
	if(this->riddenByEntity != NULL && this->riddenByEntity->isDead) {
	this->riddenByEntity = NULL;
	}
	}
}

void EntityBoat::updateRiderPosition()
{
	if(this->riddenByEntity != NULL) {
	double var1 = cosf((double)this->rotationYaw * 3.1415927F / 180.0) * 0.4;
	double var3 = sinf((double)this->rotationYaw * 3.1415927F / 180.0) * 0.4;
	this->riddenByEntity->setPosition(this->posX + var1, this->posY + this->getMountedYOffset() + this->riddenByEntity->getYOffset(), this->posZ + var3);
	}
}

void EntityBoat::writeEntityToNBT(NBTTagCompound* var1)
{
}

void EntityBoat::readEntityFromNBT(NBTTagCompound* var1)
{
}

float EntityBoat::getYOffsetForRender()
{
	return 0.0F;
}

bool EntityBoat::interact(EntityPlayer* var1)
{
	if(this->riddenByEntity != NULL && (dynamic_cast<EntityPlayer*>(this->riddenByEntity) != NULL) && this->riddenByEntity != var1) {
	return true;
	} else {
	if(!this->worldObj->multiplayerWorld) {
	var1->mountEntity(this);
	}
	return true;
	}
}

void EntityBoat::initStatics()
{
}
