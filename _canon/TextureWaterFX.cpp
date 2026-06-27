#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TextureWaterFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureWaterFX.h"
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

TextureWaterFX::TextureWaterFX()
	: TextureFX(Block::s_waterStill->blockIndexInTexture)
	, tickCounter(0)
{
	this->red = JArray<float >(256);
	this->green2 = JArray<float >(256);
	this->blue = JArray<float >(256);
	this->green = JArray<float >(256);
	this->tickCounter = 0;
}

TextureWaterFX::~TextureWaterFX()
{
}

void TextureWaterFX::onTick()
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
	for(int var4 = var1 - 1; var4 <= var1 + 1; ++var4) {
	var5 = var4 & 15;
	var6 = var2 & 15;
	var3 += this->red[var5 + var6 * 16];
	}
	this->green2[var1 + var2 * 16] = var3 / 3.3F + this->blue[var1 + var2 * 16] * 0.8F;
	}
	}
	for(var1 = 0; var1 < 16; ++var1) {
	for(var2 = 0; var2 < 16; ++var2) {
	this->blue[var1 + var2 * 16] += this->green[var1 + var2 * 16] * 0.05F;
	if(this->blue[var1 + var2 * 16] < 0.0F) {
	this->blue[var1 + var2 * 16] = 0.0F;
	}
	this->green[var1 + var2 * 16] -= 0.1F;
	if(javaMathRandom() < 0.05) {
	this->green[var1 + var2 * 16] = 0.5F;
	}
	}
	}
	JArray<float > var12 = this->green2;
	this->green2 = this->red;
	this->red = var12;
	for(var2 = 0; var2 < 256; ++var2) {
	var3 = this->red[var2];
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

void TextureWaterFX::initStatics()
{
}
