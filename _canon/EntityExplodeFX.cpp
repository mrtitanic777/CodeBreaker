#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityExplodeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityExplodeFX.h"
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

EntityExplodeFX::EntityExplodeFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: EntityFX(var1, var2, var4, var6, var8, var10, var12)
{
	this->motionX = var8 + (double)((float)(javaMathRandom() * 2.0 - 1.0) * 0.05F);
	this->motionY = var10 + (double)((float)(javaMathRandom() * 2.0 - 1.0) * 0.05F);
	this->motionZ = var12 + (double)((float)(javaMathRandom() * 2.0 - 1.0) * 0.05F);
	this->particleRed = this->particleGreen = this->particleBlue = this->rand->nextFloat() * 0.3F + 0.7F;
	this->particleScale = this->rand->nextFloat() * this->rand->nextFloat() * 6.0F + 1.0F;
	this->particleMaxAge = (int)(16.0 / ((double)this->rand->nextFloat() * 0.8 + 0.2)) + 2;
}

EntityExplodeFX::~EntityExplodeFX()
{
}

void EntityExplodeFX::renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
	EntityFX::renderParticle(var1, var2, var3, var4, var5, var6, var7);
}

void EntityExplodeFX::onUpdate()
{
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	if(this->e++ >= this->particleMaxAge) {
	this->setEntityDead();
	}
	this->particleTextureIndex = 7 - this->e * 8 / this->particleMaxAge;
	this->motionY += 0.004;
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.9F;
	this->motionY *= (double)0.9F;
	this->motionZ *= (double)0.9F;
	if(this->onGround) {
	this->motionX *= (double)0.7F;
	this->motionZ *= (double)0.7F;
	}
}

void EntityExplodeFX::initStatics()
{
}
