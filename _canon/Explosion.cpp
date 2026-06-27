#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Explosion.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Explosion.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "ChunkPosition.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
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

Explosion::Explosion(World* var1, Entity* var2, double var3, double var5, double var7, float var9)
	: isFlaming(0)
	, explosionRNG(NULL)
	, worldObj(NULL)
	, explosionX(0)
	, explosionY(0)
	, explosionZ(0)
	, exploder(NULL)
	, explosionSize(0)
{
	this->isFlaming = false;
	this->explosionRNG = new Random();
	this->destroyedBlockPositions = std::set<void*>();
	this->worldObj = var1;
	this->exploder = var2;
	this->explosionSize = var9;
	this->explosionX = var3;
	this->explosionY = var5;
	this->explosionZ = var7;
}

Explosion::~Explosion()
{
}

void Explosion::doExplosionA()
{
	float var1 = this->explosionSize;
	unsigned char var2 = 16;
	int var3;
	int var4;
	int var5;
	double var15;
	double var17;
	double var19;
	for(var3 = 0; var3 < var2; ++var3) {
	for(var4 = 0; var4 < var2; ++var4) {
	for(var5 = 0; var5 < var2; ++var5) {
	if(var3 == 0 || var3 == var2 - 1 || var4 == 0 || var4 == var2 - 1 || var5 == 0 || var5 == var2 - 1) {
	double var6 = (double)((float)var3 / ((float)var2 - 1.0F) * 2.0F - 1.0F);
	double var8 = (double)((float)var4 / ((float)var2 - 1.0F) * 2.0F - 1.0F);
	double var10 = (double)((float)var5 / ((float)var2 - 1.0F) * 2.0F - 1.0F);
	double var12 = sqrtf(var6 * var6 + var8 * var8 + var10 * var10);
	var6 /= var12;
	var8 /= var12;
	var10 /= var12;
	float var14 = this->explosionSize * (0.7F + this->worldObj->rand->nextFloat() * 0.6F);
	var15 = this->explosionX;
	var17 = this->explosionY;
	var19 = this->explosionZ;
	for(float var21 = 0.3F; var14 > 0.0F; var14 -= var21 * (12.0F / 16.0F)) {
	int var22 = MathHelper::floor_double(var15);
	int var23 = MathHelper::floor_double(var17);
	int var24 = MathHelper::floor_double(var19);
	int var25 = this->worldObj->getBlockId(var22, var23, var24);
	if(var25 > 0) {
	var14 -= (Block::s_blocksList[var25]->getExplosionResistance(this->exploder) + 0.3F) * var21;
	}
	if(var14 > 0.0F) {
	this->destroyedBlockPositions.insert(new ChunkPosition(var22, var23, var24));
	}
	var15 += var6 * (double)var21;
	var17 += var8 * (double)var21;
	var19 += var10 * (double)var21;
	}
	}
	}
	}
	}
	this->explosionSize *= 2.0F;
	var3 = MathHelper::floor_double(this->explosionX - (double)this->explosionSize - 1.0);
	var4 = MathHelper::floor_double(this->explosionX + (double)this->explosionSize + 1.0);
	var5 = MathHelper::floor_double(this->explosionY - (double)this->explosionSize - 1.0);
	int var29 = MathHelper::floor_double(this->explosionY + (double)this->explosionSize + 1.0);
	int var7 = MathHelper::floor_double(this->explosionZ - (double)this->explosionSize - 1.0);
	int var30 = MathHelper::floor_double(this->explosionZ + (double)this->explosionSize + 1.0);
	JList<void*> var9 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this->exploder, AxisAlignedBB::getBoundingBoxFromPool((double)var3, (double)var5, (double)var7, (double)var4, (double)var29, (double)var30));
	Vec3D* var31 = Vec3D::createVector(this->explosionX, this->explosionY, this->explosionZ);
	for(int var11 = 0; var11 < var9.size(); ++var11) {
	Entity* var33 = (Entity*)var9[var11];
	double var13 = var33->getDistance(this->explosionX, this->explosionY, this->explosionZ) / (double)this->explosionSize;
	if(var13 <= 1.0) {
	var15 = var33->posX - this->explosionX;
	var17 = var33->posY - this->explosionY;
	var19 = var33->posZ - this->explosionZ;
	double var39 = (double)MathHelper::sqrt_double(var15 * var15 + var17 * var17 + var19 * var19);
	var15 /= var39;
	var17 /= var39;
	var19 /= var39;
	double var40 = (double)this->worldObj->addKey(var31, var33->boundingBox);
	double var41 = (1.0 - var13) * var40;
	var33->canAttackEntity(this->exploder, (int)((var41 * var41 + var41) / 2.0 * 8.0 * (double)this->explosionSize + 1.0));
	var33->motionX += var15 * var41;
	var33->motionY += var17 * var41;
	var33->motionZ += var19 * var41;
	}
	}
	this->explosionSize = var1;
	JList<void*> var32;
	var32.addAll(this->destroyedBlockPositions);
	if(this->isFlaming) {
	for(int var34 = var32.size() - 1; var34 >= 0; --var34) {
	ChunkPosition* var35 = (ChunkPosition*)var32[var34];
	int var36 = var35->x;
	int var37 = var35->y;
	int var16 = var35->z;
	int var38 = this->worldObj->getBlockId(var36, var37, var16);
	int var18 = this->worldObj->getBlockId(var36, var37 - 1, var16);
	if(var38 == 0 && Block::s_opaqueCubeLookup[var18] && this->explosionRNG->nextInt(3) == 0) {
	this->worldObj->setBlockWithNotify(var36, var37, var16, Block::s_fire->blockID);
	}
	}
	}
}

void Explosion::doExplosionB()
{
	this->worldObj->playSoundEffect(this->explosionX,this->explosionY,this->explosionZ,"random.explode",4.0F,(1.0F + (this->worldObj->rand->nextFloat() - this->worldObj->rand->nextFloat()) * 0.2F) * 0.7F);
	JList<void*> var1;
	var1.addAll(this->destroyedBlockPositions);
	for(int var2 = var1.size() - 1; var2 >= 0; --var2) {
	ChunkPosition* var3 = (ChunkPosition*)var1[var2];
	int var4 = var3->x;
	int var5 = var3->y;
	int var6 = var3->z;
	int var7 = this->worldObj->getBlockId(var4, var5, var6);
	for(int var8 = 0; var8 < 1; ++var8) {
	double var9 = (double)((float)var4 + this->worldObj->rand->nextFloat());
	double var11 = (double)((float)var5 + this->worldObj->rand->nextFloat());
	double var13 = (double)((float)var6 + this->worldObj->rand->nextFloat());
	double var15 = var9 - this->explosionX;
	double var17 = var11 - this->explosionY;
	double var19 = var13 - this->explosionZ;
	double var21 = (double)MathHelper::sqrt_double(var15 * var15 + var17 * var17 + var19 * var19);
	var15 /= var21;
	var17 /= var21;
	var19 /= var21;
	double var23 = 0.5 / (var21 / (double)this->explosionSize + 0.1);
	var23 *= (double)(this->worldObj->rand->nextFloat() * this->worldObj->rand->nextFloat() + 0.3F);
	var15 *= var23;
	var17 *= var23;
	var19 *= var23;
	this->worldObj->spawnParticle("explode",(var9 + this->explosionX * 1.0) / 2.0,(var11 + this->explosionY * 1.0) / 2.0,(var13 + this->explosionZ * 1.0) / 2.0,var15,var17,var19);
	this->worldObj->spawnParticle("smoke", var9, var11, var13, var15, var17, var19);
	}
	if(var7 > 0) {
	if (Block::s_blocksList[var7] != NULL) { Block::s_blocksList[var7]->dropBlockAsItemWithChance(this->worldObj, var4, var5, var6, this->worldObj->getBlockMetadata(var4, var5, var6), 0.3F); }
	this->worldObj->setBlockWithNotify(var4, var5, var6, 0);
	if (Block::s_blocksList[var7] != NULL) { Block::s_blocksList[var7]->onBlockDestroyedByExplosion(this->worldObj, var4, var5, var6); }
	}
	}
}

void Explosion::initStatics()
{
}
