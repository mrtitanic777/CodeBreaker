#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLiving.h"
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

EntityLiving::EntityLiving(World* var1)
	: Entity(var1)
	, maxHurtResistantTime(0)
	, randomEntitySeed(0)
	, field_9364_q(0)
	, field_9363_r(0)
	, renderYawOffset(0)
	, prevRenderYawOffset(0)
	, prevOnGroundSpeedFactor(0)
	, onGroundSpeedFactor(0)
	, legYaw(0)
	, prevLegYaw(0)
	, field_9358_y(0)
	, field_9355_A(0)
	, renderYawOffsetBase(0)
	, field_9349_D(0)
	, scoreValue(0)
	, field_9345_F(0)
	, noAi(0)
	, prevSwingProgress(0)
	, swingProgress(0)
	, health(0)
	, prevHealth(0)
	, a(0)
	, hurtTime(0)
	, maxHurtTime(0)
	, attackedAtYaw(0)
	, deathTime(0)
	, attackTime(0)
	, field_9329_Q(0)
	, field_9328_R(0)
	, dead(0)
	, field_9326_T(0)
	, field_9325_U(0)
	, prevLimbSwingAmount(0)
	, limbSwingAmount(0)
	, limbSwing(0)
	, newPosRotationIncrements(0)
	, newPosX(0)
	, newPosY(0)
	, newPosZ(0)
	, newRotationYaw(0)
	, newRotationPitch(0)
	, field_9348_ae(0)
	, lastDamage(0)
	, entityAge(0)
	, moveStrafing(0)
	, moveForward(0)
	, randomYawVelocity(0)
	, isJumping(0)
	, defaultPitch(0)
	, moveSpeed(0)
	, b(NULL)
	, c(0)
{
	this->maxHurtResistantTime = 20;
	this->renderYawOffset = 0.0F;
	this->prevRenderYawOffset = 0.0F;
	this->field_9358_y = true;
	this->texture = "/mob/char.png";
	this->field_9355_A = true;
	this->renderYawOffsetBase = 0.0F;
	this->entityType = NULL;
	this->field_9349_D = 1.0F;
	this->scoreValue = 0;
	this->field_9345_F = 0.0F;
	this->noAi = false;
	this->health = 10;
	this->attackedAtYaw = 0.0F;
	this->deathTime = 0;
	this->attackTime = 0;
	this->dead = false;
	this->field_9326_T = -1;
	this->field_9325_U = (float)(javaMathRandom() * (double)0.9F + (double)0.1F);
	this->field_9348_ae = 0.0F;
	this->lastDamage = 0;
	this->entityAge = 0;
	this->isJumping = false;
	this->defaultPitch = 0.0F;
	this->moveSpeed = 0.7F;
	this->c = 0;
	this->preventEntitySpawning = true;
	this->field_9363_r = (float)(javaMathRandom() + 1.0) * 0.01F;
	this->setPosition(this->posX, this->posY, this->posZ);
	this->randomEntitySeed = (float)javaMathRandom() * 12398.0F;
	this->rotationYaw = (float)(javaMathRandom() * (double)((float)3.1415927F) * 2.0);
	this->field_9364_q = 1.0F;
	this->stepHeight = 0.5F;
}

EntityLiving::~EntityLiving()
{
}

bool EntityLiving::canEntityBeSeen(Entity* var1)
{
	return this->worldObj->rayTraceBlocks(Vec3D::createVector(this->posX, this->posY + (double)this->getEyeHeight(), this->posZ), Vec3D::createVector(var1->posX, var1->posY + (double)var1->getEyeHeight(), var1->posZ)) == NULL;
}

JavaString EntityLiving::getEntityTexture()
{
	return this->texture;
}

bool EntityLiving::canBeCollidedWith()
{
	return !this->isDead;
}

bool EntityLiving::canBePushed()
{
	return !this->isDead;
}

float EntityLiving::getEyeHeight()
{
	return this->height * 0.85F;
}

int EntityLiving::getTalkInterval()
{
	return 80;
}

void EntityLiving::onEntityUpdate()
{
	this->prevSwingProgress = this->swingProgress;
	Entity::onEntityUpdate();
	if(this->rand->nextInt(1000) < this->a++) {
	this->a = -this->getTalkInterval();
	JavaString var1 = this->getLivingSound();
	if(var1 != NULL) {
	this->worldObj->playSoundAtEntity(this, var1, this->getSoundVolume(), (this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F);
	}
	}
	if(this->isEntityAlive() && this->isEntityInsideOpaqueBlock()) {
	this->canAttackEntity((Entity*)NULL, 1);
	}
	if(this->isImmuneToFire || this->worldObj->multiplayerWorld) {
	this->fire = 0;
	}
	int var8;
	if(this->isEntityAlive() && this->isInsideOfMaterial(Material::s_water)) {
	--this->air;
	if(this->air == -20) {
	this->air = 0;
	for(var8 = 0; var8 < 8; ++var8) {
	float var2 = this->rand->nextFloat() - this->rand->nextFloat();
	float var3 = this->rand->nextFloat() - this->rand->nextFloat();
	float var4 = this->rand->nextFloat() - this->rand->nextFloat();
	this->worldObj->spawnParticle("bubble",this->posX + (double)var2,this->posY + (double)var3,this->posZ + (double)var4,this->motionX,this->motionY,this->motionZ);
	}
	this->canAttackEntity((Entity*)NULL, 2);
	}
	this->fire = 0;
	} else {
	this->air = this->maxAir;
	}
	this->field_9329_Q = this->field_9328_R;
	if(this->attackTime > 0) {
	--this->attackTime;
	}
	if(this->hurtTime > 0) {
	--this->hurtTime;
	}
	if(this->hurtResistantTime > 0) {
	--this->hurtResistantTime;
	}
	if(this->health <= 0) {
	++this->deathTime;
	if(this->deathTime > 20) {
	this->onEntityDeath();
	this->setEntityDead();
	for(var8 = 0; var8 < 20; ++var8) {
	double var9 = this->rand->nextGaussian() * 0.02;
	double var10 = this->rand->nextGaussian() * 0.02;
	double var6 = this->rand->nextGaussian() * 0.02;
	this->worldObj->spawnParticle("explode",this->posX + (double)(this->rand->nextFloat() * this->width * 2.0F) - (double)this->width,this->posY + (double)(this->rand->nextFloat() * this->height),this->posZ + (double)(this->rand->nextFloat() * this->width * 2.0F) - (double)this->width,var9,var10,var6);
	}
	}
	}
	this->prevLegYaw = this->legYaw;
	this->prevRenderYawOffset = this->renderYawOffset;
	this->prevRotationYaw = this->rotationYaw;
	this->prevRotationPitch = this->rotationPitch;
}

void EntityLiving::spawnExplosionParticle()
{
	for(int var1 = 0; var1 < 20; ++var1) {
	double var2 = this->rand->nextGaussian() * 0.02;
	double var4 = this->rand->nextGaussian() * 0.02;
	double var6 = this->rand->nextGaussian() * 0.02;
	double var8 = 10.0;
	this->worldObj->spawnParticle("explode",this->posX + (double)(this->rand->nextFloat() * this->width * 2.0F) - (double)this->width - var2 * var8,this->posY + (double)(this->rand->nextFloat() * this->height) - var4 * var8,this->posZ + (double)(this->rand->nextFloat() * this->width * 2.0F) - (double)this->width - var6 * var8,var2,var4,var6);
	}
}

void EntityLiving::updateRidden()
{
	Entity::updateRidden();
	this->prevOnGroundSpeedFactor = this->onGroundSpeedFactor;
	this->onGroundSpeedFactor = 0.0F;
}

void EntityLiving::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9)
{
	this->yOffset = 0.0F;
	this->newPosX = var1;
	this->newPosY = var3;
	this->newPosZ = var5;
	this->newRotationYaw = (double)var7;
	this->newRotationPitch = (double)var8;
	this->newPosRotationIncrements = var9;
}

void EntityLiving::onUpdate()
{
	Entity::onUpdate();
	this->onLivingUpdate();
	double var1 = this->posX - this->prevPosX;
	double var3 = this->posZ - this->prevPosZ;
	float var5 = MathHelper::sqrt_double(var1 * var1 + var3 * var3);
	float var6 = this->renderYawOffset;
	float var7 = 0.0F;
	this->prevOnGroundSpeedFactor = this->onGroundSpeedFactor;
	float var8 = 0.0F;
	if(var5 > 0.05F) {
	var8 = 1.0F;
	var7 = var5 * 3.0F;
	var6 = (float)atan2f(var3, var1) * 180.0F / (float)3.1415927F - 90.0F;
	}
	if(this->swingProgress > 0.0F) {
	var6 = this->rotationYaw;
	}
	if(!this->onGround) {
	var8 = 0.0F;
	}
	this->onGroundSpeedFactor += (var8 - this->onGroundSpeedFactor) * 0.3F;
	float var9;
	for(var9 = var6 - this->renderYawOffset; var9 < -180.0F; var9 += 360.0F) {
	}
	while(var9 >= 180.0F) {
	var9 -= 360.0F;
	}
	this->renderYawOffset += var9 * 0.3F;
	float var10;
	for(var10 = this->rotationYaw - this->renderYawOffset; var10 < -180.0F; var10 += 360.0F) {
	}
	while(var10 >= 180.0F) {
	var10 -= 360.0F;
	}
	bool var11 = var10 < -90.0F || var10 >= 90.0F;
	if(var10 < -75.0F) {
	var10 = -75.0F;
	}
	if(var10 >= 75.0F) {
	var10 = 75.0F;
	}
	this->renderYawOffset = this->rotationYaw - var10;
	if(var10 * var10 > 2500.0F) {
	this->renderYawOffset += var10 * 0.2F;
	}
	if(var11) {
	var7 *= -1.0F;
	}
	while(this->rotationYaw - this->prevRotationYaw < -180.0F) {
	this->prevRotationYaw -= 360.0F;
	}
	while(this->rotationYaw - this->prevRotationYaw >= 180.0F) {
	this->prevRotationYaw += 360.0F;
	}
	while(this->renderYawOffset - this->prevRenderYawOffset < -180.0F) {
	this->prevRenderYawOffset -= 360.0F;
	}
	while(this->renderYawOffset - this->prevRenderYawOffset >= 180.0F) {
	this->prevRenderYawOffset += 360.0F;
	}
	while(this->rotationPitch - this->prevRotationPitch < -180.0F) {
	this->prevRotationPitch -= 360.0F;
	}
	while(this->rotationPitch - this->prevRotationPitch >= 180.0F) {
	this->prevRotationPitch += 360.0F;
	}
	this->legYaw += var7;
}

void EntityLiving::setSize(float var1, float var2)
{
	Entity::setSize(var1, var2);
}

void EntityLiving::heal(int var1)
{
	if(this->health > 0) {
	this->health += var1;
	if(this->health > 20) {
	this->health = 20;
	}
	this->hurtResistantTime = this->maxHurtResistantTime / 2;
	}
}

bool EntityLiving::canAttackEntity(Entity* var1, int var2)
{
	if(this->worldObj->multiplayerWorld) {
	return false;
	} else {
	this->entityAge = 0;
	if(this->health <= 0) {
	return false;
	} else {
	this->limbSwingAmount = 1.5F;
	bool var3 = true;
	if((float)this->hurtResistantTime > (float)this->maxHurtResistantTime / 2.0F) {
	if(var2 <= this->lastDamage) {
	return false;
	}
	this->damageEntity(var2 - this->lastDamage);
	this->lastDamage = var2;
	var3 = false;
	} else {
	this->lastDamage = var2;
	this->prevHealth = this->health;
	this->hurtResistantTime = this->maxHurtResistantTime;
	this->damageEntity(var2);
	this->hurtTime = this->maxHurtTime = 10;
	}
	this->attackedAtYaw = 0.0F;
	if(var3) {
	this->worldObj->setEntityState(this, (unsigned char)2);
	this->setBeenAttacked();
	if(var1 != NULL) {
	double var4 = var1->posX - this->posX;
	double var6;
	for(var6 = var1->posZ - this->posZ; var4 * var4 + var6 * var6 < 1.0E-4; var6 = (javaMathRandom() - javaMathRandom()) * 0.01) {
	var4 = (javaMathRandom() - javaMathRandom()) * 0.01;
	}
	this->attackedAtYaw = (float)(atan2f(var6, var4) * 180.0 / (double)((float)3.1415927F)) - this->rotationYaw;
	this->knockBack(var1, var2, var4, var6);
	} else {
	this->attackedAtYaw = (float)((int)(javaMathRandom() * 2.0) * 180);
	}
	}
	if(this->health <= 0) {
	if(var3) {
	this->worldObj->playSoundAtEntity(this, this->getDeathSound(), this->getSoundVolume(), (this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F);
	}
	this->onDeath(var1);
	} else if(var3) {
	this->worldObj->playSoundAtEntity(this, this->getHurtSound(), this->getSoundVolume(), (this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F);
	}
	return true;
	}
	}
}

void EntityLiving::performHurtAnimation()
{
	this->hurtTime = this->maxHurtTime = 10;
	this->attackedAtYaw = 0.0F;
}

void EntityLiving::damageEntity(int var1)
{
	this->health -= var1;
}

float EntityLiving::getSoundVolume()
{
	return 1.0F;
}

JavaString EntityLiving::getLivingSound()
{
	return std::string();
}

JavaString EntityLiving::getHurtSound()
{
	return "random.hurt";
}

JavaString EntityLiving::getDeathSound()
{
	return "random.hurt";
}

void EntityLiving::knockBack(Entity* var1, int var2, double var3, double var5)
{
	float var7 = MathHelper::sqrt_double(var3 * var3 + var5 * var5);
	float var8 = 0.4F;
	this->motionX /= 2.0;
	this->motionY /= 2.0;
	this->motionZ /= 2.0;
	this->motionX -= var3 / (double)var7 * (double)var8;
	this->motionY += (double)0.4F;
	this->motionZ -= var5 / (double)var7 * (double)var8;
	if(this->motionY > (double)0.4F) {
	this->motionY = (double)0.4F;
	}
}

void EntityLiving::onDeath(Entity* var1)
{
	if(this->scoreValue > 0 && var1 != NULL) {
	var1->addToPlayerScore(this, this->scoreValue);
	}
	this->dead = true;
	if(!this->worldObj->multiplayerWorld) {
	int var2 = this->getDropItemId();
	if(var2 > 0) {
	int var3 = this->rand->nextInt(3);
	for(int var4 = 0; var4 < var3; ++var4) {
	this->dropItem(var2, 1);
	}
	}
	}
	this->worldObj->setEntityState(this, (unsigned char)3);
}

int EntityLiving::getDropItemId()
{
	return 0;
}

void EntityLiving::fall(float var1)
{
	int var2 = (int)ceilf((double)(var1 - 3.0F));
	if(var2 > 0) {
	this->canAttackEntity((Entity*)NULL, var2);
	int var3 = this->worldObj->getBlockId(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->posY - (double)0.2F - (double)this->yOffset), MathHelper::floor_double(this->posZ));
	if(var3 > 0) {
	StepSound* var4 = Block::s_blocksList[var3]->stepSound;
	this->worldObj->playSoundAtEntity(this, var4->getStepSound(), var4->getVolume() * 0.5F, var4->getPitch() * (12.0F / 16.0F));
	}
	}
}

void EntityLiving::moveEntityWithHeading(float var1, float var2)
{
	double var3;
	if(this->handleWaterMovement()) {
	var3 = this->posY;
	this->moveFlying(var1, var2, 0.02F);
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.8F;
	this->motionY *= (double)0.8F;
	this->motionZ *= (double)0.8F;
	this->motionY -= 0.02;
	if(this->isCollidedHorizontally && this->isOffsetPositionInLiquid(this->motionX, this->motionY + (double)0.6F - this->posY + var3, this->motionZ)) {
	this->motionY = (double)0.3F;
	}
	} else if(this->handleLavaMovement()) {
	var3 = this->posY;
	this->moveFlying(var1, var2, 0.02F);
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= 0.5;
	this->motionY *= 0.5;
	this->motionZ *= 0.5;
	this->motionY -= 0.02;
	if(this->isCollidedHorizontally && this->isOffsetPositionInLiquid(this->motionX, this->motionY + (double)0.6F - this->posY + var3, this->motionZ)) {
	this->motionY = (double)0.3F;
	}
	} else {
	float var8 = 0.91F;
	if(this->onGround) {
	var8 = 546.0F * 0.1F * 0.1F * 0.1F;
	int var4 = this->worldObj->getBlockId(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->boundingBox->minY) - 1, MathHelper::floor_double(this->posZ));
	if(var4 > 0) {
	var8 = Block::s_blocksList[var4]->slipperiness * 0.91F;
	}
	}
	float var9 = 0.16277136F / (var8 * var8 * var8);
	this->moveFlying(var1, var2, this->onGround ? 0.1F * var9 : 0.02F);
	var8 = 0.91F;
	if(this->onGround) {
	var8 = 546.0F * 0.1F * 0.1F * 0.1F;
	int var5 = this->worldObj->getBlockId(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->boundingBox->minY) - 1, MathHelper::floor_double(this->posZ));
	if(var5 > 0) {
	var8 = Block::s_blocksList[var5]->slipperiness * 0.91F;
	}
	}
	if(this->isOnLadder()) {
	this->fallDistance = 0.0F;
	if(this->motionY < -0.15) {
	this->motionY = -0.15;
	}
	}
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	if(this->isCollidedHorizontally && this->isOnLadder()) {
	this->motionY = 0.2;
	}
	this->motionY -= 0.08;
	this->motionY *= (double)0.98F;
	this->motionX *= (double)var8;
	this->motionZ *= (double)var8;
	}
	this->prevLimbSwingAmount = this->limbSwingAmount;
	var3 = this->posX - this->prevPosX;
	double var10 = this->posZ - this->prevPosZ;
	float var7 = MathHelper::sqrt_double(var3 * var3 + var10 * var10) * 4.0F;
	if(var7 > 1.0F) {
	var7 = 1.0F;
	}
	this->limbSwingAmount += (var7 - this->limbSwingAmount) * 0.4F;
	this->limbSwing += this->limbSwingAmount;
}

bool EntityLiving::isOnLadder()
{
	int var1 = MathHelper::floor_double(this->posX);
	int var2 = MathHelper::floor_double(this->boundingBox->minY);
	int var3 = MathHelper::floor_double(this->posZ);
	return this->worldObj->getBlockId(var1, var2, var3) == Block::s_ladder->blockID || this->worldObj->getBlockId(var1, var2 + 1, var3) == Block::s_ladder->blockID;
}

void EntityLiving::writeEntityToNBT(NBTTagCompound* var1)
{
	var1->setShort("Health", (short)this->health);
	var1->setShort("HurtTime", (short)this->hurtTime);
	var1->setShort("DeathTime", (short)this->deathTime);
	var1->setShort("AttackTime", (short)this->attackTime);
}

void EntityLiving::readEntityFromNBT(NBTTagCompound* var1)
{
	this->health = var1->getShort("Health");
	if(!var1->hasKey("Health")) {
	this->health = 10;
	}
	this->hurtTime = var1->getShort("HurtTime");
	this->deathTime = var1->getShort("DeathTime");
	this->attackTime = var1->getShort("AttackTime");
}

bool EntityLiving::isEntityAlive()
{
	return !this->isDead && this->health > 0;
}

void EntityLiving::onLivingUpdate()
{
	if(this->newPosRotationIncrements > 0) {
	double var1 = this->posX + (this->newPosX - this->posX) / (double)this->newPosRotationIncrements;
	double var3 = this->posY + (this->newPosY - this->posY) / (double)this->newPosRotationIncrements;
	double var5 = this->posZ + (this->newPosZ - this->posZ) / (double)this->newPosRotationIncrements;
	double var7;
	for(var7 = this->newRotationYaw - (double)this->rotationYaw; var7 < -180.0; var7 += 360.0) {
	}
	while(var7 >= 180.0) {
	var7 -= 360.0;
	}
	this->rotationYaw = (float)((double)this->rotationYaw + var7 / (double)this->newPosRotationIncrements);
	this->rotationPitch = (float)((double)this->rotationPitch + (this->newRotationPitch - (double)this->rotationPitch) / (double)this->newPosRotationIncrements);
	--this->newPosRotationIncrements;
	this->setPosition(var1, var3, var5);
	this->setRotation(this->rotationYaw, this->rotationPitch);
	}
	if(this->health <= 0) {
	this->isJumping = false;
	this->moveStrafing = 0.0F;
	this->moveForward = 0.0F;
	this->randomYawVelocity = 0.0F;
	} else if(!this->noAi) {
	this->updateEntityActionState();
	}
	bool var9 = this->handleWaterMovement();
	bool var2 = this->handleLavaMovement();
	if(this->isJumping) {
	if(var9) {
	this->motionY += (double)0.04F;
	} else if(var2) {
	this->motionY += (double)0.04F;
	} else if(this->onGround) {
	this->jump();
	}
	}
	this->moveStrafing *= 0.98F;
	this->moveForward *= 0.98F;
	this->randomYawVelocity *= 0.9F;
	this->moveEntityWithHeading(this->moveStrafing, this->moveForward);
	JList<void*> var10 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this, this->boundingBox->expands((double)0.2F, 0.0, (double)0.2F));
	if(var10 != NULL && var10.size() > 0) {
	for(int var4 = 0; var4 < var10.size(); ++var4) {
	Entity* var11 = (Entity*)var10[var4];
	if(var11->canBePushed()) {
	var11->applyEntityCollision(this);
	}
	}
	}
}

void EntityLiving::jump()
{
	this->motionY = (double)0.42F;
}

void EntityLiving::updateEntityActionState()
{
	++this->entityAge;
	EntityPlayer* var1 = this->worldObj->getClosestPlayerToEntity(this, -1.0);
	if(var1 != NULL) {
	double var2 = var1->posX - this->posX;
	double var4 = var1->posY - this->posY;
	double var6 = var1->posZ - this->posZ;
	double var8 = var2 * var2 + var4 * var4 + var6 * var6;
	if(var8 > 16384.0) {
	this->setEntityDead();
	}
	if(this->entityAge > 600 && this->rand->nextInt(800) == 0) {
	if(var8 < 1024.0) {
	this->entityAge = 0;
	} else {
	this->setEntityDead();
	}
	}
	}
	this->moveStrafing = 0.0F;
	this->moveForward = 0.0F;
	float var10 = 8.0F;
	if(this->rand->nextFloat() < 0.02F) {
	var1 = this->worldObj->getClosestPlayerToEntity(this, (double)var10);
	if(var1 != NULL) {
	this->b = var1;
	this->c = 10 + this->rand->nextInt(20);
	} else {
	this->randomYawVelocity = (this->rand->nextFloat() - 0.5F) * 20.0F;
	}
	}
	if(this->b != NULL) {
	this->faceEntity(this->b, 10.0F);
	if(this->c-- <= 0 || this->b->isDead || this->b->getDistanceSqToEntity(this) > (double)(var10 * var10)) {
	this->b = NULL;
	}
	} else {
	if(this->rand->nextFloat() < 0.05F) {
	this->randomYawVelocity = (this->rand->nextFloat() - 0.5F) * 20.0F;
	}
	this->rotationYaw += this->randomYawVelocity;
	this->rotationPitch = this->defaultPitch;
	}
	bool var3 = this->handleWaterMovement();
	bool var11 = this->handleLavaMovement();
	if(var3 || var11) {
	this->isJumping = this->rand->nextFloat() < 0.8F;
	}
}

void EntityLiving::faceEntity(Entity* var1, float var2)
{
	double var3 = var1->posX - this->posX;
	double var7 = var1->posZ - this->posZ;
	double var5;
	if((dynamic_cast<EntityLiving*>(var1) != NULL)) {
	EntityLiving* var9 = (EntityLiving*)var1;
	var5 = var9->posY + (double)var9->getEyeHeight() - (this->posY + (double)this->getEyeHeight());
	} else {
	var5 = (var1->boundingBox->minY + var1->boundingBox->maxY) / 2.0 - (this->posY + (double)this->getEyeHeight());
	}
	double var13 = (double)MathHelper::sqrt_double(var3 * var3 + var7 * var7);
	float var11 = (float)(atan2f(var7, var3) * 180.0 / (double)((float)3.1415927F)) - 90.0F;
	float var12 = (float)(atan2f(var5, var13) * 180.0 / (double)((float)3.1415927F));
	this->rotationPitch = -this->updateRotation(this->rotationPitch, var12, var2);
	this->rotationYaw = this->updateRotation(this->rotationYaw, var11, var2);
}

float EntityLiving::updateRotation(float var1, float var2, float var3)
{
	float var4;
	for(var4 = var2 - var1; var4 < -180.0F; var4 += 360.0F) {
	}
	while(var4 >= 180.0F) {
	var4 -= 360.0F;
	}
	if(var4 > var3) {
	var4 = var3;
	}
	if(var4 < -var3) {
	var4 = -var3;
	}
	return var1 + var4;
}

void EntityLiving::onEntityDeath()
{
}

bool EntityLiving::getCanSpawnHere()
{
	return this->worldObj->checkIfAABBIsClear(this->boundingBox) && this->worldObj->getCollidingBoundingBoxes(this, this->boundingBox).size() == 0 && !this->worldObj->getIsAnyLiquid(this->boundingBox);
}

void EntityLiving::outOfWorld()
{
	this->canAttackEntity((Entity*)NULL, 4);
}

float EntityLiving::getSwingProgress(float var1)
{
	float var2 = this->swingProgress - this->prevSwingProgress;
	if(var2 < 0.0F) {
	++var2;
	}
	return this->prevSwingProgress + var2 * var1;
}

Vec3D* EntityLiving::getPosition(float var1)
{
	if(var1 == 1.0F) {
	return Vec3D::createVector(this->posX, this->posY, this->posZ);
	} else {
	double var2 = this->prevPosX + (this->posX - this->prevPosX) * (double)var1;
	double var4 = this->prevPosY + (this->posY - this->prevPosY) * (double)var1;
	double var6 = this->prevPosZ + (this->posZ - this->prevPosZ) * (double)var1;
	return Vec3D::createVector(var2, var4, var6);
	}
}

Vec3D* EntityLiving::getLookVec()
{
	return this->getLook(1.0F);
}

Vec3D* EntityLiving::getLook(float var1)
{
	float var2;
	float var3;
	float var4;
	float var5;
	if(var1 == 1.0F) {
	var2 = MathHelper::cos(-this->rotationYaw * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	var3 = MathHelper::sin(-this->rotationYaw * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	var4 = -MathHelper::cos(-this->rotationPitch * ((float)3.1415927F / 180.0F));
	var5 = MathHelper::sin(-this->rotationPitch * ((float)3.1415927F / 180.0F));
	return Vec3D::createVector((double)(var3 * var4), (double)var5, (double)(var2 * var4));
	} else {
	var2 = this->prevRotationPitch + (this->rotationPitch - this->prevRotationPitch) * var1;
	var3 = this->prevRotationYaw + (this->rotationYaw - this->prevRotationYaw) * var1;
	var4 = MathHelper::cos(-var3 * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	var5 = MathHelper::sin(-var3 * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	float var6 = -MathHelper::cos(-var2 * ((float)3.1415927F / 180.0F));
	float var7 = MathHelper::sin(-var2 * ((float)3.1415927F / 180.0F));
	return Vec3D::createVector((double)(var5 * var6), (double)var7, (double)(var4 * var6));
	}
}

MovingObjectPosition* EntityLiving::rayTrace(double var1, float var3)
{
	Vec3D* var4 = this->getPosition(var3);
	Vec3D* var5 = this->getLook(var3);
	Vec3D* var6 = var4->addVector(var5->xCoord * var1, var5->yCoord * var1, var5->zCoord * var1);
	return this->worldObj->rayTraceBlocks(var4, var6);
}

int EntityLiving::getMaxSpawnedInChunk()
{
	return 4;
}

ItemStack* EntityLiving::getHeldItem()
{
	return NULL;
}

void EntityLiving::handleHealthUpdate(unsigned char var1)
{
	if(var1 == 2) {
	this->limbSwingAmount = 1.5F;
	this->hurtResistantTime = this->maxHurtResistantTime;
	this->hurtTime = this->maxHurtTime = 10;
	this->attackedAtYaw = 0.0F;
	this->worldObj->playSoundAtEntity(this, this->getHurtSound(), this->getSoundVolume(), (this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F);
	this->canAttackEntity((Entity*)NULL, 0);
	} else if(var1 == 3) {
	this->worldObj->playSoundAtEntity(this, this->getDeathSound(), this->getSoundVolume(), (this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F);
	this->health = 0;
	this->onDeath((Entity*)NULL);
	} else {
	Entity::handleHealthUpdate(var1);
	}
}

void EntityLiving::initStatics()
{
}
