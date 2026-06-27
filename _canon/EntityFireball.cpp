#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityFireball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFireball.h"
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

EntityFireball::EntityFireball(World* var1)
	: Entity(var1)
	, xTile(0)
	, yTile(0)
	, zTile(0)
	, inTile(0)
	, inGround(0)
	, arrowShake(0)
	, shootingEntity(NULL)
	, ticksAlive(0)
	, ticksInAir(0)
	, accelerationX(0)
	, accelerationY(0)
	, accelerationZ(0)
{
	this->xTile = -1;
	this->yTile = -1;
	this->zTile = -1;
	this->inTile = 0;
	this->inGround = false;
	this->arrowShake = 0;
	this->ticksInAir = 0;
	this->setSize(1.0F, 1.0F);
}

EntityFireball::EntityFireball(World* var1, EntityLiving* var2, double var3, double var5, double var7)
	: Entity(var1)
	, xTile(0)
	, yTile(0)
	, zTile(0)
	, inTile(0)
	, inGround(0)
	, arrowShake(0)
	, shootingEntity(NULL)
	, ticksAlive(0)
	, ticksInAir(0)
	, accelerationX(0)
	, accelerationY(0)
	, accelerationZ(0)
{
	this->xTile = -1;
	this->yTile = -1;
	this->zTile = -1;
	this->inTile = 0;
	this->inGround = false;
	this->arrowShake = 0;
	this->ticksInAir = 0;
	this->shootingEntity = var2;
	this->setSize(1.0F, 1.0F);
	this->setLocationAndAngles(var2->posX, var2->posY, var2->posZ, var2->rotationYaw, var2->rotationPitch);
	this->setPosition(this->posX, this->posY, this->posZ);
	this->yOffset = 0.0F;
	this->motionX = this->motionY = this->motionZ = 0.0;
	var3 += this->rand->nextGaussian() * 0.4;
	var5 += this->rand->nextGaussian() * 0.4;
	var7 += this->rand->nextGaussian() * 0.4;
	double var9 = (double)MathHelper::sqrt_double(var3 * var3 + var5 * var5 + var7 * var7);
	this->accelerationX = var3 / var9 * 0.1;
	this->accelerationY = var5 / var9 * 0.1;
	this->accelerationZ = var7 / var9 * 0.1;
}

EntityFireball::~EntityFireball()
{
}

bool EntityFireball::isInRangeToRenderDist(double var1)
{
	double var3 = this->boundingBox->getAverageEdgeLength() * 4.0;
	var3 *= 64.0;
	return var1 < var3 * var3;
}

void EntityFireball::onUpdate()
{
	Entity::onUpdate();
	this->fire = 10;
	if(this->arrowShake > 0) {
	--this->arrowShake;
	}
	if(this->inGround) {
	int var1 = this->worldObj->getBlockId(this->xTile, this->yTile, this->zTile);
	if(var1 == this->inTile) {
	++this->ticksAlive;
	if(this->ticksAlive == 1200) {
	this->setEntityDead();
	}
	return;
	}
	this->inGround = false;
	this->motionX *= (double)(this->rand->nextFloat() * 0.2F);
	this->motionY *= (double)(this->rand->nextFloat() * 0.2F);
	this->motionZ *= (double)(this->rand->nextFloat() * 0.2F);
	this->ticksAlive = 0;
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
	Entity* var4 = NULL;
	JList<void*> var5 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this, this->boundingBox->addCoord(this->motionX, this->motionY, this->motionZ)->expands(1.0, 1.0, 1.0));
	double var6 = 0.0;
	for(int var8 = 0; var8 < var5.size(); ++var8) {
	Entity* var9 = (Entity*)var5[var8];
	if(var9->canBeCollidedWith() && (var9 != this->shootingEntity || this->ticksInAir >= 25)) {
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
	if(var3 != NULL) {
	if(var3->entityHit != NULL && var3->entityHit->canAttackEntity(this->shootingEntity, 0)) {
	}
	this->worldObj->newExplosion((Entity*)NULL, this->posX, this->posY, this->posZ, 1.0F, true);
	this->setEntityDead();
	}
	this->posX += this->motionX;
	this->posY += this->motionY;
	this->posZ += this->motionZ;
	float var16 = MathHelper::sqrt_double(this->motionX * this->motionX + this->motionZ * this->motionZ);
	this->rotationYaw = (float)(atan2f(this->motionX, this->motionZ) * 180.0 / (double)((float)3.1415927F));
	for(this->rotationPitch = (float)(atan2f(this->motionY, (double)var16) * 180.0 / (double)((float)3.1415927F)); this->rotationPitch - this->prevRotationPitch < -180.0F; this->prevRotationPitch -= 360.0F) {
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
	float var17 = 0.95F;
	if(this->handleWaterMovement()) {
	for(int var18 = 0; var18 < 4; ++var18) {
	float var19 = 0.25F;
	this->worldObj->spawnParticle("bubble", this->posX - this->motionX * (double)var19, this->posY - this->motionY * (double)var19, this->posZ - this->motionZ * (double)var19, this->motionX, this->motionY, this->motionZ);
	}
	var17 = 0.8F;
	}
	this->motionX += this->accelerationX;
	this->motionY += this->accelerationY;
	this->motionZ += this->accelerationZ;
	this->motionX *= (double)var17;
	this->motionY *= (double)var17;
	this->motionZ *= (double)var17;
	this->worldObj->spawnParticle("smoke",this->posX,this->posY + 0.5,this->posZ,0.0,0.0,0.0);
	this->setPosition(this->posX, this->posY, this->posZ);
}

void EntityFireball::writeEntityToNBT(NBTTagCompound* var1)
{
	var1->setShort("xTile", (short)this->xTile);
	var1->setShort("yTile", (short)this->yTile);
	var1->setShort("zTile", (short)this->zTile);
	var1->setByte("inTile", (unsigned char)this->inTile);
	var1->setByte("shake", (unsigned char)this->arrowShake);
	var1->setByte("inGround", (unsigned char)(this->inGround ? 1 : 0));
}

void EntityFireball::readEntityFromNBT(NBTTagCompound* var1)
{
	this->xTile = var1->getShort("xTile");
	this->yTile = var1->getShort("yTile");
	this->zTile = var1->getShort("zTile");
	this->inTile = var1->getByte("inTile") & 255;
	this->arrowShake = var1->getByte("shake") & 255;
	this->inGround = var1->getByte("inGround") == 1;
}

bool EntityFireball::canBeCollidedWith()
{
	return true;
}

float EntityFireball::getShadowSize()
{
	return 1.0F;
}

bool EntityFireball::canAttackEntity(Entity* var1, int var2)
{
	this->setBeenAttacked();
	if(var1 != NULL) {
	Vec3D* var3 = var1->getLookVec();
	if(var3 != NULL) {
	this->motionX = var3->xCoord;
	this->motionY = var3->yCoord;
	this->motionZ = var3->zCoord;
	this->accelerationX = this->motionX * 0.1;
	this->accelerationY = this->motionY * 0.1;
	this->accelerationZ = this->motionZ * 0.1;
	}
	return true;
	} else {
	return false;
	}
}

float EntityFireball::getYOffsetForRender()
{
	return 0.0F;
}

void EntityFireball::initStatics()
{
}
