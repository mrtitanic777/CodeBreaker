#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TerrainTextureManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TerrainTextureManager.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BufferedImage.h"
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
#include "IOException.h"
#include "ImageIO.h"
#include "InventoryPlayer.h"
#include "IsoImageBuffer.h"
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
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

TerrainTextureManager::TerrainTextureManager()
{
	this->blockColors = JArray<float >(768);
	this->imagePixels = JArray<int >(5120);
	this->blockDepth = JArray<int >(5120);
	this->waterDepth = JArray<int >(5120);
	this->waterBrightness = JArray<int >(5120);
	this->columnHeights = JArray<int >(34);
	this->blockSideTextures = JArray<int >(768);
	try {
	BufferedImage* var1 = ImageIO::read(Class(typeid(TerrainTextureManager)).getResource("/terrain.png"));
	JArray<int > var2 = JArray<int >(65536);
	var1->getRGB(0, 0, 256, 256, var2, 0, 256);
	for(int var3 = 0; var3 < 256; ++var3) {
	int var4 = 0;
	int var5 = 0;
	int var6 = 0;
	int var7 = var3 % 16 * 16;
	int var8 = var3 / 16 * 16;
	int var9 = 0;
	for(int var10 = 0; var10 < 16; ++var10) {
	for(int var11 = 0; var11 < 16; ++var11) {
	int var12 = var2[var11 + var7 + (var10 + var8) * 256];
	int var13 = (var12 >> 24) & 255;
	if(var13 > 128) {
	var4 += var12 >> 16 & 255;
	var5 += var12 >> 8 & 255;
	var6 += var12 & 255;
	++var9;
	}
	}
	if(var9 == 0) {
	++var9;
	}
	this->blockColors[var3 * 3 + 0] = (float)(var4 / var9);
	this->blockColors[var3 * 3 + 1] = (float)(var5 / var9);
	this->blockColors[var3 * 3 + 2] = (float)(var6 / var9);
	}
	}
	} catch (IOException* var14) {
	var14->printStackTrace();
	}
	for(int var15 = 0; var15 < 256; ++var15) {
	if(Block::s_blocksList[var15] != NULL) {
	this->blockSideTextures[var15 * 3 + 0] = Block::s_blocksList[var15]->getBlockTextureFromSide(1);
	this->blockSideTextures[var15 * 3 + 1] = Block::s_blocksList[var15]->getBlockTextureFromSide(2);
	this->blockSideTextures[var15 * 3 + 2] = Block::s_blocksList[var15]->getBlockTextureFromSide(3);
	}
	}
}

TerrainTextureManager::~TerrainTextureManager()
{
}

void TerrainTextureManager::renderIsoTile(IsoImageBuffer* var1)
{
	World* var2 = var1->worldObj;
	if(var2 == NULL) {
	var1->isUpdating = true;
	var1->isDirty = true;
	} else {
	int var3 = var1->anotherInt * 16;
	int var4 = var1->anInt * 16;
	int var5 = var3 + 16;
	int var6 = var4 + 16;
	Chunk* var7 = var2->getChunkFromChunkCoords(var1->anotherInt, var1->anInt);
	if(var7->isEmptyChunk) {
	var1->isUpdating = true;
	var1->isDirty = true;
	} else {
	var1->isUpdating = false;
	Arrays::fill(this->blockDepth, 0);
	Arrays::fill(this->waterDepth, 0);
	Arrays::fill(this->columnHeights, 160);
	for(int var8 = var6 - 1; var8 >= var4; --var8) {
	for(int var9 = var5 - 1; var9 >= var3; --var9) {
	int var10 = var9 - var3;
	int var11 = var8 - var4;
	int var12 = var10 + var11;
	bool var13 = true;
	for(int var14 = 0; var14 < 128; ++var14) {
	int var15 = var11 - var10 - var14 + 160 - 16;
	if(var15 < this->columnHeights[var12] || var15 < this->columnHeights[var12 + 1]) {
	Block* var16 = Block::s_blocksList[var2->getBlockId(var9, var14, var8)];
	if(var16 == NULL) {
	var13 = false;
	} else if(var16->blockMaterial == Material::s_water) {
	int var24 = var2->getBlockId(var9, var14 + 1, var8);
	if(var24 == 0 || Block::s_blocksList[var24]->blockMaterial != Material::s_water) {
	float var25 = (float)var14 / 127.0F * 0.6F + 0.4F;
	float var26 = var2->getLightBrightness(var9, var14 + 1, var8) * var25;
	if(var15 >= 0 && var15 < 160) {
	int var27 = var12 + var15 * 32;
	if(var12 >= 0 && var12 <= 32 && this->waterDepth[var27] <= var14) {
	this->waterDepth[var27] = var14;
	this->waterBrightness[var27] = (int)(var26 * 127.0F);
	}
	if(var12 >= -1 && var12 <= 31 && this->waterDepth[var27 + 1] <= var14) {
	this->waterDepth[var27 + 1] = var14;
	this->waterBrightness[var27 + 1] = (int)(var26 * 127.0F);
	}
	var13 = false;
	}
	}
	} else {
	if(var13) {
	if(var15 < this->columnHeights[var12]) {
	this->columnHeights[var12] = var15;
	}
	if(var15 < this->columnHeights[var12 + 1]) {
	this->columnHeights[var12 + 1] = var15;
	}
	}
	float var17 = (float)var14 / 127.0F * 0.6F + 0.4F;
	int var18;
	int var19;
	float var20;
	float var22;
	if(var15 >= 0 && var15 < 160) {
	var18 = var12 + var15 * 32;
	var19 = this->blockSideTextures[var16->blockID * 3 + 0];
	var20 = (var2->getLightBrightness(var9, var14 + 1, var8) * 0.8F + 0.2F) * var17;
	if(var12 >= 0 && this->blockDepth[var18] <= var14) {
	this->blockDepth[var18] = var14;
	this->imagePixels[var18] = -16777216 | ((int)(this->blockColors[var19 * 3 + 0] * var20) << 16) | ((int)(this->blockColors[var19 * 3 + 1] * var20) << 8) | (int)(this->blockColors[var19 * 3 + 2] * var20);
	}
	if(var12 < 31) {
	var22 = var20 * 0.9F;
	if(this->blockDepth[var18 + 1] <= var14) {
	this->blockDepth[var18 + 1] = var14;
	this->imagePixels[var18 + 1] = -16777216 | ((int)(this->blockColors[var19 * 3 + 0] * var22) << 16) | ((int)(this->blockColors[var19 * 3 + 1] * var22) << 8) | (int)(this->blockColors[var19 * 3 + 2] * var22);
	}
	}
	}
	if(var15 >= -1 && var15 < 159) {
	var18 = var12 + (var15 + 1) * 32;
	var19 = this->blockSideTextures[var16->blockID * 3 + 1];
	var20 = var2->getLightBrightness(var9 - 1, var14, var8) * 0.8F + 0.2F;
	int var21 = this->blockSideTextures[var16->blockID * 3 + 2];
	var22 = var2->getLightBrightness(var9, var14, var8 + 1) * 0.8F + 0.2F;
	float var23;
	if(var12 >= 0) {
	var23 = var20 * var17 * 0.6F;
	if(this->blockDepth[var18] <= var14 - 1) {
	this->blockDepth[var18] = var14 - 1;
	this->imagePixels[var18] = -16777216 | ((int)(this->blockColors[var19 * 3 + 0] * var23) << 16) | ((int)(this->blockColors[var19 * 3 + 1] * var23) << 8) | (int)(this->blockColors[var19 * 3 + 2] * var23);
	}
	}
	if(var12 < 31) {
	var23 = var22 * 0.9F * var17 * 0.4F;
	if(this->blockDepth[var18 + 1] <= var14 - 1) {
	this->blockDepth[var18 + 1] = var14 - 1;
	this->imagePixels[var18 + 1] = -16777216 | ((int)(this->blockColors[var21 * 3 + 0] * var23) << 16) | ((int)(this->blockColors[var21 * 3 + 1] * var23) << 8) | (int)(this->blockColors[var21 * 3 + 2] * var23);
	}
	}
	}
	}
	}
	}
	}
	}
	this->blendWaterOverlay();
	if(var1->image == NULL) {
	var1->image = new BufferedImage(32, 160, 2);
	}
	var1->image->setRGB(0, 0, 32, 160, this->imagePixels, 0, 32);
	var1->isDirty = true;
	}
	}
}

void TerrainTextureManager::blendWaterOverlay()
{
	for(int var1 = 0; var1 < 32; ++var1) {
	for(int var2 = 0; var2 < 160; ++var2) {
	int var3 = var1 + var2 * 32;
	if(this->blockDepth[var3] == 0) {
	this->imagePixels[var3] = 0;
	}
	if(this->waterDepth[var3] > this->blockDepth[var3]) {
	int var4 = (this->imagePixels[var3] >> 24) & 255;
	this->imagePixels[var3] = ((this->imagePixels[var3] & 16711422) >> 1) + this->waterBrightness[var3];
	if(var4 < 128) {
	this->imagePixels[var3] = Integer::MIN_VALUE + this->waterBrightness[var3] * 2;
	} else {
	this->imagePixels[var3] |= -16777216;
	}
	}
	}
	}
}

void TerrainTextureManager::initStatics()
{
}
