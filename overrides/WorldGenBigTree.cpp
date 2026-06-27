#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldGenBigTree.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenBigTree.h"
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
#include "WorldGenerator.h"
#include "WorldProvider.h"

static unsigned char _WorldGenBigTree_s_axisCoordPairs_data[] = { (unsigned char)2, (unsigned char)0, (unsigned char)0, (unsigned char)1, (unsigned char)2, (unsigned char)1 };
JArray<unsigned char > WorldGenBigTree::s_axisCoordPairs(_WorldGenBigTree_s_axisCoordPairs_data, 6);

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  Java allocated rectangular 2D int arrays directly, e.g. in generateLeafNodeList():
 *      int[][] var2 = new int[var1 * this.heightLimit][4];
 *      ...
 *      this.leafNodes = new int[var4][4];
 *  No native int[][] in the runtime model, so this helper reproduces Java's rectangular
 *  int[rows][cols] semantics (each row separately allocated and zero-filled).
 *  ---- end original Java ---- */
// PORT: Java `new int[rows][cols]` 2D array -> a JArray of row JArrays (each separately
// allocated and zero-filled, matching Java's rectangular int[][] semantics).
static JArray<JArray<int > > _newInt2D(int rows, int cols) {
	JArray<JArray<int > > m(rows);
	for (int i = 0; i < rows; ++i) m[i] = JArray<int>(cols);
	return m;
}

WorldGenBigTree::~WorldGenBigTree()
{
}

void WorldGenBigTree::generateLeafNodeList()
{
	this->trunkHeight = (int)((double)this->heightLimit * this->heightAttenuation);
	if(this->trunkHeight >= this->heightLimit) {
	this->trunkHeight = this->heightLimit - 1;
	}
	int var1 = (int)(1.382 + powf(this->leafDensity * (double)this->heightLimit / 13.0, 2.0));
	if(var1 < 1) {
	var1 = 1;
	}
	JArray<JArray<int > > var2 = _newInt2D(var1 * this->heightLimit, 4);
	int var3 = this->basePos[1] + this->heightLimit - this->leafDistanceLimit;
	int var4 = 1;
	int var5 = this->basePos[1] + this->trunkHeight;
	int var6 = var3 - this->basePos[1];
	var2[0][0] = this->basePos[0];
	var2[0][1] = var3;
	var2[0][2] = this->basePos[2];
	var2[0][3] = var5;
	--var3;
	while(true) {
	while(var6 >= 0) {
	int var7 = 0;
	float var8 = this->leafShapeRadius(var6);
	if(var8 < 0.0F) {
	--var3;
	--var6;
	} else {
	for(double var9 = 0.5; var7 < var1; ++var7) {
	double var11 = this->scaleWidth * (double)var8 * ((double)this->rand->nextFloat() + 0.328);
	double var13 = (double)this->rand->nextFloat() * 2.0 * 3.14159;
	int var15 = (int)(var11 * sinf(var13) + (double)this->basePos[0] + var9);
	int var16 = (int)(var11 * cosf(var13) + (double)this->basePos[2] + var9);
	JArray<int > var17 = (JArrayInit<int >(), var15, var3, var16);
	JArray<int > var18 = (JArrayInit<int >(), var15, var3 + this->leafDistanceLimit, var16);
	if(this->checkBlockLine(var17, var18) == -1) {
	JArray<int > var19 = (JArrayInit<int >(), this->basePos[0], this->basePos[1], this->basePos[2]);
	double var20 = sqrtf(powf((double)abs(this->basePos[0] - var17[0]), 2.0) + powf((double)abs(this->basePos[2] - var17[2]), 2.0));
	double var22 = var20 * this->branchSlope;
	if((double)var17[1] - var22 > (double)var5) {
	var19[1] = var5;
	} else {
	var19[1] = (int)((double)var17[1] - var22);
	}
	if(this->checkBlockLine(var19, var17) == -1) {
	var2[var4][0] = var15;
	var2[var4][1] = var3;
	var2[var4][2] = var16;
	var2[var4][3] = var19[1];
	++var4;
	}
	}
	}
	--var3;
	--var6;
	}
	}
	this->leafNodes = _newInt2D(var4, 4);
	System::arraycopy(var2, 0, this->leafNodes, 0, var4);
	return;
	}
}

void WorldGenBigTree::placeLeafCrossSection(int var1, int var2, int var3, float var4, unsigned char var5, int var6)
{
	int var7 = (int)((double)var4 + 0.618);
	unsigned char var8 = s_axisCoordPairs[var5];
	unsigned char var9 = s_axisCoordPairs[var5 + 3];
	JArray<int > var10 = (JArrayInit<int >(), var1, var2, var3);
	JArray<int > var11 = (JArrayInit<int >(), 0, 0, 0);
	int var12 = -var7;
	int var13 = -var7;
	for(var11[var5] = var10[var5]; var12 <= var7; ++var12) {
	var11[var8] = var10[var8] + var12;
	var13 = -var7;
	while(true) {
	while(true) {
	if(var13 > var7) {
	goto label32_cont;
	}
	double var15 = sqrtf(powf((double)abs(var12) + 0.5, 2.0) + powf((double)abs(var13) + 0.5, 2.0));
	if(var15 > (double)var4) {
	++var13;
	} else {
	var11[var9] = var10[var9] + var13;
	int var14 = this->worldObj->getBlockId(var11[0], var11[1], var11[2]);
	if(var14 != 0 && var14 != 18) {
	++var13;
	} else {
	this->worldObj->setBlock(var11[0], var11[1], var11[2], var6);
	++var13;
	}
	}
	}
	}
	label32_cont:;
	}
	label32_brk:;
}

float WorldGenBigTree::leafShapeRadius(int var1)
{
	if((double)var1 < (double)((float)this->heightLimit) * 0.3) {
	return -1.618F;
	} else {
	float var2 = (float)this->heightLimit / 2.0F;
	float var3 = (float)this->heightLimit / 2.0F - (float)var1;
	float var4;
	if(var3 == 0.0F) {
	var4 = var2;
	} else if(abs(var3) >= var2) {
	var4 = 0.0F;
	} else {
	var4 = (float)sqrtf(powf((double)abs(var2), 2.0) - powf((double)abs(var3), 2.0));
	}
	var4 *= 0.5F;
	return var4;
	}
}

float WorldGenBigTree::layerLeafSize(int var1)
{
	return var1 >= 0 && var1 < this->leafDistanceLimit ? (var1 != 0 && var1 != this->leafDistanceLimit - 1 ? 3.0F : 2.0F) : -1.0F;
}

void WorldGenBigTree::generateLeafNode(int var1, int var2, int var3)
{
	int var4 = var2;
	for(int var5 = var2 + this->leafDistanceLimit; var4 < var5; ++var4) {
	float var6 = this->layerLeafSize(var4 - var2);
	this->placeLeafCrossSection(var1, var4, var3, var6, (unsigned char)1, 18);
	}
}

void WorldGenBigTree::placeBlockLine(JArray<int > var1, JArray<int > var2, int var3)
{
	JArray<int > var4 = (JArrayInit<int >(), 0, 0, 0);
	unsigned char var5 = 0;
	unsigned char var6;
	for(var6 = 0; var5 < 3; ++var5) {
	var4[var5] = var2[var5] - var1[var5];
	if(abs(var4[var5]) > abs(var4[var6])) {
	var6 = var5;
	}
	}
	if(var4[var6] != 0) {
	unsigned char var7 = s_axisCoordPairs[var6];
	unsigned char var8 = s_axisCoordPairs[var6 + 3];
	unsigned char var9;
	if(var4[var6] > 0) {
	var9 = 1;
	} else {
	var9 = -1;
	}
	double var10 = (double)var4[var7] / (double)var4[var6];
	double var12 = (double)var4[var8] / (double)var4[var6];
	JArray<int > var14 = (JArrayInit<int >(), 0, 0, 0);
	int var15 = 0;
	for(int var16 = var4[var6] + var9; var15 != var16; var15 += var9) {
	var14[var6] = MathHelper::floor_double((double)(var1[var6] + var15) + 0.5);
	var14[var7] = MathHelper::floor_double((double)var1[var7] + (double)var15 * var10 + 0.5);
	var14[var8] = MathHelper::floor_double((double)var1[var8] + (double)var15 * var12 + 0.5);
	this->worldObj->setBlock(var14[0], var14[1], var14[2], var3);
	}
	}
}

void WorldGenBigTree::generateLeaves()
{
	int var1 = 0;
	for(int var2 = this->leafNodes.length; var1 < var2; ++var1) {
	int var3 = this->leafNodes[var1][0];
	int var4 = this->leafNodes[var1][1];
	int var5 = this->leafNodes[var1][2];
	this->generateLeafNode(var3, var4, var5);
	}
}

bool WorldGenBigTree::leafNodeNeedsBase(int var1)
{
	return (double)var1 >= (double)this->heightLimit * 0.2;
}

void WorldGenBigTree::generateTrunk()
{
	int var1 = this->basePos[0];
	int var2 = this->basePos[1];
	int var3 = this->basePos[1] + this->trunkHeight;
	int var4 = this->basePos[2];
	JArray<int > var5 = (JArrayInit<int >(), var1, var2, var4);
	JArray<int > var6 = (JArrayInit<int >(), var1, var3, var4);
	this->placeBlockLine(var5, var6, 17);
	if(this->trunkSize == 2) {
	++var5[0];
	++var6[0];
	this->placeBlockLine(var5, var6, 17);
	++var5[2];
	++var6[2];
	this->placeBlockLine(var5, var6, 17);
	var5[0] += -1;
	var6[0] += -1;
	this->placeBlockLine(var5, var6, 17);
	}
}

void WorldGenBigTree::generateLeafNodeBases()
{
	int var1 = 0;
	int var2 = this->leafNodes.length;
	for(JArray<int > var3 = (JArrayInit<int >(), this->basePos[0], this->basePos[1], this->basePos[2]); var1 < var2; ++var1) {
	JArray<int > var4 = this->leafNodes[var1];
	JArray<int > var5 = (JArrayInit<int >(), var4[0], var4[1], var4[2]);
	var3[1] = var4[3];
	int var6 = var3[1] - this->basePos[1];
	if(this->leafNodeNeedsBase(var6)) {
	this->placeBlockLine(var3, var5, 17);
	}
	}
}

int WorldGenBigTree::checkBlockLine(JArray<int > var1, JArray<int > var2)
{
	JArray<int > var3 = (JArrayInit<int >(), 0, 0, 0);
	unsigned char var4 = 0;
	unsigned char var5;
	for(var5 = 0; var4 < 3; ++var4) {
	var3[var4] = var2[var4] - var1[var4];
	if(abs(var3[var4]) > abs(var3[var5])) {
	var5 = var4;
	}
	}
	if(var3[var5] == 0) {
	return -1;
	} else {
	unsigned char var6 = s_axisCoordPairs[var5];
	unsigned char var7 = s_axisCoordPairs[var5 + 3];
	unsigned char var8;
	if(var3[var5] > 0) {
	var8 = 1;
	} else {
	var8 = -1;
	}
	double var9 = (double)var3[var6] / (double)var3[var5];
	double var11 = (double)var3[var7] / (double)var3[var5];
	JArray<int > var13 = (JArrayInit<int >(), 0, 0, 0);
	int var14 = 0;
	int var15;
	for(var15 = var3[var5] + var8; var14 != var15; var14 += var8) {
	var13[var5] = var1[var5] + var14;
	var13[var6] = (int)((double)var1[var6] + (double)var14 * var9);
	var13[var7] = (int)((double)var1[var7] + (double)var14 * var11);
	int var16 = this->worldObj->getBlockId(var13[0], var13[1], var13[2]);
	if(var16 != 0 && var16 != 18) {
	break;
	}
	}
	return var14 == var15 ? -1 : abs(var14);
	}
}

bool WorldGenBigTree::validateTreeLocation()
{
	JArray<int > var1 = (JArrayInit<int >(), this->basePos[0], this->basePos[1], this->basePos[2]);
	JArray<int > var2 = (JArrayInit<int >(), this->basePos[0], this->basePos[1] + this->heightLimit - 1, this->basePos[2]);
	int var3 = this->worldObj->getBlockId(this->basePos[0], this->basePos[1] - 1, this->basePos[2]);
	if(var3 != 2 && var3 != 3) {
	return false;
	} else {
	int var4 = this->checkBlockLine(var1, var2);
	if(var4 == -1) {
	return true;
	} else if(var4 < 6) {
	return false;
	} else {
	this->heightLimit = var4;
	return true;
	}
	}
}

void WorldGenBigTree::setScale(double var1, double var3, double var5)
{
	this->heightLimitLimit = (int)(var1 * 12.0);
	if(var1 > 0.5) {
	this->leafDistanceLimit = 5;
	}
	this->scaleWidth = var3;
	this->leafDensity = var5;
}

bool WorldGenBigTree::generate(World* var1, Random* var2, int var3, int var4, int var5)
{
	this->worldObj = var1;
	long long var6 = var2->nextLong();
	this->rand->setSeed(var6);
	this->basePos[0] = var3;
	this->basePos[1] = var4;
	this->basePos[2] = var5;
	if(this->heightLimit == 0) {
	this->heightLimit = 5 + this->rand->nextInt(this->heightLimitLimit);
	}
	if(!this->validateTreeLocation()) {
	return false;
	} else {
	this->generateLeafNodeList();
	this->generateLeaves();
	this->generateTrunk();
	this->generateLeafNodeBases();
	return true;
	}
}
