#include "stdafx.h"
#include "JavaRT.h"
#include "Common.h"   // DIAG: DbgPrintF for the [GEN] walk-freeze probe
#include "SpawnerAnimals.h"   // CONSOLE: world-gen animal herds (Alpha 1.2.6 lacks them)

// Auto-generated from ChunkProviderGenerate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkProviderGenerate.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BlockSand.h"
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
#include "IProgressUpdate.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "MapGenBase.h"
#include "MapGenCaves.h"
#include "Material.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves.h"
#include "NoiseGeneratorOctaves2.h"
#include "NoiseGeneratorPerlin.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldGenBigTree.h"
#include "WorldGenCactus.h"
#include "WorldGenClay.h"
#include "WorldGenDungeons.h"
#include "WorldGenFlowers.h"
#include "WorldGenLakes.h"
#include "WorldGenLiquids.h"
#include "WorldGenMinable.h"
#include "WorldGenPumpkin.h"
#include "WorldGenReed.h"
#include "WorldGenTrees.h"
#include "WorldGenerator.h"
#include "WorldProvider.h"

ChunkProviderGenerate::ChunkProviderGenerate(World* var1, long long var2)
	: rand(NULL)
	, noiseGen1(NULL)
	, noiseGen2(NULL)
	, noiseGen3(NULL)
	, noiseGen4(NULL)
	, noiseGen5(NULL)
	, noiseGen6(NULL)
	, noiseGen7(NULL)
	, mobSpawnerNoise(NULL)
	, worldObj(NULL)
	, caveGenerator(NULL)
	, genWCM(NULL)            // PORT(★ parallel pre-gen): default = use worldObj's shared WCM (single-thread)
	, genDeferSpawn(false)    // PORT(★ parallel pre-gen): default = spawn mobs inline (single-thread)
{
	this->sandNoise = JArray<double >(256);
	this->gravelNoise = JArray<double >(256);
	this->stoneNoise = JArray<double >(256);
	this->caveGenerator = new MapGenCaves();
	this->field_914_i = j2x_newarr2<int >(32, 32);
	this->worldObj = var1;
	this->rand = new Random(var2);
	this->noiseGen1 = new NoiseGeneratorOctaves(this->rand, 16);
	this->noiseGen2 = new NoiseGeneratorOctaves(this->rand, 16);
	this->noiseGen3 = new NoiseGeneratorOctaves(this->rand, 8);
	this->noiseGen4 = new NoiseGeneratorOctaves(this->rand, 4);
	this->noiseGen5 = new NoiseGeneratorOctaves(this->rand, 4);
	this->noiseGen6 = new NoiseGeneratorOctaves(this->rand, 10);
	this->noiseGen7 = new NoiseGeneratorOctaves(this->rand, 16);
	this->mobSpawnerNoise = new NoiseGeneratorOctaves(this->rand, 8);
}

ChunkProviderGenerate::~ChunkProviderGenerate()
{
	delete mobSpawnerNoise;
	mobSpawnerNoise = NULL;
	delete noiseGen1;
	noiseGen1 = NULL;
	delete noiseGen2;
	noiseGen2 = NULL;
	delete noiseGen3;
	noiseGen3 = NULL;
	delete noiseGen4;
	noiseGen4 = NULL;
	delete noiseGen5;
	noiseGen5 = NULL;
	delete noiseGen6;
	noiseGen6 = NULL;
	delete noiseGen7;
	noiseGen7 = NULL;
	delete rand;
	rand = NULL;
}

void ChunkProviderGenerate::generateTerrain(int var1, int var2, JArray<unsigned char > var3, JArray<MobSpawnerBase* > var4, JArray<double > var5)
{
	unsigned char var6 = 4;
	unsigned char var7 = 64;
	int var8 = var6 + 1;
	unsigned char var9 = 17;
	int var10 = var6 + 1;
	this->noiseArray = this->initializeNoiseField(this->noiseArray, var1 * var6, 0, var2 * var6, var8, var9, var10);
	for(int var11 = 0; var11 < var6; ++var11) {
	for(int var12 = 0; var12 < var6; ++var12) {
	for(int var13 = 0; var13 < 16; ++var13) {
	double var14 = 0.125;
	double var16 = this->noiseArray[((var11 + 0) * var10 + var12 + 0) * var9 + var13 + 0];
	double var18 = this->noiseArray[((var11 + 0) * var10 + var12 + 1) * var9 + var13 + 0];
	double var20 = this->noiseArray[((var11 + 1) * var10 + var12 + 0) * var9 + var13 + 0];
	double var22 = this->noiseArray[((var11 + 1) * var10 + var12 + 1) * var9 + var13 + 0];
	double var24 = (this->noiseArray[((var11 + 0) * var10 + var12 + 0) * var9 + var13 + 1] - var16) * var14;
	double var26 = (this->noiseArray[((var11 + 0) * var10 + var12 + 1) * var9 + var13 + 1] - var18) * var14;
	double var28 = (this->noiseArray[((var11 + 1) * var10 + var12 + 0) * var9 + var13 + 1] - var20) * var14;
	double var30 = (this->noiseArray[((var11 + 1) * var10 + var12 + 1) * var9 + var13 + 1] - var22) * var14;
	for(int var32 = 0; var32 < 8; ++var32) {
	double var33 = 0.25;
	double var35 = var16;
	double var37 = var18;
	double var39 = (var20 - var16) * var33;
	double var41 = (var22 - var18) * var33;
	for(int var43 = 0; var43 < 4; ++var43) {
	int var44 = ((var43 + var11 * 4) << 11) | ((0 + var12 * 4) << 7) | (var13 * 8 + var32);
	short var45 = 128;
	double var46 = 0.25;
	double var48 = var35;
	double var50 = (var37 - var35) * var46;
	for(int var52 = 0; var52 < 4; ++var52) {
	double var53 = var5[(var11 * 4 + var43) * 16 + var12 * 4 + var52];
	int var55 = 0;
	if(var13 * 8 + var32 < var7) {
	if(var53 < 0.5 && var13 * 8 + var32 >= var7 - 1) {
	var55 = Block::s_blockIce->blockID;
	} else {
	var55 = Block::s_waterMoving->blockID;
	}
	}
	if(var48 > 0.0) {
	var55 = Block::s_stone->blockID;
	}
	// FINITE WORLD (Xbox 360 Edition style): the map is a hard-capped 864x864 island centered on
	// (0,0) (half-extent 432 blocks = 27 chunks each way). Java generates ~infinite terrain, so we
	// taper the land height down into the sea over the last 48 blocks before the edge, then deep
	// endless ocean beyond -- a smooth coastline (replaceBlocksForBiome lays sand beaches on it).
	{
	int _wx = var1 * 16 + (var43 + var11 * 4);
	int _wz = var2 * 16 + (var12 * 4 + var52);
	int _ax = _wx < 0 ? -_wx : _wx;
	int _az = _wz < 0 ? -_wz : _wz;
	int _edge = _ax > _az ? _ax : _az;            // chebyshev distance -> square world
	if(_edge > 432 - 48) {                         // within the 48-block coastal taper band
	double _t = (double)(_edge - (432 - 48)) / 48.0;
	if(_t > 1.0) _t = 1.0;                          // >= 432: clamp to deep ocean (extends forever)
	int _target = 62 - (int)(_t * 22.0);           // sea floor: 62 (shallow coast) -> 40 (deep)
	int _y = var13 * 8 + var32;
	if(_y > _target) {                             // only LOWER terrain (keeps natural low coastlines)
	var55 = (_y < var7) ? Block::s_waterMoving->blockID : 0;
	}
	}
	}
	var3[var44] = (unsigned char)var55;
	var44 += var45;
	var48 += var50;
	}
	var35 += var39;
	var37 += var41;
	}
	var16 += var24;
	var18 += var26;
	var20 += var28;
	var22 += var30;
	}
	}
	}
	}
}

void ChunkProviderGenerate::replaceBlocksForBiome(int var1, int var2, JArray<unsigned char > var3, JArray<MobSpawnerBase* > var4)
{
	unsigned char var5 = 64;
	double var6 = 1.0 / 32.0;
	this->sandNoise = this->noiseGen4->generateNoiseOctaves(this->sandNoise, (double)(var1 * 16), (double)(var2 * 16), 0.0, 16, 16, 1, var6, var6, 1.0);
	this->gravelNoise = this->noiseGen4->generateNoiseOctaves(this->gravelNoise, (double)(var2 * 16), 109.0134, (double)(var1 * 16), 16, 1, 16, var6, 1.0, var6);
	this->stoneNoise = this->noiseGen5->generateNoiseOctaves(this->stoneNoise, (double)(var1 * 16), (double)(var2 * 16), 0.0, 16, 16, 1, var6 * 2.0, var6 * 2.0, var6 * 2.0);
	for(int var8 = 0; var8 < 16; ++var8) {
	for(int var9 = 0; var9 < 16; ++var9) {
	MobSpawnerBase* var10 = var4[var8 * 16 + var9];
	bool var11 = this->sandNoise[var8 + var9 * 16] + this->rand->nextDouble() * 0.2 > 0.0;
	bool var12 = this->gravelNoise[var8 + var9 * 16] + this->rand->nextDouble() * 0.2 > 3.0;
	int var13 = (int)(this->stoneNoise[var8 + var9 * 16] / 3.0 + 3.0 + this->rand->nextDouble() * 0.25);
	int var14 = -1;
	unsigned char var15 = var10->topBlock;
	unsigned char var16 = var10->fillerBlock;
	for(int var17 = 127; var17 >= 0; --var17) {
	int var18 = (var8 * 16 + var9) * 128 + var17;
	if(var17 <= 0 + this->rand->nextInt(5)) {
	var3[var18] = (unsigned char)Block::s_bedrock->blockID;
	} else {
	unsigned char var19 = var3[var18];
	if(var19 == 0) {
	var14 = -1;
	} else if(var19 == Block::s_stone->blockID) {
	if(var14 == -1) {
	if(var13 <= 0) {
	var15 = 0;
	var16 = (unsigned char)Block::s_stone->blockID;
	} else if(var17 >= var5 - 4 && var17 <= var5 + 1) {
	var15 = var10->topBlock;
	var16 = var10->fillerBlock;
	if(var12) {
	var15 = 0;
	}
	if(var12) {
	var16 = (unsigned char)Block::s_gravel->blockID;
	}
	if(var11) {
	var15 = (unsigned char)Block::s_sand->blockID;
	}
	if(var11) {
	var16 = (unsigned char)Block::s_sand->blockID;
	}
	}
	if(var17 < var5 && var15 == 0) {
	var15 = (unsigned char)Block::s_waterMoving->blockID;
	}
	var14 = var13;
	if(var17 >= var5 - 1) {
	var3[var18] = var15;
	} else {
	var3[var18] = var16;
	}
	} else if(var14 > 0) {
	--var14;
	var3[var18] = var16;
	}
	}
	}
	}
	}
	}
}

Chunk* ChunkProviderGenerate::provideChunk(int var1, int var2)
{
	this->rand->setSeed((long long)var1 * 341873128712LL + (long long)var2 * 132897987541LL);
	JArray<unsigned char > var3 = JArray<unsigned char >(-Short::MIN_VALUE);
	Chunk* var4 = new Chunk(this->worldObj, var3, var1, var2);
	// PORT(★ parallel pre-gen): use the per-worker WCM when set (its temperature/humidity scratch is
	// private to this thread); otherwise the shared world WCM (single-threaded path, unchanged).
	WorldChunkManager* _wcm = this->genWCM ? this->genWCM : this->worldObj->getWorldChunkManager();
	this->biomesForGeneration = _wcm->loadBlockGeneratorData(this->biomesForGeneration, var1 * 16, var2 * 16, 16, 16);
	JArray<double > var5 = _wcm->temperature;
	this->generateTerrain(var1, var2, var3, this->biomesForGeneration, var5);
	this->replaceBlocksForBiome(var1, var2, var3, this->biomesForGeneration);
	this->caveGenerator->generate(this, this->worldObj, var1, var2, var3);
	var4->generateSkylightMap();
	// PORT(★ parallel pre-gen): performWorldGenSpawning writes entities to the world (not thread-safe).
	// On a worker (genDeferSpawn) skip it; the main thread runs it per chunk after install.
	if(!this->genDeferSpawn) {
	SpawnerAnimals::performWorldGenSpawning(this->worldObj, var4, var1, var2);   // CONSOLE: seed passive herds on grass
	}
	return var4;
}

JArray<double > ChunkProviderGenerate::initializeNoiseField(JArray<double > var1, int var2, int var3, int var4, int var5, int var6, int var7)
{
	if(var1 == NULL) {
	var1 = JArray<double >(var5 * var6 * var7);
	}
	double var8 = 684.412;
	double var10 = 684.412;
	JArray<double > var12 = this->worldObj->getWorldChunkManager()->temperature;
	JArray<double > var13 = this->worldObj->getWorldChunkManager()->humidity;
	this->noiseData4 = this->noiseGen6->generateNoiseOctaves2D(this->noiseData4, var2, var4, var5, var7, 1.121, 1.121, 0.5);
	this->noiseData5 = this->noiseGen7->generateNoiseOctaves2D(this->noiseData5, var2, var4, var5, var7, 200.0, 200.0, 0.5);
	this->noiseData1 = this->noiseGen3->generateNoiseOctaves(this->noiseData1, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8 / 80.0, var10 / 160.0, var8 / 80.0);
	this->noiseData2 = this->noiseGen1->generateNoiseOctaves(this->noiseData2, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
	this->noiseData3 = this->noiseGen2->generateNoiseOctaves(this->noiseData3, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
	int var14 = 0;
	int var15 = 0;
	int var16 = 16 / var5;
	for(int var17 = 0; var17 < var5; ++var17) {
	int var18 = var17 * var16 + var16 / 2;
	for(int var19 = 0; var19 < var7; ++var19) {
	int var20 = var19 * var16 + var16 / 2;
	double var21 = var12[var18 * 16 + var20];
	double var23 = var13[var18 * 16 + var20] * var21;
	double var25 = 1.0 - var23;
	var25 *= var25;
	var25 *= var25;
	var25 = 1.0 - var25;
	double var27 = (this->noiseData4[var15] + 256.0) / 512.0;
	var27 *= var25;
	if(var27 > 1.0) {
	var27 = 1.0;
	}
	double var29 = this->noiseData5[var15] / 8000.0;
	if(var29 < 0.0) {
	var29 = -var29 * 0.3;
	}
	var29 = var29 * 3.0 - 2.0;
	if(var29 < 0.0) {
	var29 /= 2.0;
	if(var29 < -1.0) {
	var29 = -1.0;
	}
	var29 /= 1.4;
	var29 /= 2.0;
	var27 = 0.0;
	} else {
	if(var29 > 1.0) {
	var29 = 1.0;
	}
	var29 /= 8.0;
	}
	if(var27 < 0.0) {
	var27 = 0.0;
	}
	var27 += 0.5;
	var29 = var29 * (double)var6 / 16.0;
	double var31 = (double)var6 / 2.0 + var29 * 4.0;
	++var15;
	for(int var33 = 0; var33 < var6; ++var33) {
	double var34 = 0.0;
	double var36 = ((double)var33 - var31) * 12.0 / var27;
	if(var36 < 0.0) {
	var36 *= 4.0;
	}
	double var38 = this->noiseData2[var14] / 512.0;
	double var40 = this->noiseData3[var14] / 512.0;
	double var42 = (this->noiseData1[var14] / 10.0 + 1.0) / 2.0;
	if(var42 < 0.0) {
	var34 = var38;
	} else if(var42 > 1.0) {
	var34 = var40;
	} else {
	var34 = var38 + (var40 - var38) * var42;
	}
	var34 -= var36;
	if(var33 > var6 - 4) {
	double var44 = (double)((float)(var33 - (var6 - 4)) / 3.0F);
	var34 = var34 * (1.0 - var44) + -10.0 * var44;
	}
	var1[var14] = var34;
	++var14;
	}
	}
	}
	return var1;
}

bool ChunkProviderGenerate::chunkExists(int var1, int var2)
{
	return true;
}

void ChunkProviderGenerate::populate(IChunkProvider* var1, int var2, int var3)
{
	BlockSand::s_fallInstantly = true;
	int var4 = var2 * 16;
	int var5 = var3 * 16;
	MobSpawnerBase* var6 = this->worldObj->getWorldChunkManager()->getBiome(var4 + 16, var5 + 16);
	this->rand->setSeed(this->worldObj->randomSeed);
	long long var7 = this->rand->nextLong() / 2LL * 2LL + 1LL;
	long long var9 = this->rand->nextLong() / 2LL * 2LL + 1LL;
	this->rand->setSeed((long long)var2 * var7 + (long long)var3 * var9 ^ this->worldObj->randomSeed);
	double var11 = 0.25;
	int var13;
	int var14;
	int var15;
	if(this->rand->nextInt(4) == 0) {
	var13 = var4 + this->rand->nextInt(16) + 8;
	var14 = this->rand->nextInt(128);
	var15 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenLakes(Block::s_waterMoving->blockID))->generate(this->worldObj, this->rand, var13, var14, var15);
	}
	if(this->rand->nextInt(8) == 0) {
	var13 = var4 + this->rand->nextInt(16) + 8;
	var14 = this->rand->nextInt(this->rand->nextInt(120) + 8);
	var15 = var5 + this->rand->nextInt(16) + 8;
	if(var14 < 64 || this->rand->nextInt(10) == 0) {
	(new WorldGenLakes(Block::s_lavaMoving->blockID))->generate(this->worldObj, this->rand, var13, var14, var15);
	}
	}
	int var16;
	for(var13 = 0; var13 < 8; ++var13) {
	var14 = var4 + this->rand->nextInt(16) + 8;
	var15 = this->rand->nextInt(128);
	var16 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenDungeons())->generate(this->worldObj, this->rand, var14, var15, var16);
	}
	for(var13 = 0; var13 < 10; ++var13) {
	var14 = var4 + this->rand->nextInt(16);
	var15 = this->rand->nextInt(128);
	var16 = var5 + this->rand->nextInt(16);
	(new WorldGenClay(32))->generate(this->worldObj, this->rand, var14, var15, var16);
	}
	for(var13 = 0; var13 < 20; ++var13) {
	var14 = var4 + this->rand->nextInt(16);
	var15 = this->rand->nextInt(128);
	var16 = var5 + this->rand->nextInt(16);
	(new WorldGenMinable(Block::s_dirt->blockID, 32))->generate(this->worldObj, this->rand, var14, var15, var16);
	}
	for(var13 = 0; var13 < 10; ++var13) {
	var14 = var4 + this->rand->nextInt(16);
	var15 = this->rand->nextInt(128);
	var16 = var5 + this->rand->nextInt(16);
	(new WorldGenMinable(Block::s_gravel->blockID, 32))->generate(this->worldObj, this->rand, var14, var15, var16);
	}
	for(var13 = 0; var13 < 20; ++var13) {
	var14 = var4 + this->rand->nextInt(16);
	var15 = this->rand->nextInt(128);
	var16 = var5 + this->rand->nextInt(16);
	(new WorldGenMinable(Block::s_oreCoal->blockID, 16))->generate(this->worldObj, this->rand, var14, var15, var16);
	}
	for(var13 = 0; var13 < 20; ++var13) {
	var14 = var4 + this->rand->nextInt(16);
	var15 = this->rand->nextInt(64);
	var16 = var5 + this->rand->nextInt(16);
	(new WorldGenMinable(Block::s_oreIron->blockID, 8))->generate(this->worldObj, this->rand, var14, var15, var16);
	}
	for(var13 = 0; var13 < 2; ++var13) {
	var14 = var4 + this->rand->nextInt(16);
	var15 = this->rand->nextInt(32);
	var16 = var5 + this->rand->nextInt(16);
	(new WorldGenMinable(Block::s_oreGold->blockID, 8))->generate(this->worldObj, this->rand, var14, var15, var16);
	}
	for(var13 = 0; var13 < 8; ++var13) {
	var14 = var4 + this->rand->nextInt(16);
	var15 = this->rand->nextInt(16);
	var16 = var5 + this->rand->nextInt(16);
	(new WorldGenMinable(Block::s_oreRedstone->blockID, 7))->generate(this->worldObj, this->rand, var14, var15, var16);
	}
	for(var13 = 0; var13 < 1; ++var13) {
	var14 = var4 + this->rand->nextInt(16);
	var15 = this->rand->nextInt(16);
	var16 = var5 + this->rand->nextInt(16);
	(new WorldGenMinable(Block::s_oreDiamond->blockID, 7))->generate(this->worldObj, this->rand, var14, var15, var16);
	}
	var11 = 0.5;
	var13 = (int)((this->mobSpawnerNoise->generateNoise((double)var4 * var11, (double)var5 * var11) / 8.0 + this->rand->nextDouble() * 4.0 + 4.0) / 3.0);
	var14 = 0;
	if(this->rand->nextInt(10) == 0) {
	++var14;
	}
	if(var6 == MobSpawnerBase::s_forest) {
	var14 += var13 + 5;
	}
	if(var6 == MobSpawnerBase::s_rainforest) {
	var14 += var13 + 5;
	}
	if(var6 == MobSpawnerBase::s_seasonalForest) {
	var14 += var13 + 2;
	}
	if(var6 == MobSpawnerBase::s_taiga) {
	var14 += var13 + 5;
	}
	if(var6 == MobSpawnerBase::s_desert) {
	var14 -= 20;
	}
	if(var6 == MobSpawnerBase::s_tundra) {
	var14 -= 20;
	}
	if(var6 == MobSpawnerBase::s_plains) {
	var14 -= 20;
	}
	JObject var24 = new WorldGenTrees();
	if(this->rand->nextInt(10) == 0) {
	var24 = new WorldGenBigTree();
	}
	if(var6 == MobSpawnerBase::s_rainforest && this->rand->nextInt(3) == 0) {
	var24 = new WorldGenBigTree();
	}
	int var17;
	int var18;
	for(var16 = 0; var16 < var14; ++var16) {
	var17 = var4 + this->rand->nextInt(16) + 8;
	var18 = var5 + this->rand->nextInt(16) + 8;
	((WorldGenerator*)var24)->setScale(1.0, 1.0, 1.0);
	((WorldGenerator*)var24)->generate(this->worldObj, this->rand, var17, this->worldObj->getHeightValue(var17, var18), var18);
	}
	int var19;
	for(var16 = 0; var16 < 2; ++var16) {
	var17 = var4 + this->rand->nextInt(16) + 8;
	var18 = this->rand->nextInt(128);
	var19 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenFlowers(Block::s_plantYellow->blockID))->generate(this->worldObj, this->rand, var17, var18, var19);
	}
	if(this->rand->nextInt(2) == 0) {
	var16 = var4 + this->rand->nextInt(16) + 8;
	var17 = this->rand->nextInt(128);
	var18 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenFlowers(Block::s_plantRed->blockID))->generate(this->worldObj, this->rand, var16, var17, var18);
	}
	if(this->rand->nextInt(4) == 0) {
	var16 = var4 + this->rand->nextInt(16) + 8;
	var17 = this->rand->nextInt(128);
	var18 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenFlowers(Block::s_mushroomBrown->blockID))->generate(this->worldObj, this->rand, var16, var17, var18);
	}
	if(this->rand->nextInt(8) == 0) {
	var16 = var4 + this->rand->nextInt(16) + 8;
	var17 = this->rand->nextInt(128);
	var18 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenFlowers(Block::s_mushroomRed->blockID))->generate(this->worldObj, this->rand, var16, var17, var18);
	}
	for(var16 = 0; var16 < 10; ++var16) {
	var17 = var4 + this->rand->nextInt(16) + 8;
	var18 = this->rand->nextInt(128);
	var19 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenReed())->generate(this->worldObj, this->rand, var17, var18, var19);
	}
	if(this->rand->nextInt(32) == 0) {
	var16 = var4 + this->rand->nextInt(16) + 8;
	var17 = this->rand->nextInt(128);
	var18 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenPumpkin())->generate(this->worldObj, this->rand, var16, var17, var18);
	}
	var16 = 0;
	if(var6 == MobSpawnerBase::s_desert) {
	var16 += 10;
	}
	int var20;
	for(var17 = 0; var17 < var16; ++var17) {
	var18 = var4 + this->rand->nextInt(16) + 8;
	var19 = this->rand->nextInt(128);
	var20 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenCactus())->generate(this->worldObj, this->rand, var18, var19, var20);
	}
	for(var17 = 0; var17 < 50; ++var17) {
	var18 = var4 + this->rand->nextInt(16) + 8;
	var19 = this->rand->nextInt(this->rand->nextInt(120) + 8);
	var20 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenLiquids(Block::s_waterStill->blockID))->generate(this->worldObj, this->rand, var18, var19, var20);
	}
	for(var17 = 0; var17 < 20; ++var17) {
	var18 = var4 + this->rand->nextInt(16) + 8;
	var19 = this->rand->nextInt(this->rand->nextInt(this->rand->nextInt(112) + 8) + 8);
	var20 = var5 + this->rand->nextInt(16) + 8;
	(new WorldGenLiquids(Block::s_lavaStill->blockID))->generate(this->worldObj, this->rand, var18, var19, var20);
	}
	this->temperatureField = this->worldObj->getWorldChunkManager()->getTemperatures(this->temperatureField, var4 + 8, var5 + 8, 16, 16);
	for(var17 = var4 + 8; var17 < var4 + 8 + 16; ++var17) {
	for(var18 = var5 + 8; var18 < var5 + 8 + 16; ++var18) {
	var19 = var17 - (var4 + 8);
	var20 = var18 - (var5 + 8);
	int var21 = this->worldObj->getTopSolidOrLiquidBlock(var17, var18);
	double var22 = this->temperatureField[var19 * 16 + var20] - (double)(var21 - 64) / 64.0 * 0.3;
	if(var22 < 0.5 && var21 > 0 && var21 < 128 && this->worldObj->getBlockId(var17, var21, var18) == 0 && this->worldObj->getBlockMaterial(var17, var21 - 1, var18)->blocksMovement() && this->worldObj->getBlockMaterial(var17, var21 - 1, var18) != Material::s_ice) {
	this->worldObj->setBlockWithNotify(var17, var21, var18, Block::s_snow->blockID);
	}
	}
	}
	BlockSand::s_fallInstantly = false;
}

bool ChunkProviderGenerate::saveChunks(bool var1, IProgressUpdate* var2)
{
	return true;
}

bool ChunkProviderGenerate::unloadQueuedChunks()
{
	return false;
}

bool ChunkProviderGenerate::canSave()
{
	return true;
}

void ChunkProviderGenerate::initStatics()
{
}
