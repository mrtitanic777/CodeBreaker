#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityRainFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityRainFX.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockFluids.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "ByteBuffer.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFX.h"
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

EntityRainFX::EntityRainFX(World* var1, double var2, double var4, double var6)
	: EntityFX(var1, var2, var4, var6, 0.0, 0.0, 0.0)
{
	this->motionX *= (double)0.3F;
	this->motionY = (double)((float)javaMathRandom() * 0.2F + 0.1F);
	this->motionZ *= (double)0.3F;
	this->particleRed = 1.0F;
	this->particleGreen = 1.0F;
	this->particleBlue = 1.0F;
	this->particleTextureIndex = 19 + this->rand->nextInt(4);
	this->setSize(0.01F, 0.01F);
	this->particleGravity = 0.06F;
	this->particleMaxAge = (int)(8.0 / (javaMathRandom() * 0.8 + 0.2));
}

EntityRainFX::~EntityRainFX()
{
}

void EntityRainFX::renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
	EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

void EntityRainFX::onUpdate()
{
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	this->motionY -= (double)this->particleGravity;
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.98F;
	this->motionY *= (double)0.98F;
	this->motionZ *= (double)0.98F;
	if(this->particleMaxAge-- <= 0) {
	this->setEntityDead();
	}
	if(this->onGround) {
	if(javaMathRandom() < 0.5) {
	this->setEntityDead();
	}
	this->motionX *= (double)0.7F;
	this->motionZ *= (double)0.7F;
	}
	Material* var1 = this->worldObj->getBlockMaterial(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->posY), MathHelper::floor_double(this->posZ));
	if(var1->getIsLiquid() || var1->isSolid()) {
	double var2 = (double)((float)(MathHelper::floor_double(this->posY) + 1) - BlockFluids::getFluidHeightPercent(this->worldObj->getBlockMetadata(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->posY), MathHelper::floor_double(this->posZ))));
	if(this->posY < var2) {
	this->setEntityDead();
	}
	}
}

void EntityRainFX::initStatics()
{
}
