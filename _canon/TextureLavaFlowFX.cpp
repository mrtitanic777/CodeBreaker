#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TextureLavaFlowFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureLavaFlowFX.h"
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
#include "TextureFX.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

TextureLavaFlowFX::TextureLavaFlowFX()
	: TextureFX(Block::s_lavaStill->blockIndexInTexture + 1)
	, tickCounter(0)
{
	this->intensity = JArray<float >(256);
	this->intensityTemp = JArray<float >(256);
	this->heat = JArray<float >(256);
	this->heatVelocity = JArray<float >(256);
	this->tickCounter = 0;
	this->tileSize = 2;
}

TextureLavaFlowFX::~TextureLavaFlowFX()
{
}

void TextureLavaFlowFX::onTick()
{
	++this->tickCounter;
	int var2;
	float var3;
	int var5;
	int var6;
	int var7;
	int var8;
	int var9;
	for(int var1 = 0; var1 < 16; ++var1) {
	for(var2 = 0; var2 < 16; ++var2) {
	var3 = 0.0F;
	int var4 = (int)(MathHelper::sin((float)var2 * (float)3.1415927F * 2.0F / 16.0F) * 1.2F);
	var5 = (int)(MathHelper::sin((float)var1 * (float)3.1415927F * 2.0F / 16.0F) * 1.2F);
	for(var6 = var1 - 1; var6 <= var1 + 1; ++var6) {
	for(var7 = var2 - 1; var7 <= var2 + 1; ++var7) {
	var8 = (var6 + var4) & 15;
	var9 = (var7 + var5) & 15;
	var3 += this->intensity[var8 + var9 * 16];
	}
	}
	this->intensityTemp[var1 + var2 * 16] = var3 / 10.0F + (this->heat[(var1 + 0 & 15) + (var2 + 0 & 15) * 16] + this->heat[(var1 + 1 & 15) + (var2 + 0 & 15) * 16] + this->heat[(var1 + 1 & 15) + (var2 + 1 & 15) * 16] + this->heat[(var1 + 0 & 15) + (var2 + 1 & 15) * 16]) / 4.0F * 0.8F;
	this->heat[var1 + var2 * 16] += this->heatVelocity[var1 + var2 * 16] * 0.01F;
	if(this->heat[var1 + var2 * 16] < 0.0F) {
	this->heat[var1 + var2 * 16] = 0.0F;
	}
	this->heatVelocity[var1 + var2 * 16] -= 0.06F;
	if(javaMathRandom() < 0.005) {
	this->heatVelocity[var1 + var2 * 16] = 1.5F;
	}
	}
	}
	JArray<float > var11 = this->intensityTemp;
	this->intensityTemp = this->intensity;
	this->intensity = var11;
	for(var2 = 0; var2 < 256; ++var2) {
	var3 = this->intensity[var2 - this->tickCounter / 3 * 16 & 255] * 2.0F;
	if(var3 > 1.0F) {
	var3 = 1.0F;
	}
	if(var3 < 0.0F) {
	var3 = 0.0F;
	}
	var5 = (int)(var3 * 100.0F + 155.0F);
	var6 = (int)(var3 * var3 * 255.0F);
	var7 = (int)(var3 * var3 * var3 * var3 * 128.0F);
	if(this->anaglyphEnabled) {
	var8 = (var5 * 30 + var6 * 59 + var7 * 11) / 100;
	var9 = (var5 * 30 + var6 * 70) / 100;
	int var10 = (var5 * 30 + var7 * 70) / 100;
	var5 = var8;
	var6 = var9;
	var7 = var10;
	}
	this->imageData[var2 * 4 + 0] = (unsigned char)var5;
	this->imageData[var2 * 4 + 1] = (unsigned char)var6;
	this->imageData[var2 * 4 + 2] = (unsigned char)var7;
	this->imageData[var2 * 4 + 3] = -1;
	}
}

void TextureLavaFlowFX::initStatics()
{
}
