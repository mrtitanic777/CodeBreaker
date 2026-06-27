#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMinecart.h"
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
#include "Slot.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

// PORT: Java `new int[][][]{...}` aggregate has no C++03 equivalent for heap arrays.
// Build the 10 x 2 x 3 rail-direction matrix faithfully from a flat table.
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  private static final int[][][] railDirectionMatrix = new int[][][]{{{0, 0, -1}, {0, 0, 1}}, {{-1, 0, 0}, {1, 0, 0}}, {{-1, -1, 0}, {1, 0, 0}}, {{-1, 0, 0}, {1, -1, 0}}, {{0, 0, -1}, {0, -1, 1}}, {{0, -1, -1}, {0, 0, 1}}, {{0, 0, 1}, {1, 0, 0}}, {{0, 0, 1}, {-1, 0, 0}}, {{0, 0, -1}, {-1, 0, 0}}, {{0, 0, -1}, {1, 0, 0}}};
 *  ---- end original Java ----
 *  Faithful reconstruction (same 10x2x3 values), only the literal-aggregate form changed. */
static JArray<JArray<JArray<int > > > _buildRailDirectionMatrix() {
	static const int rows[10][2][3] = {
		{{0, 0, -1}, {0, 0, 1}}, {{-1, 0, 0}, {1, 0, 0}}, {{-1, -1, 0}, {1, 0, 0}},
		{{-1, 0, 0}, {1, -1, 0}}, {{0, 0, -1}, {0, -1, 1}}, {{0, -1, -1}, {0, 0, 1}},
		{{0, 0, 1}, {1, 0, 0}}, {{0, 0, 1}, {-1, 0, 0}}, {{0, 0, -1}, {-1, 0, 0}},
		{{0, 0, -1}, {1, 0, 0}}
	};
	JArray<JArray<JArray<int > > > m(10);
	for (int i = 0; i < 10; ++i) {
		JArray<JArray<int > > pair(2);
		for (int j = 0; j < 2; ++j) {
			JArray<int> triple(3);
			for (int k = 0; k < 3; ++k) triple[k] = rows[i][j][k];
			pair[j] = triple;
		}
		m[i] = pair;
	}
	return m;
}
JArray<JArray<JArray<int > > > EntityMinecart::s_railDirectionMatrix = _buildRailDirectionMatrix();

EntityMinecart::EntityMinecart(World* var1)
	: Entity(var1)
	, a(0)
	, b(0)
	, c(0)
	, isInReverse(0)
	, d(0)
	, fuel(0)
	, pushX(0)
	, pushZ(0)
	, turnProgress(0)
	, minecartX(0)
	, minecartY(0)
	, minecartZ(0)
	, minecartYaw(0)
	, minecartPitch(0)
	, velocityX(0)
	, velocityY(0)
	, velocityZ(0)
{
	this->cargoItems = JArray<ItemStack* >(36);
	this->a = 0;
	this->b = 0;
	this->c = 1;
	this->isInReverse = false;
	this->preventEntitySpawning = true;
	this->setSize(0.98F, 0.7F);
	this->yOffset = this->height / 2.0F;
	this->entityWalks = false;
}

EntityMinecart::EntityMinecart(World* var1, double var2, double var4, double var6, int var8)
	: Entity(var1)
	, a(0)
	, b(0)
	, c(0)
	, isInReverse(0)
	, d(0)
	, fuel(0)
	, pushX(0)
	, pushZ(0)
	, turnProgress(0)
	, minecartX(0)
	, minecartY(0)
	, minecartZ(0)
	, minecartYaw(0)
	, minecartPitch(0)
	, velocityX(0)
	, velocityY(0)
	, velocityZ(0)
{
	// inlined this(var1) -- C++03 lacks ctor delegation
	this->cargoItems = JArray<ItemStack* >(36);
	this->a = 0;
	this->b = 0;
	this->c = 1;
	this->isInReverse = false;
	this->preventEntitySpawning = true;
	this->setSize(0.98F, 0.7F);
	this->yOffset = this->height / 2.0F;
	this->entityWalks = false;
	this->setPosition(var2, var4 + (double)this->yOffset, var6);
	this->motionX = 0.0;
	this->motionY = 0.0;
	this->motionZ = 0.0;
	this->prevPosX = var2;
	this->prevPosY = var4;
	this->prevPosZ = var6;
	this->d = var8;
}

EntityMinecart::~EntityMinecart()
{
}

AxisAlignedBB* EntityMinecart::getBoundingBox(Entity* var1)
{
	return var1->boundingBox;
}

AxisAlignedBB* EntityMinecart::getCollisionBox()
{
	return NULL;
}

bool EntityMinecart::canBePushed()
{
	return true;
}

double EntityMinecart::getMountedYOffset()
{
	return (double)this->height * 0.0 - (double)0.3F;
}

bool EntityMinecart::canAttackEntity(Entity* var1, int var2)
{
	if(!this->worldObj->multiplayerWorld && !this->isDead) {
	this->c = -this->c;
	this->b = 10;
	this->setBeenAttacked();
	this->a += var2 * 10;
	if(this->a > 40) {
	this->dropItemWithOffset(Item::s_minecartEmpty->shiftedIndex, 1, 0.0F);
	if(this->d == 1) {
	this->dropItemWithOffset(Block::s_crate->blockID, 1, 0.0F);
	} else if(this->d == 2) {
	this->dropItemWithOffset(Block::s_stoneOvenIdle->blockID, 1, 0.0F);
	}
	this->setEntityDead();
	}
	return true;
	} else {
	return true;
	}
}

void EntityMinecart::performHurtAnimation()
{
	System::out.println("Animating hurt");
	this->c = -this->c;
	this->b = 10;
	this->a += this->a * 10;
}

bool EntityMinecart::canBeCollidedWith()
{
	return !this->isDead;
}

void EntityMinecart::setEntityDead()
{
	for(int var1 = 0; var1 < this->getSizeInventory(); ++var1) {
	ItemStack* var2 = this->getStackInSlot(var1);
	if(var2 != NULL) {
	float var3 = this->rand->nextFloat() * 0.8F + 0.1F;
	float var4 = this->rand->nextFloat() * 0.8F + 0.1F;
	float var5 = this->rand->nextFloat() * 0.8F + 0.1F;
	while(var2->stackSize > 0) {
	int var6 = this->rand->nextInt(21) + 10;
	if(var6 > var2->stackSize) {
	var6 = var2->stackSize;
	}
	var2->stackSize -= var6;
	EntityItem* var7 = new EntityItem(this->worldObj, this->posX + (double)var3, this->posY + (double)var4, this->posZ + (double)var5, new ItemStack(var2->itemID, var6, var2->itemDamage));
	float var8 = 0.05F;
	var7->motionX = (double)((float)this->rand->nextGaussian() * var8);
	var7->motionY = (double)((float)this->rand->nextGaussian() * var8 + 0.2F);
	var7->motionZ = (double)((float)this->rand->nextGaussian() * var8);
	this->worldObj->entityJoinedWorld(var7);
	}
	}
	}
	Entity::setEntityDead();
}

void EntityMinecart::onUpdate()
{
	if(this->b > 0) {
	--this->b;
	}
	if(this->a > 0) {
	--this->a;
	}
	double var7;
	if(this->worldObj->multiplayerWorld && this->turnProgress > 0) {
	if(this->turnProgress > 0) {
	double var41 = this->posX + (this->minecartX - this->posX) / (double)this->turnProgress;
	double var42 = this->posY + (this->minecartY - this->posY) / (double)this->turnProgress;
	double var5 = this->posZ + (this->minecartZ - this->posZ) / (double)this->turnProgress;
	for(var7 = this->minecartYaw - (double)this->rotationYaw; var7 < -180.0; var7 += 360.0) {
	}
	while(var7 >= 180.0) {
	var7 -= 360.0;
	}
	this->rotationYaw = (float)((double)this->rotationYaw + var7 / (double)this->turnProgress);
	this->rotationPitch = (float)((double)this->rotationPitch + (this->minecartPitch - (double)this->rotationPitch) / (double)this->turnProgress);
	--this->turnProgress;
	this->setPosition(var41, var42, var5);
	this->setRotation(this->rotationYaw, this->rotationPitch);
	} else {
	this->setPosition(this->posX, this->posY, this->posZ);
	this->setRotation(this->rotationYaw, this->rotationPitch);
	}
	} else {
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	this->motionY -= (double)0.04F;
	int var1 = MathHelper::floor_double(this->posX);
	int var2 = MathHelper::floor_double(this->posY);
	int var3 = MathHelper::floor_double(this->posZ);
	if(this->worldObj->getBlockId(var1, var2 - 1, var3) == Block::s_minecartTrack->blockID) {
	--var2;
	}
	double var4 = 0.4;
	bool var6 = false;
	var7 = 1.0 / 128.0;
	if(this->worldObj->getBlockId(var1, var2, var3) == Block::s_minecartTrack->blockID) {
	Vec3D* var9 = this->getPosOnRail(this->posX, this->posY, this->posZ);
	int var10 = this->worldObj->getBlockMetadata(var1, var2, var3);
	this->posY = (double)var2;
	if(var10 >= 2 && var10 <= 5) {
	this->posY = (double)(var2 + 1);
	}
	if(var10 == 2) {
	this->motionX -= var7;
	}
	if(var10 == 3) {
	this->motionX += var7;
	}
	if(var10 == 4) {
	this->motionZ += var7;
	}
	if(var10 == 5) {
	this->motionZ -= var7;
	}
	JArray<JArray<int > > var11 = s_railDirectionMatrix[var10];
	double var12 = (double)(var11[1][0] - var11[0][0]);
	double var14 = (double)(var11[1][2] - var11[0][2]);
	double var16 = sqrtf(var12 * var12 + var14 * var14);
	double var18 = this->motionX * var12 + this->motionZ * var14;
	if(var18 < 0.0) {
	var12 = -var12;
	var14 = -var14;
	}
	double var20 = sqrtf(this->motionX * this->motionX + this->motionZ * this->motionZ);
	this->motionX = var20 * var12 / var16;
	this->motionZ = var20 * var14 / var16;
	double var22 = 0.0;
	double var24 = (double)var1 + 0.5 + (double)var11[0][0] * 0.5;
	double var26 = (double)var3 + 0.5 + (double)var11[0][2] * 0.5;
	double var28 = (double)var1 + 0.5 + (double)var11[1][0] * 0.5;
	double var30 = (double)var3 + 0.5 + (double)var11[1][2] * 0.5;
	var12 = var28 - var24;
	var14 = var30 - var26;
	double var32;
	double var34;
	double var36;
	if(var12 == 0.0) {
	this->posX = (double)var1 + 0.5;
	var22 = this->posZ - (double)var3;
	} else if(var14 == 0.0) {
	this->posZ = (double)var3 + 0.5;
	var22 = this->posX - (double)var1;
	} else {
	var32 = this->posX - var24;
	var34 = this->posZ - var26;
	var36 = (var32 * var12 + var34 * var14) * 2.0;
	var22 = var36;
	}
	this->posX = var24 + var12 * var22;
	this->posZ = var26 + var14 * var22;
	this->setPosition(this->posX, this->posY + (double)this->yOffset, this->posZ);
	var32 = this->motionX;
	var34 = this->motionZ;
	if(this->riddenByEntity != NULL) {
	var32 *= 0.75;
	var34 *= 0.75;
	}
	if(var32 < -var4) {
	var32 = -var4;
	}
	if(var32 > var4) {
	var32 = var4;
	}
	if(var34 < -var4) {
	var34 = -var4;
	}
	if(var34 > var4) {
	var34 = var4;
	}
	this->moveEntity(var32, 0.0, var34);
	if(var11[0][1] != 0 && MathHelper::floor_double(this->posX) - var1 == var11[0][0] && MathHelper::floor_double(this->posZ) - var3 == var11[0][2]) {
	this->setPosition(this->posX, this->posY + (double)var11[0][1], this->posZ);
	} else if(var11[1][1] != 0 && MathHelper::floor_double(this->posX) - var1 == var11[1][0] && MathHelper::floor_double(this->posZ) - var3 == var11[1][2]) {
	this->setPosition(this->posX, this->posY + (double)var11[1][1], this->posZ);
	}
	if(this->riddenByEntity != NULL) {
	this->motionX *= (double)0.997F;
	this->motionY *= 0.0;
	this->motionZ *= (double)0.997F;
	} else {
	if(this->d == 2) {
	var36 = (double)MathHelper::sqrt_double(this->pushX * this->pushX + this->pushZ * this->pushZ);
	if(var36 > 0.01) {
	var6 = true;
	this->pushX /= var36;
	this->pushZ /= var36;
	double var38 = 0.04;
	this->motionX *= (double)0.8F;
	this->motionY *= 0.0;
	this->motionZ *= (double)0.8F;
	this->motionX += this->pushX * var38;
	this->motionZ += this->pushZ * var38;
	} else {
	this->motionX *= (double)0.9F;
	this->motionY *= 0.0;
	this->motionZ *= (double)0.9F;
	}
	}
	this->motionX *= (double)0.96F;
	this->motionY *= 0.0;
	this->motionZ *= (double)0.96F;
	}
	Vec3D* var46 = this->getPosOnRail(this->posX, this->posY, this->posZ);
	if(var46 != NULL && var9 != NULL) {
	double var37 = (var9->yCoord - var46->yCoord) * 0.05;
	var20 = sqrtf(this->motionX * this->motionX + this->motionZ * this->motionZ);
	if(var20 > 0.0) {
	this->motionX = this->motionX / var20 * (var20 + var37);
	this->motionZ = this->motionZ / var20 * (var20 + var37);
	}
	this->setPosition(this->posX, var46->yCoord, this->posZ);
	}
	int var47 = MathHelper::floor_double(this->posX);
	int var48 = MathHelper::floor_double(this->posZ);
	if(var47 != var1 || var48 != var3) {
	var20 = sqrtf(this->motionX * this->motionX + this->motionZ * this->motionZ);
	this->motionX = var20 * (double)(var47 - var1);
	this->motionZ = var20 * (double)(var48 - var3);
	}
	if(this->d == 2) {
	double var39 = (double)MathHelper::sqrt_double(this->pushX * this->pushX + this->pushZ * this->pushZ);
	if(var39 > 0.01 && this->motionX * this->motionX + this->motionZ * this->motionZ > 0.001) {
	this->pushX /= var39;
	this->pushZ /= var39;
	if(this->pushX * this->motionX + this->pushZ * this->motionZ < 0.0) {
	this->pushX = 0.0;
	this->pushZ = 0.0;
	} else {
	this->pushX = this->motionX;
	this->pushZ = this->motionZ;
	}
	}
	}
	} else {
	if(this->motionX < -var4) {
	this->motionX = -var4;
	}
	if(this->motionX > var4) {
	this->motionX = var4;
	}
	if(this->motionZ < -var4) {
	this->motionZ = -var4;
	}
	if(this->motionZ > var4) {
	this->motionZ = var4;
	}
	if(this->onGround) {
	this->motionX *= 0.5;
	this->motionY *= 0.5;
	this->motionZ *= 0.5;
	}
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	if(!this->onGround) {
	this->motionX *= (double)0.95F;
	this->motionY *= (double)0.95F;
	this->motionZ *= (double)0.95F;
	}
	}
	this->rotationPitch = 0.0F;
	double var43 = this->prevPosX - this->posX;
	double var44 = this->prevPosZ - this->posZ;
	if(var43 * var43 + var44 * var44 > 0.001) {
	this->rotationYaw = (float)(atan2f(var44, var43) * 180.0 / 3.1415927F);
	if(this->isInReverse) {
	this->rotationYaw += 180.0F;
	}
	}
	double var13;
	for(var13 = (double)(this->rotationYaw - this->prevRotationYaw); var13 >= 180.0; var13 -= 360.0) {
	}
	while(var13 < -180.0) {
	var13 += 360.0;
	}
	if(var13 < -170.0 || var13 >= 170.0) {
	this->rotationYaw += 180.0F;
	this->isInReverse = !this->isInReverse;
	}
	this->setRotation(this->rotationYaw, this->rotationPitch);
	JList<void*> var15 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this, this->boundingBox->expands((double)0.2F, 0.0, (double)0.2F));
	if(var15 != NULL && var15.size() > 0) {
	for(int var45 = 0; var45 < var15.size(); ++var45) {
	Entity* var17 = (Entity*)var15[var45];
	if(var17 != this->riddenByEntity && var17->canBePushed() && (dynamic_cast<EntityMinecart*>(var17) != NULL)) {
	var17->applyEntityCollision(this);
	}
	}
	}
	if(this->riddenByEntity != NULL && this->riddenByEntity->isDead) {
	this->riddenByEntity = NULL;
	}
	if(var6 && this->rand->nextInt(4) == 0) {
	--this->fuel;
	if(this->fuel < 0) {
	this->pushX = this->pushZ = 0.0;
	}
	this->worldObj->spawnParticle("largesmoke",this->posX,this->posY + 0.8,this->posZ,0.0,0.0,0.0);
	}
	}
}

Vec3D* EntityMinecart::getPosOffsetFromRail(double var1, double var3, double var5, double var7)
{
	int var9 = MathHelper::floor_double(var1);
	int var10 = MathHelper::floor_double(var3);
	int var11 = MathHelper::floor_double(var5);
	if(this->worldObj->getBlockId(var9, var10 - 1, var11) == Block::s_minecartTrack->blockID) {
	--var10;
	}
	if(this->worldObj->getBlockId(var9, var10, var11) == Block::s_minecartTrack->blockID) {
	int var12 = this->worldObj->getBlockMetadata(var9, var10, var11);
	var3 = (double)var10;
	if(var12 >= 2 && var12 <= 5) {
	var3 = (double)(var10 + 1);
	}
	JArray<JArray<int > > var13 = s_railDirectionMatrix[var12];
	double var14 = (double)(var13[1][0] - var13[0][0]);
	double var16 = (double)(var13[1][2] - var13[0][2]);
	double var18 = sqrtf(var14 * var14 + var16 * var16);
	var14 /= var18;
	var16 /= var18;
	var1 += var14 * var7;
	var5 += var16 * var7;
	if(var13[0][1] != 0 && MathHelper::floor_double(var1) - var9 == var13[0][0] && MathHelper::floor_double(var5) - var11 == var13[0][2]) {
	var3 += (double)var13[0][1];
	} else if(var13[1][1] != 0 && MathHelper::floor_double(var1) - var9 == var13[1][0] && MathHelper::floor_double(var5) - var11 == var13[1][2]) {
	var3 += (double)var13[1][1];
	}
	return this->getPosOnRail(var1, var3, var5);
	} else {
	return NULL;
	}
}

Vec3D* EntityMinecart::getPosOnRail(double var1, double var3, double var5)
{
	int var7 = MathHelper::floor_double(var1);
	int var8 = MathHelper::floor_double(var3);
	int var9 = MathHelper::floor_double(var5);
	if(this->worldObj->getBlockId(var7, var8 - 1, var9) == Block::s_minecartTrack->blockID) {
	--var8;
	}
	if(this->worldObj->getBlockId(var7, var8, var9) == Block::s_minecartTrack->blockID) {
	int var10 = this->worldObj->getBlockMetadata(var7, var8, var9);
	var3 = (double)var8;
	if(var10 >= 2 && var10 <= 5) {
	var3 = (double)(var8 + 1);
	}
	JArray<JArray<int > > var11 = s_railDirectionMatrix[var10];
	double var12 = 0.0;
	double var14 = (double)var7 + 0.5 + (double)var11[0][0] * 0.5;
	double var16 = (double)var8 + 0.5 + (double)var11[0][1] * 0.5;
	double var18 = (double)var9 + 0.5 + (double)var11[0][2] * 0.5;
	double var20 = (double)var7 + 0.5 + (double)var11[1][0] * 0.5;
	double var22 = (double)var8 + 0.5 + (double)var11[1][1] * 0.5;
	double var24 = (double)var9 + 0.5 + (double)var11[1][2] * 0.5;
	double var26 = var20 - var14;
	double var28 = (var22 - var16) * 2.0;
	double var30 = var24 - var18;
	if(var26 == 0.0) {
	var1 = (double)var7 + 0.5;
	var12 = var5 - (double)var9;
	} else if(var30 == 0.0) {
	var5 = (double)var9 + 0.5;
	var12 = var1 - (double)var7;
	} else {
	double var32 = var1 - var14;
	double var34 = var5 - var18;
	double var36 = (var32 * var26 + var34 * var30) * 2.0;
	var12 = var36;
	}
	var1 = var14 + var26 * var12;
	var3 = var16 + var28 * var12;
	var5 = var18 + var30 * var12;
	if(var28 < 0.0) {
	++var3;
	}
	if(var28 > 0.0) {
	var3 += 0.5;
	}
	return Vec3D::createVector(var1, var3, var5);
	} else {
	return NULL;
	}
}

void EntityMinecart::writeEntityToNBT(NBTTagCompound* var1)
{
	var1->setInteger("Type", this->d);
	if(this->d == 2) {
	var1->setDouble("PushX", this->pushX);
	var1->setDouble("PushZ", this->pushZ);
	var1->setShort("Fuel", (short)this->fuel);
	} else if(this->d == 1) {
	NBTTagList* var2 = new NBTTagList();
	for(int var3 = 0; var3 < this->cargoItems.length; ++var3) {
	if(this->cargoItems[var3] != NULL) {
	NBTTagCompound* var4 = new NBTTagCompound();
	var4->setByte("Slot", (unsigned char)var3);
	if (this->cargoItems[var3] != NULL) { this->cargoItems[var3]->writeToNBT(var4); }
	var2->setTag(var4);
	}
	}
	var1->setTag("Items", var2);
	}
}

void EntityMinecart::readEntityFromNBT(NBTTagCompound* var1)
{
	this->d = var1->getInteger("Type");
	if(this->d == 2) {
	this->pushX = var1->getDouble("PushX");
	this->pushZ = var1->getDouble("PushZ");
	this->fuel = var1->getShort("Fuel");
	} else if(this->d == 1) {
	NBTTagList* var2 = var1->getTagList("Items");
	this->cargoItems = JArray<ItemStack* >(this->getSizeInventory());
	for(int var3 = 0; var3 < var2->tagCount(); ++var3) {
	NBTTagCompound* var4 = (NBTTagCompound*)var2->tagAt(var3);
	int var5 = var4->getByte("Slot") & 255;
	if(var5 >= 0 && var5 < this->cargoItems.length) {
	this->cargoItems[var5] = new ItemStack(var4);
	}
	}
	}
}

float EntityMinecart::getYOffsetForRender()
{
	return 0.0F;
}

void EntityMinecart::applyEntityCollision(Entity* var1)
{
	if(!this->worldObj->multiplayerWorld) {
	if(var1 != this->riddenByEntity) {
	if((dynamic_cast<EntityLiving*>(var1) != NULL) && !((dynamic_cast<EntityPlayer*>(var1) != NULL)) && this->d == 0 && this->motionX * this->motionX + this->motionZ * this->motionZ > 0.01 && this->riddenByEntity == NULL && var1->ridingEntity == NULL) {
	var1->mountEntity(this);
	}
	double var2 = var1->posX - this->posX;
	double var4 = var1->posZ - this->posZ;
	double var6 = var2 * var2 + var4 * var4;
	if(var6 >= (double)1.0E-4F) {
	var6 = (double)MathHelper::sqrt_double(var6);
	var2 /= var6;
	var4 /= var6;
	double var8 = 1.0 / var6;
	if(var8 > 1.0) {
	var8 = 1.0;
	}
	var2 *= var8;
	var4 *= var8;
	var2 *= (double)0.1F;
	var4 *= (double)0.1F;
	var2 *= (double)(1.0F - this->entityCollisionReduction);
	var4 *= (double)(1.0F - this->entityCollisionReduction);
	var2 *= 0.5;
	var4 *= 0.5;
	if((dynamic_cast<EntityMinecart*>(var1) != NULL)) {
	double var10 = var1->motionX + this->motionX;
	double var12 = var1->motionZ + this->motionZ;
	if(((EntityMinecart*)var1)->d == 2 && this->d != 2) {
	this->motionX *= (double)0.2F;
	this->motionZ *= (double)0.2F;
	this->addVelocity(var1->motionX - var2, 0.0, var1->motionZ - var4);
	var1->motionX *= (double)0.7F;
	var1->motionZ *= (double)0.7F;
	} else if(((EntityMinecart*)var1)->d != 2 && this->d == 2) {
	var1->motionX *= (double)0.2F;
	var1->motionZ *= (double)0.2F;
	var1->addVelocity(this->motionX + var2, 0.0, this->motionZ + var4);
	this->motionX *= (double)0.7F;
	this->motionZ *= (double)0.7F;
	} else {
	var10 /= 2.0;
	var12 /= 2.0;
	this->motionX *= (double)0.2F;
	this->motionZ *= (double)0.2F;
	this->addVelocity(var10 - var2, 0.0, var12 - var4);
	var1->motionX *= (double)0.2F;
	var1->motionZ *= (double)0.2F;
	var1->addVelocity(var10 + var2, 0.0, var12 + var4);
	}
	} else {
	this->addVelocity(-var2, 0.0, -var4);
	var1->addVelocity(var2 / 4.0, 0.0, var4 / 4.0);
	}
	}
	}
	}
}

int EntityMinecart::getSizeInventory()
{
	return 27;
}

ItemStack* EntityMinecart::getStackInSlot(int var1)
{
	return this->cargoItems[var1];
}

ItemStack* EntityMinecart::decrStackSize(int var1, int var2)
{
	if(this->cargoItems[var1] != NULL) {
	ItemStack* var3;
	if(this->cargoItems[var1]->stackSize <= var2) {
	var3 = this->cargoItems[var1];
	this->cargoItems[var1] = NULL;
	return var3;
	} else {
	var3 = this->cargoItems[var1]->splitStack(var2);
	if(this->cargoItems[var1]->stackSize == 0) {
	this->cargoItems[var1] = NULL;
	}
	return var3;
	}
	} else {
	return NULL;
	}
}

void EntityMinecart::setInventorySlotContents(int var1, ItemStack* var2)
{
	this->cargoItems[var1] = var2;
	if(var2 != NULL && var2->stackSize > this->getInventoryStackLimit()) {
	var2->stackSize = this->getInventoryStackLimit();
	}
}

JavaString EntityMinecart::getInvName()
{
	return "Minecart";
}

int EntityMinecart::getInventoryStackLimit()
{
	return 64;
}

void EntityMinecart::onInventoryChanged()
{
}

bool EntityMinecart::interact(EntityPlayer* var1)
{
	if(this->d == 0) {
	if(this->riddenByEntity != NULL && (dynamic_cast<EntityPlayer*>(this->riddenByEntity) != NULL) && this->riddenByEntity != var1) {
	return true;
	}
	if(!this->worldObj->multiplayerWorld) {
	var1->mountEntity(this);
	}
	} else if(this->d == 1) {
	var1->displayGUIChest(this);
	} else if(this->d == 2) {
	ItemStack* var2 = var1->inventory->getCurrentItem();
	if(var2 != NULL && var2->itemID == Item::s_coal->shiftedIndex) {
	if(--var2->stackSize == 0) {
	var1->inventory->setInventorySlotContents(var1->inventory->currentItem, (ItemStack*)NULL);
	}
	this->fuel += 1200;
	}
	this->pushX = this->posX - var1->posX;
	this->pushZ = this->posZ - var1->posZ;
	}
	return true;
}

void EntityMinecart::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9)
{
	this->minecartX = var1;
	this->minecartY = var3;
	this->minecartZ = var5;
	this->minecartYaw = (double)var7;
	this->minecartPitch = (double)var8;
	this->turnProgress = var9 + 2;
	this->motionX = this->velocityX;
	this->motionY = this->velocityY;
	this->motionZ = this->velocityZ;
}

void EntityMinecart::setVelocity(double var1, double var3, double var5)
{
	this->velocityX = this->motionX = var1;
	this->velocityY = this->motionY = var3;
	this->velocityZ = this->motionZ = var5;
}
