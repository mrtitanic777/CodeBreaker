#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityOtherPlayerMP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityOtherPlayerMP.h"
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

EntityOtherPlayerMP::EntityOtherPlayerMP(World* var1, const JavaString& var2)
	: EntityPlayer(var1)
	, otherPlayerMPPosRotationIncrements(0)
	, otherPlayerMPX(0)
	, otherPlayerMPY(0)
	, otherPlayerMPZ(0)
	, otherPlayerMPYaw(0)
	, otherPlayerMPPitch(0)
	, a(0)
{
	this->a = 0.0F;
	this->field_771_i = var2;
	this->yOffset = 0.0F;
	this->stepHeight = 0.0F;
	if(var2 != NULL && var2.length() > 0) {
	this->skinUrl = (JCat() + "http://www.minecraft.net/skin/" + var2 + ".png");
	System::out.println((JCat() + "Loading texture " + this->skinUrl));
	}
	this->noClip = true;
	this->renderDistanceWeight = 10.0;
}

EntityOtherPlayerMP::~EntityOtherPlayerMP()
{
}

bool EntityOtherPlayerMP::canAttackEntity(Entity* var1, int var2)
{
	return true;
}

void EntityOtherPlayerMP::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9)
{
	this->yOffset = 0.0F;
	this->otherPlayerMPX = var1;
	this->otherPlayerMPY = var3;
	this->otherPlayerMPZ = var5;
	this->otherPlayerMPYaw = (double)var7;
	this->otherPlayerMPPitch = (double)var8;
	this->otherPlayerMPPosRotationIncrements = var9;
}

void EntityOtherPlayerMP::onUpdate()
{
	EntityPlayer::onUpdate();
	this->prevLimbSwingAmount = this->limbSwingAmount;
	double var1 = this->posX - this->prevPosX;
	double var3 = this->posZ - this->prevPosZ;
	float var5 = MathHelper::sqrt_double(var1 * var1 + var3 * var3) * 4.0F;
	if(var5 > 1.0F) {
	var5 = 1.0F;
	}
	this->limbSwingAmount += (var5 - this->limbSwingAmount) * 0.4F;
	this->limbSwing += this->limbSwingAmount;
}

float EntityOtherPlayerMP::getYOffsetForRender()
{
	return 0.0F;
}

void EntityOtherPlayerMP::onLivingUpdate()
{
	EntityPlayer::updateEntityActionState();
	if(this->otherPlayerMPPosRotationIncrements > 0) {
	double var1 = this->posX + (this->otherPlayerMPX - this->posX) / (double)this->otherPlayerMPPosRotationIncrements;
	double var3 = this->posY + (this->otherPlayerMPY - this->posY) / (double)this->otherPlayerMPPosRotationIncrements;
	double var5 = this->posZ + (this->otherPlayerMPZ - this->posZ) / (double)this->otherPlayerMPPosRotationIncrements;
	double var7;
	for(var7 = this->otherPlayerMPYaw - (double)this->rotationYaw; var7 < -180.0; var7 += 360.0) {
	}
	while(var7 >= 180.0) {
	var7 -= 360.0;
	}
	this->rotationYaw = (float)((double)this->rotationYaw + var7 / (double)this->otherPlayerMPPosRotationIncrements);
	this->rotationPitch = (float)((double)this->rotationPitch + (this->otherPlayerMPPitch - (double)this->rotationPitch) / (double)this->otherPlayerMPPosRotationIncrements);
	--this->otherPlayerMPPosRotationIncrements;
	this->setPosition(var1, var3, var5);
	this->setRotation(this->rotationYaw, this->rotationPitch);
	}
	this->prevCameraYaw = this->cameraYaw;
	float var9 = MathHelper::sqrt_double(this->motionX * this->motionX + this->motionZ * this->motionZ);
	float var2 = (float)atanf(-this->motionY * (double)0.2F) * 15.0F;
	if(var9 > 0.1F) {
	var9 = 0.1F;
	}
	if(!this->onGround || this->health <= 0) {
	var9 = 0.0F;
	}
	if(this->onGround || this->health <= 0) {
	var2 = 0.0F;
	}
	this->cameraYaw += (var9 - this->cameraYaw) * 0.4F;
	this->field_9328_R += (var2 - this->field_9328_R) * 0.8F;
}

bool EntityOtherPlayerMP::isSneaking()
{
	return this->isSneakingRender;
}

void EntityOtherPlayerMP::initStatics()
{
}
