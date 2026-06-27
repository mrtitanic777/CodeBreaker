#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TextureFlamesFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFlamesFX.h"
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

TextureFlamesFX::TextureFlamesFX(int var1)
	: TextureFX(Block::s_fire->blockIndexInTexture + var1 * 16)
{
	this->flameBufferA = JArray<float >(320);
	this->flameBufferB = JArray<float >(320);
}

TextureFlamesFX::~TextureFlamesFX()
{
}

void TextureFlamesFX::onTick()
{
	int var2;
	float var4;
	int var5;
	int var6;
	for(int var1 = 0; var1 < 16; ++var1) {
	for(var2 = 0; var2 < 20; ++var2) {
	int var3 = 18;
	var4 = this->flameBufferA[var1 + (var2 + 1) % 20 * 16] * (float)var3;
	for(var5 = var1 - 1; var5 <= var1 + 1; ++var5) {
	for(var6 = var2; var6 <= var2 + 1; ++var6) {
	if(var5 >= 0 && var6 >= 0 && var5 < 16 && var6 < 20) {
	var4 += this->flameBufferA[var5 + var6 * 16];
	}
	++var3;
	}
	}
	this->flameBufferB[var1 + var2 * 16] = var4 / ((float)var3 * 1.06F);
	if(var2 >= 19) {
	this->flameBufferB[var1 + var2 * 16] = (float)(javaMathRandom() * javaMathRandom() * javaMathRandom() * 4.0 + javaMathRandom() * (double)0.1F + (double)0.2F);
	}
	}
	}
	JArray<float > var12 = this->flameBufferB;
	this->flameBufferB = this->flameBufferA;
	this->flameBufferA = var12;
	for(var2 = 0; var2 < 256; ++var2) {
	float var13 = this->flameBufferA[var2] * 1.8F;
	if(var13 > 1.0F) {
	var13 = 1.0F;
	}
	if(var13 < 0.0F) {
	var13 = 0.0F;
	}
	var5 = (int)(var13 * 155.0F + 100.0F);
	var6 = (int)(var13 * var13 * 255.0F);
	int var7 = (int)(var13 * var13 * var13 * var13 * var13 * var13 * var13 * var13 * var13 * var13 * 255.0F);
	short var8 = 255;
	if(var13 < 0.5F) {
	var8 = 0;
	}
	var4 = (var13 - 0.5F) * 2.0F;
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

void TextureFlamesFX::initStatics()
{
}
