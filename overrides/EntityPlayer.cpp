#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPlayer.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "Dimension.h"
#include "Entity.h"
#include "EntityArrow.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityLiving.h"
#include "EntityMobs.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IInventory.h"
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
#include "TileEntityFurnace.h"
#include "TileEntitySign.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // field initializer on the EntityPlayer class:
 *  public InventoryPlayer inventory = new InventoryPlayer(this);
 *  ---- end original Java ----
 *  The Java instance-field initializer that constructs the player's inventory is
 *  dropped: inventory is left NULL (see init list), so every this->inventory->...
 *  call below (decrementAnimations, dropAllItems, getStrVsBlock, etc.) is a null deref. */
EntityPlayer::EntityPlayer(World* var1)
	: EntityLiving(var1)
	, inventory(NULL)
	, blockBreakingPower(0)
	, score(0)
	, prevCameraYaw(0)
	, cameraYaw(0)
	, isSwinging(0)
	, swingProgressInt(0)
	, dimension(0)
	, damageRemainder(0)
	, fishEntity(NULL)
{
	this->inventory = new InventoryPlayer(this);   // restored Java field initializer (was dropped -> null-deref)
	this->yOffset = 1.62F;
	this->setLocationAndAngles((double)var1->spawnX + 0.5D, (double)(var1->spawnY + 1), (double)var1->spawnZ + 0.5D, 0.0F, 0.0F);
	this->health = 20;
	this->entityType = "humanoid";
	this->renderYawOffsetBase = 180.0F;
	this->fireResistance = 20;
	this->texture = "/mob/char.png";
}

EntityPlayer::~EntityPlayer()
{
}

void EntityPlayer::updateRidden()
{
	EntityLiving::updateRidden();
	this->prevCameraYaw = this->cameraYaw;
	this->cameraYaw = 0.0F;
}

void EntityPlayer::preparePlayerToSpawn()
{
	this->yOffset = 1.62F;
	this->setSize(0.6F, 1.8F);
	EntityLiving::preparePlayerToSpawn();
	this->health = 20;
	this->deathTime = 0;
}

void EntityPlayer::updateEntityActionState()
{
	if(this->isSwinging) {
	++this->swingProgressInt;
	if(this->swingProgressInt == 8) {
	this->swingProgressInt = 0;
	this->isSwinging = false;
	}
	} else {
	this->swingProgressInt = 0;
	}
	this->swingProgress = (float)this->swingProgressInt / 8.0F;
}

void EntityPlayer::onLivingUpdate()
{
	if(this->worldObj->difficultySetting == 0 && this->health < 20 && this->ticksExisted % 20 * 4 == 0) {
	this->heal(1);
	}
	this->inventory->decrementAnimations();
	this->prevCameraYaw = this->cameraYaw;
	EntityLiving::onLivingUpdate();
	float var1 = MathHelper::sqrt_double(this->motionX * this->motionX + this->motionZ * this->motionZ);
	float var2 = (float)atanf(-this->motionY * (double)0.2F) * 15.0F;
	if(var1 > 0.1F) {
	var1 = 0.1F;
	}
	if(!this->onGround || this->health <= 0) {
	var1 = 0.0F;
	}
	if(this->onGround || this->health <= 0) {
	var2 = 0.0F;
	}
	this->cameraYaw += (var1 - this->cameraYaw) * 0.4F;
	this->field_9328_R += (var2 - this->field_9328_R) * 0.8F;
	if(this->health > 0) {
	JList<void*> var3 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this, this->boundingBox->expands(1.0D, 0.0D, 1.0D));
	if(var3 != NULL) {
	for(int var4 = 0; var4 < var3.size(); ++var4) {
	this->collideWithPlayer((Entity*)var3[var4]);
	}
	}
	}
}

void EntityPlayer::collideWithPlayer(Entity* var1)
{
	var1->onCollideWithPlayer(this);
}

int EntityPlayer::getScore()
{
	return this->score;
}

void EntityPlayer::onDeath(Entity* var1)
{
	EntityLiving::onDeath(var1);
	this->setSize(0.2F, 0.2F);
	this->setPosition(this->posX, this->posY, this->posZ);
	this->motionY = (double)0.1F;
	if(this->field_771_i == "Notch") {
	this->dropPlayerItemWithRandomChoice(new ItemStack(Item::s_appleRed, 1), true);
	}
	this->inventory->dropAllItems();
	if(var1 != NULL) {
	this->motionX = (double)(-MathHelper::cos((this->attackedAtYaw + this->rotationYaw) * (float)3.1415927F / 180.0F) * 0.1F);
	this->motionZ = (double)(-MathHelper::sin((this->attackedAtYaw + this->rotationYaw) * (float)3.1415927F / 180.0F) * 0.1F);
	} else {
	this->motionX = this->motionZ = 0.0D;
	}
	this->yOffset = 0.1F;
}

void EntityPlayer::addToPlayerScore(Entity* var1, int var2)
{
	this->score += var2;
}

void EntityPlayer::dropPlayerItem(ItemStack* var1)
{
	this->dropPlayerItemWithRandomChoice(var1, false);
}

void EntityPlayer::dropPlayerItemWithRandomChoice(ItemStack* var1, bool var2)
{
	if(var1 != NULL) {
	EntityItem* var3 = new EntityItem(this->worldObj, this->posX, this->posY - (double)0.3F + (double)this->getEyeHeight(), this->posZ, var1);
	var3->delayBeforeCanPickup = 40;
	float var4 = 0.1F;
	float var5;
	if(var2) {
	var5 = this->rand->nextFloat() * 0.5F;
	float var6 = this->rand->nextFloat() * (float)3.1415927F * 2.0F;
	var3->motionX = (double)(-MathHelper::sin(var6) * var5);
	var3->motionZ = (double)(MathHelper::cos(var6) * var5);
	var3->motionY = (double)0.2F;
	} else {
	var4 = 0.3F;
	var3->motionX = (double)(-MathHelper::sin(this->rotationYaw / 180.0F * (float)3.1415927F) * MathHelper::cos(this->rotationPitch / 180.0F * (float)3.1415927F) * var4);
	var3->motionZ = (double)(MathHelper::cos(this->rotationYaw / 180.0F * (float)3.1415927F) * MathHelper::cos(this->rotationPitch / 180.0F * (float)3.1415927F) * var4);
	var3->motionY = (double)(-MathHelper::sin(this->rotationPitch / 180.0F * (float)3.1415927F) * var4 + 0.1F);
	var4 = 0.02F;
	var5 = this->rand->nextFloat() * (float)3.1415927F * 2.0F;
	var4 *= this->rand->nextFloat();
	var3->motionX += cosf((double)var5) * (double)var4;
	var3->motionY += (double)((this->rand->nextFloat() - this->rand->nextFloat()) * 0.1F);
	var3->motionZ += sinf((double)var5) * (double)var4;
	}
	this->joinEntityItemWithWorld(var3);
	}
}

void EntityPlayer::joinEntityItemWithWorld(EntityItem* var1)
{
	this->worldObj->entityJoinedWorld(var1);
}

float EntityPlayer::getCurrentPlayerStrVsBlock(Block* var1)
{
	float var2 = this->inventory->getStrVsBlock(var1);
	if(this->isInsideOfMaterial(Material::s_water)) {
	var2 /= 5.0F;
	}
	if(!this->onGround) {
	var2 /= 5.0F;
	}
	return var2;
}

bool EntityPlayer::canHarvestBlock(Block* var1)
{
	return this->inventory->canHarvestBlock(var1);
}

void EntityPlayer::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityLiving::readEntityFromNBT(var1);
	NBTTagList* var2 = var1->getTagList("Inventory");
	this->inventory->readFromNBT(var2);
	this->dimension = var1->getInteger("Dimension");
}

void EntityPlayer::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityLiving::writeEntityToNBT(var1);
	var1->setTag("Inventory", this->inventory->writeToNBT(new NBTTagList()));
	var1->setInteger("Dimension", this->dimension);
}

void EntityPlayer::displayGUIChest(IInventory* var1)
{
}

void EntityPlayer::displayWorkbenchGUI()
{
}

void EntityPlayer::onItemPickup(Entity* var1, int var2)
{
}

float EntityPlayer::getEyeHeight()
{
	return 0.12F;
}

bool EntityPlayer::canAttackEntity(Entity* var1, int var2)
{
	this->entityAge = 0;
	if(this->health <= 0) {
	return false;
	} else {
	if((dynamic_cast<EntityMobs*>(var1) != NULL) || (dynamic_cast<EntityArrow*>(var1) != NULL)) {
	if(this->worldObj->difficultySetting == 0) {
	var2 = 0;
	}
	if(this->worldObj->difficultySetting == 1) {
	var2 = var2 / 3 + 1;
	}
	if(this->worldObj->difficultySetting == 3) {
	var2 = var2 * 3 / 2;
	}
	}
	return var2 == 0 ? false : EntityLiving::canAttackEntity(var1, var2);
	}
}

void EntityPlayer::damageEntity(int var1)
{
	int var2 = 25 - this->inventory->getTotalArmorValue();
	int var3 = var1 * var2 + this->damageRemainder;
	this->inventory->damageArmor(var1);
	var1 = var3 / 25;
	this->damageRemainder = var3 % 25;
	EntityLiving::damageEntity(var1);
}

void EntityPlayer::displayGUIFurnace(TileEntityFurnace* var1)
{
}

void EntityPlayer::displayGUIEditSign(TileEntitySign* var1)
{
}

void EntityPlayer::useCurrentItemOnEntity(Entity* var1)
{
	var1->interact(this);
}

ItemStack* EntityPlayer::getCurrentEquippedItem()
{
	return this->inventory->getCurrentItem();
}

void EntityPlayer::destroyCurrentEquippedItem()
{
	this->inventory->setInventorySlotContents(this->inventory->currentItem, (ItemStack*)NULL);
}

double EntityPlayer::getYOffset()
{
	return (double)(this->yOffset - 0.5F);
}

void EntityPlayer::swingItem()
{
	this->swingProgressInt = -1;
	this->isSwinging = true;
}

void EntityPlayer::attackTargetEntityWithCurrentItem(Entity* var1)
{
	int var2 = this->inventory->getDamageVsEntity(var1);
	if(var2 > 0) {
	var1->canAttackEntity(this, var2);
	ItemStack* var3 = this->getCurrentEquippedItem();
	if(var3 != NULL && (dynamic_cast<EntityLiving*>(var1) != NULL)) {
	var3->hitEntity((EntityLiving*)var1);
	if(var3->stackSize <= 0) {
	var3->onItemDestroyedByUse(this);
	this->destroyCurrentEquippedItem();
	}
	}
	}
}

void EntityPlayer::respawnPlayer()
{
}
