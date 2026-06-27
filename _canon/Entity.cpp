#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Entity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockFluids.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityList.h"
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
#include "NBTTagDouble.h"
#include "NBTTagFloat.h"
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

int Entity::s_nextEntityId = 0;

Entity::Entity(World* var1)
	: entityId(0)
	, renderDistanceWeight(0)
	, preventEntitySpawning(0)
	, riddenByEntity(NULL)
	, ridingEntity(NULL)
	, worldObj(NULL)
	, prevPosX(0)
	, prevPosY(0)
	, prevPosZ(0)
	, posX(0)
	, posY(0)
	, posZ(0)
	, motionX(0)
	, motionY(0)
	, motionZ(0)
	, rotationYaw(0)
	, rotationPitch(0)
	, prevRotationYaw(0)
	, prevRotationPitch(0)
	, boundingBox(NULL)
	, onGround(0)
	, isCollidedHorizontally(0)
	, isCollidedVertically(0)
	, isCollided(0)
	, velocityChanged(0)
	, field_9293_aM(0)
	, isDead(0)
	, yOffset(0)
	, width(0)
	, height(0)
	, prevDistanceWalkedModified(0)
	, distanceWalkedModified(0)
	, entityWalks(0)
	, fallDistance(0)
	, nextStepDistance(0)
	, lastTickPosX(0)
	, lastTickPosY(0)
	, lastTickPosZ(0)
	, ySize(0)
	, stepHeight(0)
	, noClip(0)
	, entityCollisionReduction(0)
	, field_9313_bc(0)
	, rand(NULL)
	, ticksExisted(0)
	, fireResistance(0)
	, fire(0)
	, maxAir(0)
	, inWater(0)
	, hurtResistantTime(0)
	, air(0)
	, firstUpdate(0)
	, isImmuneToFire(0)
	, minecartType(0)
	, rotationYawDelta(0)
	, addedToChunk(0)
	, chunkCoordX(0)
	, chunkCoordY(0)
	, chunkCoordZ(0)
	, serverPosX(0)
	, serverPosY(0)
	, serverPosZ(0)
	, field_9300_bu(0)
	, field_9299_bv(0)
	, isSneakingRender(0)
{
	this->entityId = s_nextEntityId++;
	this->renderDistanceWeight = 1.0;
	this->preventEntitySpawning = false;
	this->boundingBox = AxisAlignedBB::getBoundingBox(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	this->onGround = false;
	this->isCollided = false;
	this->velocityChanged = false;
	this->field_9293_aM = true;
	this->isDead = false;
	this->yOffset = 0.0F;
	this->width = 0.6F;
	this->height = 1.8F;
	this->prevDistanceWalkedModified = 0.0F;
	this->distanceWalkedModified = 0.0F;
	this->entityWalks = true;
	this->fallDistance = 0.0F;
	this->nextStepDistance = 1;
	this->ySize = 0.0F;
	this->stepHeight = 0.0F;
	this->noClip = false;
	this->entityCollisionReduction = 0.0F;
	this->field_9313_bc = false;
	this->rand = new Random();
	this->ticksExisted = 0;
	this->fireResistance = 1;
	this->fire = 0;
	this->maxAir = 300;
	this->inWater = false;
	this->hurtResistantTime = 0;
	this->air = 300;
	this->firstUpdate = true;
	this->isImmuneToFire = false;
	this->addedToChunk = false;
	this->worldObj = var1;
	this->setPosition(0.0, 0.0, 0.0);
}

Entity::~Entity()
{
}

bool Entity::equals(JObject var1)
{
	return ((var1).is<Entity>()) ? ((Entity*)var1)->entityId == this->entityId : false;
}

int Entity::hashCode()
{
	return this->entityId;
}

void Entity::preparePlayerToSpawn()
{
	if(this->worldObj != NULL) {
	while(this->posY > 0.0) {
	this->setPosition(this->posX, this->posY, this->posZ);
	if(this->worldObj->getCollidingBoundingBoxes(this, this->boundingBox).size() == 0) {
	break;
	}
	++this->posY;
	}
	this->motionX = this->motionY = this->motionZ = 0.0;
	this->rotationPitch = 0.0F;
	}
}

void Entity::setEntityDead()
{
	this->isDead = true;
}

void Entity::setSize(float var1, float var2)
{
	this->width = var1;
	this->height = var2;
}

void Entity::setRotation(float var1, float var2)
{
	this->rotationYaw = var1;
	this->rotationPitch = var2;
}

void Entity::setPosition(double var1, double var3, double var5)
{
	this->posX = var1;
	this->posY = var3;
	this->posZ = var5;
	float var7 = this->width / 2.0F;
	float var8 = this->height;
	this->boundingBox->setBounds(var1 - (double)var7, var3 - (double)this->yOffset + (double)this->ySize, var5 - (double)var7, var1 + (double)var7, var3 - (double)this->yOffset + (double)this->ySize + (double)var8, var5 + (double)var7);
}

void Entity::setAngles(float var1, float var2)
{
	float var3 = this->rotationPitch;
	float var4 = this->rotationYaw;
	this->rotationYaw = (float)((double)this->rotationYaw + (double)var1 * 0.15);
	this->rotationPitch = (float)((double)this->rotationPitch - (double)var2 * 0.15);
	if(this->rotationPitch < -90.0F) {
	this->rotationPitch = -90.0F;
	}
	if(this->rotationPitch > 90.0F) {
	this->rotationPitch = 90.0F;
	}
	this->prevRotationPitch += this->rotationPitch - var3;
	this->prevRotationYaw += this->rotationYaw - var4;
}

void Entity::onUpdate()
{
	this->onEntityUpdate();
}

void Entity::onEntityUpdate()
{
	if(this->ridingEntity != NULL && this->ridingEntity->isDead) {
	this->ridingEntity = NULL;
	}
	++this->ticksExisted;
	this->prevDistanceWalkedModified = this->distanceWalkedModified;
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	this->prevRotationPitch = this->rotationPitch;
	this->prevRotationYaw = this->rotationYaw;
	if(this->handleWaterMovement()) {
	if(!this->inWater && !this->firstUpdate) {
	float var1 = MathHelper::sqrt_double(this->motionX * this->motionX * (double)0.2F + this->motionY * this->motionY + this->motionZ * this->motionZ * (double)0.2F) * 0.2F;
	if(var1 > 1.0F) {
	var1 = 1.0F;
	}
	this->worldObj->playSoundAtEntity(this,"random.splash",var1,1.0F + (this->rand->nextFloat() - this->rand->nextFloat()) * 0.4F);
	float var2 = (float)MathHelper::floor_double(this->boundingBox->minY);
	int var3;
	float var4;
	float var5;
	for(var3 = 0; (float)var3 < 1.0F + this->width * 20.0F; ++var3) {
	var4 = (this->rand->nextFloat() * 2.0F - 1.0F) * this->width;
	var5 = (this->rand->nextFloat() * 2.0F - 1.0F) * this->width;
	this->worldObj->spawnParticle("bubble",this->posX + (double)var4,(double)(var2 + 1.0F),this->posZ + (double)var5,this->motionX,this->motionY - (double)(this->rand->nextFloat() * 0.2F),this->motionZ);
	}
	for(var3 = 0; (float)var3 < 1.0F + this->width * 20.0F; ++var3) {
	var4 = (this->rand->nextFloat() * 2.0F - 1.0F) * this->width;
	var5 = (this->rand->nextFloat() * 2.0F - 1.0F) * this->width;
	this->worldObj->spawnParticle("splash",this->posX + (double)var4,(double)(var2 + 1.0F),this->posZ + (double)var5,this->motionX,this->motionY,this->motionZ);
	}
	}
	this->fallDistance = 0.0F;
	this->inWater = true;
	this->fire = 0;
	} else {
	this->inWater = false;
	}
	if(this->worldObj->multiplayerWorld) {
	this->fire = 0;
	} else if(this->fire > 0) {
	if(this->isImmuneToFire) {
	this->fire -= 4;
	if(this->fire < 0) {
	this->fire = 0;
	}
	} else {
	if(this->fire % 20 == 0) {
	this->canAttackEntity((Entity*)NULL, 1);
	}
	--this->fire;
	}
	}
	if(this->handleLavaMovement()) {
	this->setOnFireFromLava();
	}
	if(this->posY < -64.0) {
	this->outOfWorld();
	}
	this->firstUpdate = false;
}

void Entity::setOnFireFromLava()
{
	if(!this->isImmuneToFire) {
	this->canAttackEntity((Entity*)NULL, 4);
	this->fire = 600;
	}
}

void Entity::outOfWorld()
{
	this->setEntityDead();
}

bool Entity::isOffsetPositionInLiquid(double var1, double var3, double var5)
{
	AxisAlignedBB* var7 = this->boundingBox->getOffsetBoundingBox(var1, var3, var5);
	JList<void*> var8 = this->worldObj->getCollidingBoundingBoxes(this, var7);
	return var8.size() > 0 ? false : !this->worldObj->getIsAnyLiquid(var7);
}

void Entity::moveEntity(double var1, double var3, double var5)
{
	if(this->noClip) {
	this->boundingBox->offset(var1, var3, var5);
	this->posX = (this->boundingBox->minX + this->boundingBox->maxX) / 2.0;
	this->posY = this->boundingBox->minY + (double)this->yOffset - (double)this->ySize;
	this->posZ = (this->boundingBox->minZ + this->boundingBox->maxZ) / 2.0;
	} else {
	double var7 = this->posX;
	double var9 = this->posZ;
	double var11 = var1;
	double var13 = var3;
	double var15 = var5;
	AxisAlignedBB* var17 = this->boundingBox->copy();
	bool var18 = this->onGround && this->isSneaking();
	if(var18) {
	double var19;
	for(var19 = 0.05; var1 != 0.0 && this->worldObj->getCollidingBoundingBoxes(this, this->boundingBox->getOffsetBoundingBox(var1, -1.0, 0.0)).size() == 0; var11 = var1) {
	if(var1 < var19 && var1 >= -var19) {
	var1 = 0.0;
	} else if(var1 > 0.0) {
	var1 -= var19;
	} else {
	var1 += var19;
	}
	}
	for(; var5 != 0.0 && this->worldObj->getCollidingBoundingBoxes(this, this->boundingBox->getOffsetBoundingBox(0.0, -1.0, var5)).size() == 0; var15 = var5) {
	if(var5 < var19 && var5 >= -var19) {
	var5 = 0.0;
	} else if(var5 > 0.0) {
	var5 -= var19;
	} else {
	var5 += var19;
	}
	}
	}
	JList<void*> var35 = this->worldObj->getCollidingBoundingBoxes(this, this->boundingBox->addCoord(var1, var3, var5));
	for(int var20 = 0; var20 < var35.size(); ++var20) {
	var3 = ((AxisAlignedBB*)var35[var20])->calculateYOffset(this->boundingBox, var3);
	}
	this->boundingBox->offset(0.0, var3, 0.0);
	if(!this->field_9293_aM && var13 != var3) {
	var5 = 0.0;
	var3 = var5;
	var1 = var5;
	}
	bool var36 = this->onGround || var13 != var3 && var13 < 0.0;
	int var21;
	for(var21 = 0; var21 < var35.size(); ++var21) {
	var1 = ((AxisAlignedBB*)var35[var21])->calculateXOffset(this->boundingBox, var1);
	}
	this->boundingBox->offset(var1, 0.0, 0.0);
	if(!this->field_9293_aM && var11 != var1) {
	var5 = 0.0;
	var3 = var5;
	var1 = var5;
	}
	for(var21 = 0; var21 < var35.size(); ++var21) {
	var5 = ((AxisAlignedBB*)var35[var21])->calculateZOffset(this->boundingBox, var5);
	}
	this->boundingBox->offset(0.0, 0.0, var5);
	if(!this->field_9293_aM && var15 != var5) {
	var5 = 0.0;
	var3 = var5;
	var1 = var5;
	}
	double var23;
	int var28;
	double var37;
	if(this->stepHeight > 0.0F && var36 && this->ySize < 0.05F && (var11 != var1 || var15 != var5)) {
	var37 = var1;
	var23 = var3;
	double var25 = var5;
	var1 = var11;
	var3 = (double)this->stepHeight;
	var5 = var15;
	AxisAlignedBB* var27 = this->boundingBox->copy();
	this->boundingBox->setBB(var17);
	var35 = this->worldObj->getCollidingBoundingBoxes(this, this->boundingBox->addCoord(var11, var3, var15));
	for(var28 = 0; var28 < var35.size(); ++var28) {
	var3 = ((AxisAlignedBB*)var35[var28])->calculateYOffset(this->boundingBox, var3);
	}
	this->boundingBox->offset(0.0, var3, 0.0);
	if(!this->field_9293_aM && var13 != var3) {
	var5 = 0.0;
	var3 = var5;
	var1 = var5;
	}
	for(var28 = 0; var28 < var35.size(); ++var28) {
	var1 = ((AxisAlignedBB*)var35[var28])->calculateXOffset(this->boundingBox, var1);
	}
	this->boundingBox->offset(var1, 0.0, 0.0);
	if(!this->field_9293_aM && var11 != var1) {
	var5 = 0.0;
	var3 = var5;
	var1 = var5;
	}
	for(var28 = 0; var28 < var35.size(); ++var28) {
	var5 = ((AxisAlignedBB*)var35[var28])->calculateZOffset(this->boundingBox, var5);
	}
	this->boundingBox->offset(0.0, 0.0, var5);
	if(!this->field_9293_aM && var15 != var5) {
	var5 = 0.0;
	var3 = var5;
	var1 = var5;
	}
	if(var37 * var37 + var25 * var25 >= var1 * var1 + var5 * var5) {
	var1 = var37;
	var3 = var23;
	var5 = var25;
	this->boundingBox->setBB(var27);
	} else {
	this->ySize = (float)((double)this->ySize + 0.5);
	}
	}
	this->posX = (this->boundingBox->minX + this->boundingBox->maxX) / 2.0;
	this->posY = this->boundingBox->minY + (double)this->yOffset - (double)this->ySize;
	this->posZ = (this->boundingBox->minZ + this->boundingBox->maxZ) / 2.0;
	this->isCollidedHorizontally = var11 != var1 || var15 != var5;
	this->isCollidedVertically = var13 != var3;
	this->onGround = var13 != var3 && var13 < 0.0;
	this->isCollided = this->isCollidedHorizontally || this->isCollidedVertically;
	this->updateFallState(var3, this->onGround);
	if(var11 != var1) {
	this->motionX = 0.0;
	}
	if(var13 != var3) {
	this->motionY = 0.0;
	}
	if(var15 != var5) {
	this->motionZ = 0.0;
	}
	var37 = this->posX - var7;
	var23 = this->posZ - var9;
	int var26;
	int var38;
	int var40;
	if(this->entityWalks && !var18) {
	this->distanceWalkedModified = (float)((double)this->distanceWalkedModified + (double)MathHelper::sqrt_double(var37 * var37 + var23 * var23) * 0.6);
	var38 = MathHelper::floor_double(this->posX);
	var26 = MathHelper::floor_double(this->posY - (double)0.2F - (double)this->yOffset);
	var40 = MathHelper::floor_double(this->posZ);
	var28 = this->worldObj->getBlockId(var38, var26, var40);
	if(this->distanceWalkedModified > (float)this->nextStepDistance && var28 > 0) {
	++this->nextStepDistance;
	StepSound* var29 = Block::s_blocksList[var28]->stepSound;
	if(this->worldObj->getBlockId(var38, var26 + 1, var40) == Block::s_snow->blockID) {
	var29 = Block::s_snow->stepSound;
	this->worldObj->playSoundAtEntity(this, var29->getStepSound(), var29->getVolume() * 0.15F, var29->getPitch());
	} else if(!Block::s_blocksList[var28]->blockMaterial->getIsLiquid()) {
	this->worldObj->playSoundAtEntity(this, var29->getStepSound(), var29->getVolume() * 0.15F, var29->getPitch());
	}
	if (Block::s_blocksList[var28] != NULL) { Block::s_blocksList[var28]->onEntityWalking(this->worldObj, var38, var26, var40, this); }
	}
	}
	var38 = MathHelper::floor_double(this->boundingBox->minX);
	var26 = MathHelper::floor_double(this->boundingBox->minY);
	var40 = MathHelper::floor_double(this->boundingBox->minZ);
	var28 = MathHelper::floor_double(this->boundingBox->maxX);
	int var41 = MathHelper::floor_double(this->boundingBox->maxY);
	int var30 = MathHelper::floor_double(this->boundingBox->maxZ);
	for(int var31 = var38; var31 <= var28; ++var31) {
	for(int var32 = var26; var32 <= var41; ++var32) {
	for(int var33 = var40; var33 <= var30; ++var33) {
	int var34 = this->worldObj->getBlockId(var31, var32, var33);
	if(var34 > 0) {
	if (Block::s_blocksList[var34] != NULL) { Block::s_blocksList[var34]->onEntityCollidedWithBlock(this->worldObj, var31, var32, var33, this); }
	}
	}
	}
	}
	this->ySize *= 0.4F;
	bool var39 = this->handleWaterMovement();
	if(this->worldObj->isBoundingBoxBurning(this->boundingBox)) {
	this->dealFireDamage(1);
	if(!var39) {
	++this->fire;
	if(this->fire == 0) {
	this->fire = 300;
	}
	}
	} else if(this->fire <= 0) {
	this->fire = -this->fireResistance;
	}
	if(var39 && this->fire > 0) {
	this->worldObj->playSoundAtEntity(this,"random.fizz",0.7F,1.6F + (this->rand->nextFloat() - this->rand->nextFloat()) * 0.4F);
	this->fire = -this->fireResistance;
	}
	}
}

void Entity::updateFallState(double var1, bool var3)
{
	if(var3) {
	if(this->fallDistance > 0.0F) {
	this->fall(this->fallDistance);
	this->fallDistance = 0.0F;
	}
	} else if(var1 < 0.0) {
	this->fallDistance = (float)((double)this->fallDistance - var1);
	}
}

bool Entity::isSneaking()
{
	return false;
}

AxisAlignedBB* Entity::getCollisionBox()
{
	return NULL;
}

void Entity::dealFireDamage(int var1)
{
	if(!this->isImmuneToFire) {
	this->canAttackEntity((Entity*)NULL, var1);
	}
}

void Entity::fall(float var1)
{
}

bool Entity::handleWaterMovement()
{
	return this->worldObj->handleMaterialAcceleration(this->boundingBox->expands(0.0, (double)-0.4F, 0.0), Material::s_water, this);
}

bool Entity::isInsideOfMaterial(Material* var1)
{
	double var2 = this->posY + (double)this->getEyeHeight();
	int var4 = MathHelper::floor_double(this->posX);
	int var5 = MathHelper::floor_float((float)MathHelper::floor_double(var2));
	int var6 = MathHelper::floor_double(this->posZ);
	int var7 = this->worldObj->getBlockId(var4, var5, var6);
	if(var7 != 0 && Block::s_blocksList[var7]->blockMaterial == var1) {
	float var8 = BlockFluids::getFluidHeightPercent(this->worldObj->getBlockMetadata(var4, var5, var6)) - 1.0F / 9.0F;
	float var9 = (float)(var5 + 1) - var8;
	return var2 < (double)var9;
	} else {
	return false;
	}
}

float Entity::getEyeHeight()
{
	return 0.0F;
}

bool Entity::handleLavaMovement()
{
	return this->worldObj->func_689_a(this->boundingBox->expands(0.0, (double)-0.4F, 0.0), Material::s_lava);
}

void Entity::moveFlying(float var1, float var2, float var3)
{
	float var4 = MathHelper::sqrt_float(var1 * var1 + var2 * var2);
	if(var4 >= 0.01F) {
	if(var4 < 1.0F) {
	var4 = 1.0F;
	}
	var4 = var3 / var4;
	var1 *= var4;
	var2 *= var4;
	float var5 = MathHelper::sin(this->rotationYaw * (float)3.1415927F / 180.0F);
	float var6 = MathHelper::cos(this->rotationYaw * (float)3.1415927F / 180.0F);
	this->motionX += (double)(var1 * var6 - var2 * var5);
	this->motionZ += (double)(var2 * var6 + var1 * var5);
	}
}

float Entity::getEntityBrightness(float var1)
{
	int var2 = MathHelper::floor_double(this->posX);
	double var3 = (this->boundingBox->maxY - this->boundingBox->minY) * 0.66;
	int var5 = MathHelper::floor_double(this->posY - (double)this->yOffset + var3);
	int var6 = MathHelper::floor_double(this->posZ);
	return this->worldObj->getLightBrightness(var2, var5, var6);
}

void Entity::setWorld(World* var1)
{
	this->worldObj = var1;
}

void Entity::setPositionAndRotation(double var1, double var3, double var5, float var7, float var8)
{
	this->prevPosX = this->posX = var1;
	this->prevPosY = this->posY = var3;
	this->prevPosZ = this->posZ = var5;
	this->prevRotationYaw = this->rotationYaw = var7;
	this->prevRotationPitch = this->rotationPitch = var8;
	this->ySize = 0.0F;
	double var9 = (double)(this->prevRotationYaw - var7);
	if(var9 < -180.0) {
	this->prevRotationYaw += 360.0F;
	}
	if(var9 >= 180.0) {
	this->prevRotationYaw -= 360.0F;
	}
	this->setPosition(this->posX, this->posY, this->posZ);
	this->setRotation(var7, var8);
}

void Entity::setLocationAndAngles(double var1, double var3, double var5, float var7, float var8)
{
	this->prevPosX = this->posX = var1;
	this->prevPosY = this->posY = var3 + (double)this->yOffset;
	this->prevPosZ = this->posZ = var5;
	this->rotationYaw = var7;
	this->rotationPitch = var8;
	this->setPosition(this->posX, this->posY, this->posZ);
}

float Entity::getDistanceToEntity(Entity* var1)
{
	float var2 = (float)(this->posX - var1->posX);
	float var3 = (float)(this->posY - var1->posY);
	float var4 = (float)(this->posZ - var1->posZ);
	return MathHelper::sqrt_float(var2 * var2 + var3 * var3 + var4 * var4);
}

double Entity::getDistanceSq(double var1, double var3, double var5)
{
	double var7 = this->posX - var1;
	double var9 = this->posY - var3;
	double var11 = this->posZ - var5;
	return var7 * var7 + var9 * var9 + var11 * var11;
}

double Entity::getDistance(double var1, double var3, double var5)
{
	double var7 = this->posX - var1;
	double var9 = this->posY - var3;
	double var11 = this->posZ - var5;
	return (double)MathHelper::sqrt_double(var7 * var7 + var9 * var9 + var11 * var11);
}

double Entity::getDistanceSqToEntity(Entity* var1)
{
	double var2 = this->posX - var1->posX;
	double var4 = this->posY - var1->posY;
	double var6 = this->posZ - var1->posZ;
	return var2 * var2 + var4 * var4 + var6 * var6;
}

void Entity::onCollideWithPlayer(EntityPlayer* var1)
{
}

void Entity::applyEntityCollision(Entity* var1)
{
	if(var1->riddenByEntity != this && var1->ridingEntity != this) {
	double var2 = var1->posX - this->posX;
	double var4 = var1->posZ - this->posZ;
	double var6 = MathHelper::abs_max(var2, var4);
	if(var6 >= (double)0.01F) {
	var6 = (double)MathHelper::sqrt_double(var6);
	var2 /= var6;
	var4 /= var6;
	double var8 = 1.0 / var6;
	if(var8 > 1.0) {
	var8 = 1.0;
	}
	var2 *= var8;
	var4 *= var8;
	var2 *= (double)0.05F;
	var4 *= (double)0.05F;
	var2 *= (double)(1.0F - this->entityCollisionReduction);
	var4 *= (double)(1.0F - this->entityCollisionReduction);
	this->addVelocity(-var2, 0.0, -var4);
	var1->addVelocity(var2, 0.0, var4);
	}
	}
}

void Entity::addVelocity(double var1, double var3, double var5)
{
	this->motionX += var1;
	this->motionY += var3;
	this->motionZ += var5;
}

void Entity::setBeenAttacked()
{
	this->velocityChanged = true;
}

bool Entity::canAttackEntity(Entity* var1, int var2)
{
	this->setBeenAttacked();
	return false;
}

bool Entity::canBeCollidedWith()
{
	return false;
}

bool Entity::canBePushed()
{
	return false;
}

void Entity::addToPlayerScore(Entity* var1, int var2)
{
}

bool Entity::isInRangeToRender3d(Vec3D* var1)
{
	double var2 = this->posX - var1->xCoord;
	double var4 = this->posY - var1->yCoord;
	double var6 = this->posZ - var1->zCoord;
	double var8 = var2 * var2 + var4 * var4 + var6 * var6;
	return this->isInRangeToRenderDist(var8);
}

bool Entity::isInRangeToRenderDist(double var1)
{
	double var3 = this->boundingBox->getAverageEdgeLength();
	var3 *= 64.0 * this->renderDistanceWeight;
	return var1 < var3 * var3;
}

JavaString Entity::getEntityTexture()
{
	return std::string();
}

bool Entity::addEntityID(NBTTagCompound* var1)
{
	JavaString var2 = this->getEntityString();
	if(!this->isDead && var2 != NULL) {
	var1->setString("id", var2);
	this->writeToNBT(var1);
	return true;
	} else {
	return false;
	}
}

void Entity::writeToNBT(NBTTagCompound* var1)
{
	var1->setTag("Pos", this->dropChunk((JArrayInit<double >(), this->posX, this->posY, this->posZ)));
	var1->setTag("Motion", this->dropChunk((JArrayInit<double >(), this->motionX, this->motionY, this->motionZ)));
	var1->setTag("Rotation", this->newFloatNBTList((JArrayInit<float >(), this->rotationYaw, this->rotationPitch)));
	var1->setFloat("FallDistance", this->fallDistance);
	var1->setShort("Fire", (short)this->fire);
	var1->setShort("Air", (short)this->air);
	var1->setBoolean("OnGround", this->onGround);
	this->writeEntityToNBT(var1);
}

void Entity::readFromNBT(NBTTagCompound* var1)
{
	NBTTagList* var2 = var1->getTagList("Pos");
	NBTTagList* var3 = var1->getTagList("Motion");
	NBTTagList* var4 = var1->getTagList("Rotation");
	this->setPosition(0.0, 0.0, 0.0);
	this->motionX = ((NBTTagDouble*)var3->tagAt(0))->doubleValue;
	this->motionY = ((NBTTagDouble*)var3->tagAt(1))->doubleValue;
	this->motionZ = ((NBTTagDouble*)var3->tagAt(2))->doubleValue;
	this->prevPosX = this->lastTickPosX = this->posX = ((NBTTagDouble*)var2->tagAt(0))->doubleValue;
	this->prevPosY = this->lastTickPosY = this->posY = ((NBTTagDouble*)var2->tagAt(1))->doubleValue;
	this->prevPosZ = this->lastTickPosZ = this->posZ = ((NBTTagDouble*)var2->tagAt(2))->doubleValue;
	this->prevRotationYaw = this->rotationYaw = ((NBTTagFloat*)var4->tagAt(0))->floatValue;
	this->prevRotationPitch = this->rotationPitch = ((NBTTagFloat*)var4->tagAt(1))->floatValue;
	this->fallDistance = var1->getFloat("FallDistance");
	this->fire = var1->getShort("Fire");
	this->air = var1->getShort("Air");
	this->onGround = var1->getBoolean("OnGround");
	this->setPosition(this->posX, this->posY, this->posZ);
	this->readEntityFromNBT(var1);
}

JavaString Entity::getEntityString()
{
	return EntityList::getEntityString(this);
}

NBTTagList* Entity::dropChunk(JArray<double > var1)
{
	NBTTagList* var2 = new NBTTagList();
	JArray<double > var3 = var1;
	int var4 = var1.length;
	for(int var5 = 0; var5 < var4; ++var5) {
	double var6 = var3[var5];
	var2->setTag(new NBTTagDouble(var6));
	}
	return var2;
}

NBTTagList* Entity::newFloatNBTList(JArray<float > var1)
{
	NBTTagList* var2 = new NBTTagList();
	JArray<float > var3 = var1;
	int var4 = var1.length;
	for(int var5 = 0; var5 < var4; ++var5) {
	float var6 = var3[var5];
	var2->setTag(new NBTTagFloat(var6));
	}
	return var2;
}

float Entity::getYOffsetForRender()
{
	return this->height / 2.0F;
}

EntityItem* Entity::dropItem(int var1, int var2)
{
	return this->dropItemWithOffset(var1, var2, 0.0F);
}

EntityItem* Entity::dropItemWithOffset(int var1, int var2, float var3)
{
	EntityItem* var4 = new EntityItem(this->worldObj, this->posX, this->posY + (double)var3, this->posZ, new ItemStack(var1, var2));
	var4->delayBeforeCanPickup = 10;
	this->worldObj->entityJoinedWorld(var4);
	return var4;
}

bool Entity::isEntityAlive()
{
	return !this->isDead;
}

bool Entity::isEntityInsideOpaqueBlock()
{
	int var1 = MathHelper::floor_double(this->posX);
	int var2 = MathHelper::floor_double(this->posY + (double)this->getEyeHeight());
	int var3 = MathHelper::floor_double(this->posZ);
	return this->worldObj->isBlockOpaqueCube(var1, var2, var3);
}

bool Entity::interact(EntityPlayer* var1)
{
	return false;
}

AxisAlignedBB* Entity::getBoundingBox(Entity* var1)
{
	return NULL;
}

void Entity::updateRidden()
{
	if(this->ridingEntity->isDead) {
	this->ridingEntity = NULL;
	} else {
	this->motionX = 0.0;
	this->motionY = 0.0;
	this->motionZ = 0.0;
	this->onUpdate();
	this->ridingEntity->updateRiderPosition();
	this->rotationYawDelta += (double)(this->ridingEntity->rotationYaw - this->ridingEntity->prevRotationYaw);
	for(this->minecartType += (double)(this->ridingEntity->rotationPitch - this->ridingEntity->prevRotationPitch); this->rotationYawDelta >= 180.0; this->rotationYawDelta -= 360.0) {
	}
	while(this->rotationYawDelta < -180.0) {
	this->rotationYawDelta += 360.0;
	}
	while(this->minecartType >= 180.0) {
	this->minecartType -= 360.0;
	}
	while(this->minecartType < -180.0) {
	this->minecartType += 360.0;
	}
	double var1 = this->rotationYawDelta * 0.5;
	double var3 = this->minecartType * 0.5;
	float var5 = 10.0F;
	if(var1 > (double)var5) {
	var1 = (double)var5;
	}
	if(var1 < (double)(-var5)) {
	var1 = (double)(-var5);
	}
	if(var3 > (double)var5) {
	var3 = (double)var5;
	}
	if(var3 < (double)(-var5)) {
	var3 = (double)(-var5);
	}
	this->rotationYawDelta -= var1;
	this->minecartType -= var3;
	this->rotationYaw = (float)((double)this->rotationYaw + var1);
	this->rotationPitch = (float)((double)this->rotationPitch + var3);
	}
}

void Entity::updateRiderPosition()
{
	this->riddenByEntity->setPosition(this->posX, this->posY + this->getMountedYOffset() + this->riddenByEntity->getYOffset(), this->posZ);
}

double Entity::getYOffset()
{
	return (double)this->yOffset;
}

double Entity::getMountedYOffset()
{
	return (double)this->height * 0.75;
}

void Entity::mountEntity(Entity* var1)
{
	this->minecartType = 0.0;
	this->rotationYawDelta = 0.0;
	if(var1 == NULL) {
	if(this->ridingEntity != NULL) {
	this->setLocationAndAngles(this->ridingEntity->posX, this->ridingEntity->boundingBox->minY + (double)this->ridingEntity->height, this->ridingEntity->posZ, this->rotationYaw, this->rotationPitch);
	this->ridingEntity->riddenByEntity = NULL;
	}
	this->ridingEntity = NULL;
	} else if(this->ridingEntity == var1) {
	this->ridingEntity->riddenByEntity = NULL;
	this->ridingEntity = NULL;
	this->setLocationAndAngles(var1->posX, var1->boundingBox->minY + (double)var1->height, var1->posZ, this->rotationYaw, this->rotationPitch);
	} else {
	if(this->ridingEntity != NULL) {
	this->ridingEntity->riddenByEntity = NULL;
	}
	if(var1->riddenByEntity != NULL) {
	var1->riddenByEntity->ridingEntity = NULL;
	}
	this->ridingEntity = var1;
	var1->riddenByEntity = this;
	}
}

void Entity::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9)
{
	this->setPosition(var1, var3, var5);
	this->setRotation(var7, var8);
}

float Entity::getShadowSize()
{
	return 0.1F;
}

Vec3D* Entity::getLookVec()
{
	return NULL;
}

void Entity::setInPortal()
{
}

void Entity::setVelocity(double var1, double var3, double var5)
{
	this->motionX = var1;
	this->motionY = var3;
	this->motionZ = var5;
}

void Entity::handleHealthUpdate(unsigned char var1)
{
}

void Entity::performHurtAnimation()
{
}

void Entity::initStatics()
{
}
