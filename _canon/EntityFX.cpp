#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "ByteBuffer.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IntBuffer.h"
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
#include "Tessellator.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

double EntityFX::s_interpPosX = 0;
double EntityFX::s_interpPosY = 0;
double EntityFX::s_interpPosZ = 0;

EntityFX::EntityFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: Entity(var1)
	, particleTextureIndex(0)
	, particleTextureJitterX(0)
	, particleTextureJitterY(0)
	, e(0)
	, particleMaxAge(0)
	, particleScale(0)
	, particleGravity(0)
	, particleRed(0)
	, particleGreen(0)
	, particleBlue(0)
{
	this->e = 0;
	this->particleMaxAge = 0;
	this->setSize(0.2F, 0.2F);
	this->yOffset = this->height / 2.0F;
	this->setPosition(var2, var4, var6);
	this->particleRed = this->particleGreen = this->particleBlue = 1.0F;
	this->motionX = var8 + (double)((float)(javaMathRandom() * 2.0 - 1.0) * 0.4F);
	this->motionY = var10 + (double)((float)(javaMathRandom() * 2.0 - 1.0) * 0.4F);
	this->motionZ = var12 + (double)((float)(javaMathRandom() * 2.0 - 1.0) * 0.4F);
	float var14 = (float)(javaMathRandom() + javaMathRandom() + 1.0) * 0.15F;
	float var15 = MathHelper::sqrt_double(this->motionX * this->motionX + this->motionY * this->motionY + this->motionZ * this->motionZ);
	this->motionX = this->motionX / (double)var15 * (double)var14 * (double)0.4F;
	this->motionY = this->motionY / (double)var15 * (double)var14 * (double)0.4F + (double)0.1F;
	this->motionZ = this->motionZ / (double)var15 * (double)var14 * (double)0.4F;
	this->particleTextureJitterX = this->rand->nextFloat() * 3.0F;
	this->particleTextureJitterY = this->rand->nextFloat() * 3.0F;
	this->particleScale = (this->rand->nextFloat() * 0.5F + 0.5F) * 2.0F;
	this->particleMaxAge = (int)(4.0F / (this->rand->nextFloat() * 0.9F + 0.1F));
	this->e = 0;
	this->entityWalks = false;
}

EntityFX::~EntityFX()
{
}

EntityFX* EntityFX::multiplyVelocity(float var1)
{
	this->motionX *= (double)var1;
	this->motionY = (this->motionY - (double)0.1F) * (double)var1 + (double)0.1F;
	this->motionZ *= (double)var1;
	return this;
}

EntityFX* EntityFX::multipleParticleScaleBy(float var1)
{
	this->setSize(0.2F * var1, 0.2F * var1);
	this->particleScale *= var1;
	return this;
}

void EntityFX::onUpdate()
{
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	if(this->e++ >= this->particleMaxAge) {
	this->setEntityDead();
	}
	this->motionY -= 0.04 * (double)this->particleGravity;
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.98F;
	this->motionY *= (double)0.98F;
	this->motionZ *= (double)0.98F;
	if(this->onGround) {
	this->motionX *= (double)0.7F;
	this->motionZ *= (double)0.7F;
	}
}

void EntityFX::renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
	float var8 = (float)(this->particleTextureIndex % 16) / 16.0F;
	float var9 = var8 + 0.999F / 16.0F;
	float var10 = (float)(this->particleTextureIndex / 16) / 16.0F;
	float var11 = var10 + 0.999F / 16.0F;
	float var12 = 0.1F * this->particleScale;
	float var13 = (float)(this->prevPosX + (this->posX - this->prevPosX) * (double)var2 - s_interpPosX);
	float var14 = (float)(this->prevPosY + (this->posY - this->prevPosY) * (double)var2 - s_interpPosY);
	float var15 = (float)(this->prevPosZ + (this->posZ - this->prevPosZ) * (double)var2 - s_interpPosZ);
	float var16 = this->getEntityBrightness(var2);
	var1->setColorOpaque_F(this->particleRed * var16, this->particleGreen * var16, this->particleBlue * var16);
	var1->addVertexWithUV((double)(var13 - var3 * var12 - var6 * var12), (double)(var14 - var4 * var12), (double)(var15 - var5 * var12 - var7 * var12), (double)var8, (double)var11);
	var1->addVertexWithUV((double)(var13 - var3 * var12 + var6 * var12), (double)(var14 + var4 * var12), (double)(var15 - var5 * var12 + var7 * var12), (double)var8, (double)var10);
	var1->addVertexWithUV((double)(var13 + var3 * var12 + var6 * var12), (double)(var14 + var4 * var12), (double)(var15 + var5 * var12 + var7 * var12), (double)var9, (double)var10);
	var1->addVertexWithUV((double)(var13 + var3 * var12 - var6 * var12), (double)(var14 - var4 * var12), (double)(var15 + var5 * var12 - var7 * var12), (double)var9, (double)var11);
}

int EntityFX::getFXLayer()
{
	return 0;
}

void EntityFX::writeEntityToNBT(NBTTagCompound* var1)
{
}

void EntityFX::readEntityFromNBT(NBTTagCompound* var1)
{
}

void EntityFX::initStatics()
{
}
