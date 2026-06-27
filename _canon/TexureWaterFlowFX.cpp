#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TexureWaterFlowFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexureWaterFlowFX.h"
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
#include "TextureFX.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

TexureWaterFlowFX::TexureWaterFlowFX()
	: TextureFX(Block::s_waterStill->blockIndexInTexture + 1)
	, tickCounter(0)
{
	this->field = JArray<float >(256);
	this->tempField = JArray<float >(256);
	this->heightField = JArray<float >(256);
	this->soundField = JArray<float >(256);
	this->tickCounter = 0;
	this->tileSize = 2;
}

TexureWaterFlowFX::~TexureWaterFlowFX()
{
}

void TexureWaterFlowFX::onTick()
{
	++this->tickCounter;
	int var1;
	int var2;
	float var3;
	int var5;
	int var6;
	for(var1 = 0; var1 < 16; ++var1) {
	for(var2 = 0; var2 < 16; ++var2) {
	var3 = 0.0F;
	for(int var4 = var2 - 2; var4 <= var2; ++var4) {
	var5 = var1 & 15;
	var6 = var4 & 15;
	var3 += this->field[var5 + var6 * 16];
	}
	this->tempField[var1 + var2 * 16] = var3 / 3.2F + this->heightField[var1 + var2 * 16] * 0.8F;
	}
	}
	for(var1 = 0; var1 < 16; ++var1) {
	for(var2 = 0; var2 < 16; ++var2) {
	this->heightField[var1 + var2 * 16] += this->soundField[var1 + var2 * 16] * 0.05F;
	if(this->heightField[var1 + var2 * 16] < 0.0F) {
	this->heightField[var1 + var2 * 16] = 0.0F;
	}
	this->soundField[var1 + var2 * 16] -= 0.3F;
	if(javaMathRandom() < 0.2) {
	this->soundField[var1 + var2 * 16] = 0.5F;
	}
	}
	}
	JArray<float > var12 = this->tempField;
	this->tempField = this->field;
	this->field = var12;
	for(var2 = 0; var2 < 256; ++var2) {
	var3 = this->field[var2 - this->tickCounter * 16 & 255];
	if(var3 > 1.0F) {
	var3 = 1.0F;
	}
	if(var3 < 0.0F) {
	var3 = 0.0F;
	}
	float var13 = var3 * var3;
	var5 = (int)(32.0F + var13 * 32.0F);
	var6 = (int)(50.0F + var13 * 64.0F);
	int var7 = 255;
	int var8 = (int)(146.0F + var13 * 50.0F);
	if(this->anaglyphEnabled) {
	int var9 = (var5 * 30 + var6 * 59 + var7 * 11) / 100;
	int var10 = (var5 * 30 + var6 * 70) / 100;
	int var11 = (var5 * 30 + var7 * 70) / 100;
	var5 = var9;
	var6 = var10;
	var7 = var11;
	}
	this->imageData[var2 * 4 + 0] = (unsigned char)var5;
	this->imageData[var2 * 4 + 1] = (unsigned char)var6;
	this->imageData[var2 * 4 + 2] = (unsigned char)var7;
	this->imageData[var2 * 4 + 3] = (unsigned char)var8;
	}
}

void TexureWaterFlowFX::initStatics()
{
}
