#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Block.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"
// Force a JCat concat chain into a copyable JavaString. Variadic ctors (e.g.
// IllegalArgumentException(...)) take args by value; JCat holds a non-copyable
// ostringstream, so it must be materialized to a string first.
namespace { inline JavaString _blkStr(const std::string& s) { return JavaString(s); } }
#include "AxisAlignedBB.h"
#include "BlockBloodStone.h"
#include "BlockBookshelf.h"
#include "BlockButton.h"
#include "BlockCactus.h"
#include "BlockChest.h"
#include "BlockClay.h"
#include "BlockCrops.h"
#include "BlockDirt.h"
#include "BlockDoor.h"
#include "BlockFence.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockFlowing.h"
#include "BlockFurnace.h"
#include "BlockGlass.h"
#include "BlockGrass.h"
#include "BlockGravel.h"
#include "BlockIce.h"
#include "BlockJukeBox.h"
#include "BlockLadder.h"
#include "BlockLeaves.h"
#include "BlockLever.h"
#include "BlockLightStone.h"
#include "BlockLog.h"
#include "BlockMinecartTrack.h"
#include "BlockMobSpawner.h"
#include "BlockMushroom.h"
#include "BlockObsidian.h"
#include "BlockOre.h"
#include "BlockOreBlock.h"
#include "BlockPortal.h"
#include "BlockPressurePlate.h"
#include "BlockPumpkin.h"
#include "BlockRedstoneOre.h"
#include "BlockRedstoneTorch.h"
#include "BlockRedstoneWire.h"
#include "BlockReed.h"
#include "BlockSand.h"
#include "BlockSapling.h"
#include "BlockSign.h"
#include "BlockSlowSand.h"
#include "BlockSnow.h"
#include "BlockSnowBlock.h"
#include "BlockSoil.h"
#include "BlockSponge.h"
#include "BlockStairs.h"
#include "BlockStationary.h"
#include "BlockStep.h"
#include "BlockStone.h"
#include "BlockTNT.h"
#include "BlockTorch.h"
#include "BlockWorkbench.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "EnumMobType.h"
#include "Explosion.h"
#include "File.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IInventory.h"
#include "IllegalArgumentException.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemBlock.h"   // CONSOLE: Block's static{} block creates an ItemBlock per registered block
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
#include "Slot.h"
#include "StepSound.h"
#include "StepSoundSand.h"
#include "StepSoundStone.h"
#include "TileEntity.h"
#include "TileEntitySign.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

StepSound* Block::s_soundPowderFootstep;
StepSound* Block::s_soundWoodFootstep;
StepSound* Block::s_soundGravelFootstep;
StepSound* Block::s_soundGrassFootstep;
StepSound* Block::s_soundStoneFootstep;
StepSound* Block::s_soundMetalFootstep;
StepSound* Block::s_soundGlassFootstep;
StepSound* Block::s_soundClothFootstep;
StepSound* Block::s_soundSandFootstep;
JArray<Block* > Block::s_blocksList;
JArray<bool > Block::s_tickOnLoad;
JArray<bool > Block::s_opaqueCubeLookup;
JArray<bool > Block::s_isBlockContainer;
JArray<int > Block::s_lightOpacity;
JArray<bool > Block::s_field_340_s;
JArray<int > Block::s_lightValue;
Block* Block::s_stone;
BlockGrass* Block::s_grass;
Block* Block::s_dirt;
Block* Block::s_cobblestone;
Block* Block::s_planks;
Block* Block::s_sapling;
Block* Block::s_bedrock;
Block* Block::s_waterStill;
Block* Block::s_waterMoving;
Block* Block::s_lavaStill;
Block* Block::s_lavaMoving;
Block* Block::s_sand;
Block* Block::s_gravel;
Block* Block::s_oreGold;
Block* Block::s_oreIron;
Block* Block::s_oreCoal;
Block* Block::s_wood;
BlockLeaves* Block::s_leaves;
Block* Block::s_sponge;
Block* Block::s_glass;
Block* Block::s_reservedBlock21;
Block* Block::s_reservedBlock22;
Block* Block::s_reservedBlock23;
Block* Block::s_reservedBlock24;
Block* Block::s_reservedBlock25;
Block* Block::s_reservedBlock26;
Block* Block::s_reservedBlock27;
Block* Block::s_reservedBlock28;
Block* Block::s_reservedBlock29;
Block* Block::s_reservedBlock30;
Block* Block::s_reservedBlock31;
Block* Block::s_reservedBlock32;
Block* Block::s_reservedBlock33;
Block* Block::s_reservedBlock34;
Block* Block::s_cloth;
Block* Block::s_reservedBlock36;
BlockFlower* Block::s_plantYellow;
BlockFlower* Block::s_plantRed;
BlockFlower* Block::s_mushroomBrown;
BlockFlower* Block::s_mushroomRed;
Block* Block::s_blockGold;
Block* Block::s_blockSteel;
Block* Block::s_stairDouble;
Block* Block::s_stairSingle;
Block* Block::s_brick;
Block* Block::s_tnt;
Block* Block::s_bookShelf;
Block* Block::s_cobblestoneMossy;
Block* Block::s_obsidian;
Block* Block::s_torchWood;
BlockFire* Block::s_fire;
Block* Block::s_mobSpawner;
Block* Block::s_stairCompactPlanks;
Block* Block::s_crate;
Block* Block::s_redstoneWire;
Block* Block::s_oreDiamond;
Block* Block::s_blockDiamond;
Block* Block::s_workbench;
Block* Block::s_crops;
Block* Block::s_tilledField;
Block* Block::s_stoneOvenIdle;
Block* Block::s_stoneOvenActive;
Block* Block::s_signPost;
Block* Block::s_doorWood;
Block* Block::s_ladder;
Block* Block::s_minecartTrack;
Block* Block::s_stairCompactCobblestone;
Block* Block::s_signWall;
Block* Block::s_lever;
Block* Block::s_pressurePlateStone;
Block* Block::s_doorSteel;
Block* Block::s_pressurePlatePlanks;
Block* Block::s_oreRedstone;
Block* Block::s_oreRedstoneGlowing;
Block* Block::s_torchRedstoneIdle;
Block* Block::s_torchRedstoneActive;
Block* Block::s_button;
Block* Block::s_snow;
Block* Block::s_blockIce;
Block* Block::s_blockSnow;
Block* Block::s_cactus;
Block* Block::s_blockClay;
Block* Block::s_reed;
Block* Block::s_jukebox;
Block* Block::s_fence;
Block* Block::s_pumpkin;
Block* Block::s_bloodStone;
Block* Block::s_slowSand;
Block* Block::s_lightStone;
BlockPortal* Block::s_portal;
Block* Block::s_pumpkinLantern;

Block::Block(int var1, Material* var2)
	: blockIndexInTexture(0)
	, blockID(0)
	, blockHardness(0)
	, blockResistance(0)
	, field_370_bf(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
	, stepSound(NULL)
	, blockParticleGravity(0)
	, blockMaterial(NULL)
	, slipperiness(0)
{
	this->stepSound = s_soundPowderFootstep;
	this->blockParticleGravity = 1.0F;
	this->slipperiness = 0.6F;
	if(s_blocksList[var1] != NULL) {
	throw new IllegalArgumentException(_blkStr(JCat() + "Slot " + var1 + " is already occupied by " + s_blocksList[var1] + " when adding " + this));
	} else {
	this->blockMaterial = var2;
	s_blocksList[var1] = this;
	this->blockID = var1;
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
	s_opaqueCubeLookup[var1] = this->isOpaqueCube();
	s_lightOpacity[var1] = this->isOpaqueCube() ? 255 : 0;
	s_field_340_s[var1] = this->unusedMethod();
	s_isBlockContainer[var1] = false;
	}
}

Block::Block(int var1, int var2, Material* var3)
	: blockIndexInTexture(0)
	, blockID(0)
	, blockHardness(0)
	, blockResistance(0)
	, field_370_bf(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
	, stepSound(NULL)
	, blockParticleGravity(0)
	, blockMaterial(NULL)
	, slipperiness(0)
{
	// inlined this(var1, var3) -- C++03 lacks ctor delegation
	this->stepSound = s_soundPowderFootstep;
	this->blockParticleGravity = 1.0F;
	this->slipperiness = 0.6F;
	if(s_blocksList[var1] != NULL) {
	throw new IllegalArgumentException(_blkStr(JCat() + "Slot " + var1 + " is already occupied by " + s_blocksList[var1] + " when adding " + this));
	} else {
	this->blockMaterial = var3;
	s_blocksList[var1] = this;
	this->blockID = var1;
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
	s_opaqueCubeLookup[var1] = this->isOpaqueCube();
	s_lightOpacity[var1] = this->isOpaqueCube() ? 255 : 0;
	s_field_340_s[var1] = this->unusedMethod();
	s_isBlockContainer[var1] = false;
	}
	this->blockIndexInTexture = var2;
}

Block::~Block()
{
}

Block* Block::setStepSound(StepSound* var1)
{
	this->stepSound = var1;
	return this;
}

Block* Block::setLightOpacity(int var1)
{
	s_lightOpacity[this->blockID] = var1;
	return this;
}

Block* Block::setLightValue(float var1)
{
	s_lightValue[this->blockID] = (int)(15.0F * var1);
	return this;
}

Block* Block::setResistance(float var1)
{
	this->blockResistance = var1 * 3.0F;
	return this;
}

bool Block::unusedMethod()
{
	return false;
}

bool Block::renderAsNormalBlock()
{
	return true;
}

int Block::getRenderType()
{
	return 0;
}

Block* Block::setHardness(float var1)
{
	this->blockHardness = var1;
	if(this->blockResistance < var1 * 5.0F) {
	this->blockResistance = var1 * 5.0F;
	}
	return this;
}

void Block::setTickOnLoad(bool var1)
{
	s_tickOnLoad[this->blockID] = var1;
}

void Block::setBlockBounds(float var1, float var2, float var3, float var4, float var5, float var6)
{
	this->field_370_bf = (double)var1;
	this->minY = (double)var2;
	this->minZ = (double)var3;
	this->maxX = (double)var4;
	this->maxY = (double)var5;
	this->maxZ = (double)var6;
}

float Block::getBlockBrightness(IBlockAccess* var1, int var2, int var3, int var4)
{
	return var1->getLightBrightness(var2, var3, var4);
}

bool Block::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	return var5 == 0 && this->minY > 0.0 ? true : (var5 == 1 && this->maxY < 1.0 ? true : (var5 == 2 && this->minZ > 0.0 ? true : (var5 == 3 && this->maxZ < 1.0 ? true : (var5 == 4 && this->field_370_bf > 0.0 ? true : (var5 == 5 && this->maxX < 1.0 ? true : !var1->isBlockOpaqueCube(var2, var3, var4))))));
}

int Block::getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	return this->getBlockTextureFromSideAndMetadata(var5, var1->getBlockMetadata(var2, var3, var4));
}

int Block::getBlockTextureFromSideAndMetadata(int var1, int var2)
{
	return this->getBlockTextureFromSide(var1);
}

int Block::getBlockTextureFromSide(int var1)
{
	return this->blockIndexInTexture;
}

AxisAlignedBB* Block::getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return AxisAlignedBB::getBoundingBoxFromPool((double)var2 + this->field_370_bf, (double)var3 + this->minY, (double)var4 + this->minZ, (double)var2 + this->maxX, (double)var3 + this->maxY, (double)var4 + this->maxZ);
}

void Block::getCollidingBoundingBoxes(World* var1, int var2, int var3, int var4, AxisAlignedBB* var5, JList<void*>& var6)
{
	AxisAlignedBB* var7 = this->getCollisionBoundingBoxFromPool(var1, var2, var3, var4);
	if(var7 != NULL && var5->intersectsWith(var7)) {
	var6.push_back(var7);
	}
}

AxisAlignedBB* Block::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return AxisAlignedBB::getBoundingBoxFromPool((double)var2 + this->field_370_bf, (double)var3 + this->minY, (double)var4 + this->minZ, (double)var2 + this->maxX, (double)var3 + this->maxY, (double)var4 + this->maxZ);
}

bool Block::isOpaqueCube()
{
	return true;
}

bool Block::canCollideCheck(int var1, bool var2)
{
	return this->isCollidable();
}

bool Block::isCollidable()
{
	return true;
}

void Block::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
}

void Block::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5)
{
}

void Block::onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5)
{
}

void Block::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
}

int Block::tickRate()
{
	return 10;
}

void Block::onBlockAdded(World* var1, int var2, int var3, int var4)
{
}

void Block::onBlockRemoval(World* var1, int var2, int var3, int var4)
{
}

int Block::quantityDropped(Random* var1)
{
	return 1;
}

int Block::idDropped(int var1, Random* var2)
{
	return this->blockID;
}

float Block::getPlayerRelativeBlockHardness(EntityPlayer* var1)
{
	return this->blockHardness < 0.0F ? 0.0F : (!var1->canHarvestBlock(this) ? 1.0F / this->blockHardness / 100.0F : var1->getCurrentPlayerStrVsBlock(this) / this->blockHardness / 30.0F);
}

void Block::dropBlockAsItem(World* var1, int var2, int var3, int var4, int var5)
{
	this->dropBlockAsItemWithChance(var1, var2, var3, var4, var5, 1.0F);
}

void Block::dropBlockAsItemWithChance(World* var1, int var2, int var3, int var4, int var5, float var6)
{
	if(!var1->multiplayerWorld) {
	int var7 = this->quantityDropped(var1->rand);
	for(int var8 = 0; var8 < var7; ++var8) {
	if(var1->rand->nextFloat() <= var6) {
	int var9 = this->idDropped(var5, var1->rand);
	if(var9 > 0) {
	float var10 = 0.7F;
	double var11 = (double)(var1->rand->nextFloat() * var10) + (double)(1.0F - var10) * 0.5;
	double var13 = (double)(var1->rand->nextFloat() * var10) + (double)(1.0F - var10) * 0.5;
	double var15 = (double)(var1->rand->nextFloat() * var10) + (double)(1.0F - var10) * 0.5;
	EntityItem* var17 = new EntityItem(var1, (double)var2 + var11, (double)var3 + var13, (double)var4 + var15, new ItemStack(var9));
	var17->delayBeforeCanPickup = 10;
	var1->entityJoinedWorld(var17);
	}
	}
	}
	}
}

float Block::getExplosionResistance(Entity* var1)
{
	return this->blockResistance / 5.0F;
}

MovingObjectPosition* Block::collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6)
{
	this->setBlockBoundsBasedOnState(var1, var2, var3, var4);
	var5 = var5->addVector((double)(-var2), (double)(-var3), (double)(-var4));
	var6 = var6->addVector((double)(-var2), (double)(-var3), (double)(-var4));
	Vec3D* var7 = var5->getIntermediateWithXValue(var6, this->field_370_bf);
	Vec3D* var8 = var5->getIntermediateWithXValue(var6, this->maxX);
	Vec3D* var9 = var5->getIntermediateWithYValue(var6, this->minY);
	Vec3D* var10 = var5->getIntermediateWithYValue(var6, this->maxY);
	Vec3D* var11 = var5->getIntermediateWithZValue(var6, this->minZ);
	Vec3D* var12 = var5->getIntermediateWithZValue(var6, this->maxZ);
	if(!this->isVecInsideYZBounds(var7)) {
	var7 = NULL;
	}
	if(!this->isVecInsideYZBounds(var8)) {
	var8 = NULL;
	}
	if(!this->isVecInsideXZBounds(var9)) {
	var9 = NULL;
	}
	if(!this->isVecInsideXZBounds(var10)) {
	var10 = NULL;
	}
	if(!this->isVecInsideXYBounds(var11)) {
	var11 = NULL;
	}
	if(!this->isVecInsideXYBounds(var12)) {
	var12 = NULL;
	}
	Vec3D* var13 = NULL;
	if(var7 != NULL && (var13 == NULL || var5->distanceTo(var7) < var5->distanceTo(var13))) {
	var13 = var7;
	}
	if(var8 != NULL && (var13 == NULL || var5->distanceTo(var8) < var5->distanceTo(var13))) {
	var13 = var8;
	}
	if(var9 != NULL && (var13 == NULL || var5->distanceTo(var9) < var5->distanceTo(var13))) {
	var13 = var9;
	}
	if(var10 != NULL && (var13 == NULL || var5->distanceTo(var10) < var5->distanceTo(var13))) {
	var13 = var10;
	}
	if(var11 != NULL && (var13 == NULL || var5->distanceTo(var11) < var5->distanceTo(var13))) {
	var13 = var11;
	}
	if(var12 != NULL && (var13 == NULL || var5->distanceTo(var12) < var5->distanceTo(var13))) {
	var13 = var12;
	}
	if(var13 == NULL) {
	return NULL;
	} else {
	unsigned char var14 = -1;
	if(var13 == var7) {
	var14 = 4;
	}
	if(var13 == var8) {
	var14 = 5;
	}
	if(var13 == var9) {
	var14 = 0;
	}
	if(var13 == var10) {
	var14 = 1;
	}
	if(var13 == var11) {
	var14 = 2;
	}
	if(var13 == var12) {
	var14 = 3;
	}
	return new MovingObjectPosition(var2, var3, var4, var14, var13->addVector((double)var2, (double)var3, (double)var4));
	}
}

bool Block::isVecInsideYZBounds(Vec3D* var1)
{
	return var1 == NULL ? false : var1->yCoord >= this->minY && var1->yCoord <= this->maxY && var1->zCoord >= this->minZ && var1->zCoord <= this->maxZ;
}

bool Block::isVecInsideXZBounds(Vec3D* var1)
{
	return var1 == NULL ? false : var1->xCoord >= this->field_370_bf && var1->xCoord <= this->maxX && var1->zCoord >= this->minZ && var1->zCoord <= this->maxZ;
}

bool Block::isVecInsideXYBounds(Vec3D* var1)
{
	return var1 == NULL ? false : var1->xCoord >= this->field_370_bf && var1->xCoord <= this->maxX && var1->yCoord >= this->minY && var1->yCoord <= this->maxY;
}

void Block::onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4)
{
}

int Block::getRenderBlockPass()
{
	return 0;
}

bool Block::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockId(var2, var3, var4);
	return var5 == 0 || s_blocksList[var5]->blockMaterial->getIsLiquid();
}

bool Block::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5)
{
	return false;
}

void Block::onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5)
{
}

void Block::onBlockPlaced(World* var1, int var2, int var3, int var4, int var5)
{
}

void Block::onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5)
{
}

void Block::velocityToAddToEntity(World* var1, int var2, int var3, int var4, Entity* var5, Vec3D* var6)
{
}

void Block::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4)
{
}

int Block::colorMultiplier(IBlockAccess* var1, int var2, int var3, int var4)
{
	return 16777215;
}

bool Block::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	return false;
}

bool Block::canProvidePower()
{
	return false;
}

void Block::onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5)
{
}

bool Block::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5)
{
	return false;
}

void Block::setBlockBoundsForItemRender()
{
}

void Block::harvestBlock(World* var1, int var2, int var3, int var4, int var5)
{
	this->dropBlockAsItem(var1, var2, var3, var4, var5);
}

bool Block::canBlockStay(World* var1, int var2, int var3, int var4)
{
	return true;
}

void Block::onBlockPlacedBy(World* var1, int var2, int var3, int var4, EntityLiving* var5)
{
}


// Deferred static initialization (ordered by j2x_initAll()): was eager static
// definitions whose cross-translation-unit init order is undefined.
void Block::initStatics()
{
	Block::s_soundPowderFootstep = new StepSound("stone", 1.0F, 1.0F);
	Block::s_soundWoodFootstep = new StepSound("wood", 1.0F, 1.0F);
	Block::s_soundGravelFootstep = new StepSound("gravel", 1.0F, 1.0F);
	Block::s_soundGrassFootstep = new StepSound("grass", 1.0F, 1.0F);
	Block::s_soundStoneFootstep = new StepSound("stone", 1.0F, 1.0F);
	Block::s_soundMetalFootstep = new StepSound("stone", 1.0F, 1.5F);
	Block::s_soundGlassFootstep = new StepSoundStone("stone", 1.0F, 1.0F);
	Block::s_soundClothFootstep = new StepSound("cloth", 1.0F, 1.0F);
	Block::s_soundSandFootstep = new StepSoundSand("sand", 1.0F, 1.0F);
	Block::s_blocksList = JArray<Block* >(256);
	Block::s_tickOnLoad = JArray<bool >(256);
	Block::s_opaqueCubeLookup = JArray<bool >(256);
	Block::s_isBlockContainer = JArray<bool >(256);
	Block::s_lightOpacity = JArray<int >(256);
	Block::s_field_340_s = JArray<bool >(256);
	Block::s_lightValue = JArray<int >(256);
	Block::s_stone = (new BlockStone(1, 1))->setHardness(1.5F)->setResistance(10.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_grass = (BlockGrass*)(new BlockGrass(2))->setHardness(0.6F)->setStepSound(s_soundGrassFootstep);
	Block::s_dirt = (new BlockDirt(3, 2))->setHardness(0.5F)->setStepSound(s_soundGravelFootstep);
	Block::s_cobblestone = (new Block(4, 16, Material::s_rock))->setHardness(2.0F)->setResistance(10.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_planks = (new Block(5, 4, Material::s_wood))->setHardness(2.0F)->setResistance(5.0F)->setStepSound(s_soundWoodFootstep);
	Block::s_sapling = (new BlockSapling(6, 15))->setHardness(0.0F)->setStepSound(s_soundGrassFootstep);
	Block::s_bedrock = (new Block(7, 17, Material::s_rock))->setHardness(-1.0F)->setResistance(6000000.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_waterStill = (new BlockFlowing(8, Material::s_water))->setHardness(100.0F)->setLightOpacity(3);
	Block::s_waterMoving = (new BlockStationary(9, Material::s_water))->setHardness(100.0F)->setLightOpacity(3);
	Block::s_lavaStill = (new BlockFlowing(10, Material::s_lava))->setHardness(0.0F)->setLightValue(1.0F)->setLightOpacity(255);
	Block::s_lavaMoving = (new BlockStationary(11, Material::s_lava))->setHardness(100.0F)->setLightValue(1.0F)->setLightOpacity(255);
	Block::s_sand = (new BlockSand(12, 18))->setHardness(0.5F)->setStepSound(s_soundSandFootstep);
	Block::s_gravel = (new BlockGravel(13, 19))->setHardness(0.6F)->setStepSound(s_soundGravelFootstep);
	Block::s_oreGold = (new BlockOre(14, 32))->setHardness(3.0F)->setResistance(5.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_oreIron = (new BlockOre(15, 33))->setHardness(3.0F)->setResistance(5.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_oreCoal = (new BlockOre(16, 34))->setHardness(3.0F)->setResistance(5.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_wood = (new BlockLog(17))->setHardness(2.0F)->setStepSound(s_soundWoodFootstep);
	Block::s_leaves = (BlockLeaves*)(new BlockLeaves(18, 52))->setHardness(0.2F)->setLightOpacity(1)->setStepSound(s_soundGrassFootstep);
	Block::s_sponge = (new BlockSponge(19))->setHardness(0.6F)->setStepSound(s_soundGrassFootstep);
	Block::s_glass = (new BlockGlass(20, 49, Material::s_glass, false))->setHardness(0.3F)->setStepSound(s_soundGlassFootstep);
	Block::s_reservedBlock21 = NULL;
	Block::s_reservedBlock22 = NULL;
	Block::s_reservedBlock23 = NULL;
	Block::s_reservedBlock24 = NULL;
	Block::s_reservedBlock25 = NULL;
	Block::s_reservedBlock26 = NULL;
	Block::s_reservedBlock27 = NULL;
	Block::s_reservedBlock28 = NULL;
	Block::s_reservedBlock29 = NULL;
	Block::s_reservedBlock30 = NULL;
	Block::s_reservedBlock31 = NULL;
	Block::s_reservedBlock32 = NULL;
	Block::s_reservedBlock33 = NULL;
	Block::s_reservedBlock34 = NULL;
	Block::s_cloth = (new Block(35, 64, Material::s_cloth))->setHardness(0.8F)->setStepSound(s_soundClothFootstep);
	Block::s_reservedBlock36 = NULL;
	Block::s_plantYellow = (BlockFlower*)(new BlockFlower(37, 13))->setHardness(0.0F)->setStepSound(s_soundGrassFootstep);
	Block::s_plantRed = (BlockFlower*)(new BlockFlower(38, 12))->setHardness(0.0F)->setStepSound(s_soundGrassFootstep);
	Block::s_mushroomBrown = (BlockFlower*)(new BlockMushroom(39, 29))->setHardness(0.0F)->setStepSound(s_soundGrassFootstep)->setLightValue(2.0F / 16.0F);
	Block::s_mushroomRed = (BlockFlower*)(new BlockMushroom(40, 28))->setHardness(0.0F)->setStepSound(s_soundGrassFootstep);
	Block::s_blockGold = (new BlockOreBlock(41, 39))->setHardness(3.0F)->setResistance(10.0F)->setStepSound(s_soundMetalFootstep);
	Block::s_blockSteel = (new BlockOreBlock(42, 38))->setHardness(5.0F)->setResistance(10.0F)->setStepSound(s_soundMetalFootstep);
	Block::s_stairDouble = (new BlockStep(43, true))->setHardness(2.0F)->setResistance(10.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_stairSingle = (new BlockStep(44, false))->setHardness(2.0F)->setResistance(10.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_brick = (new Block(45, 7, Material::s_rock))->setHardness(2.0F)->setResistance(10.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_tnt = (new BlockTNT(46, 8))->setHardness(0.0F)->setStepSound(s_soundGrassFootstep);
	Block::s_bookShelf = (new BlockBookshelf(47, 35))->setHardness(1.5F)->setStepSound(s_soundWoodFootstep);
	Block::s_cobblestoneMossy = (new Block(48, 36, Material::s_rock))->setHardness(2.0F)->setResistance(10.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_obsidian = (new BlockObsidian(49, 37))->setHardness(10.0F)->setResistance(2000.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_torchWood = (new BlockTorch(50, 80))->setHardness(0.0F)->setLightValue(15.0F / 16.0F)->setStepSound(s_soundWoodFootstep);
	Block::s_fire = (BlockFire*)((BlockFire*)(new BlockFire(51, 31))->setHardness(0.0F)->setLightValue(1.0F)->setStepSound(s_soundWoodFootstep));
	Block::s_mobSpawner = (new BlockMobSpawner(52, 65))->setHardness(5.0F)->setStepSound(s_soundMetalFootstep);
	Block::s_stairCompactPlanks = new BlockStairs(53, s_planks);
	Block::s_crate = (new BlockChest(54))->setHardness(2.5F)->setStepSound(s_soundWoodFootstep);
	Block::s_redstoneWire = (new BlockRedstoneWire(55, 84))->setHardness(0.0F)->setStepSound(s_soundPowderFootstep);
	Block::s_oreDiamond = (new BlockOre(56, 50))->setHardness(3.0F)->setResistance(5.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_blockDiamond = (new BlockOreBlock(57, 40))->setHardness(5.0F)->setResistance(10.0F)->setStepSound(s_soundMetalFootstep);
	Block::s_workbench = (new BlockWorkbench(58))->setHardness(2.5F)->setStepSound(s_soundWoodFootstep);
	Block::s_crops = (new BlockCrops(59, 88))->setHardness(0.0F)->setStepSound(s_soundGrassFootstep);
	Block::s_tilledField = (new BlockSoil(60))->setHardness(0.6F)->setStepSound(s_soundGravelFootstep);
	Block::s_stoneOvenIdle = (new BlockFurnace(61, false))->setHardness(3.5F)->setStepSound(s_soundStoneFootstep);
	Block::s_stoneOvenActive = (new BlockFurnace(62, true))->setHardness(3.5F)->setStepSound(s_soundStoneFootstep)->setLightValue(14.0F / 16.0F);
	Block::s_signPost = (new BlockSign(63, Class(), true))->setHardness(1.0F)->setStepSound(s_soundWoodFootstep);
	Block::s_doorWood = (new BlockDoor(64, Material::s_wood))->setHardness(3.0F)->setStepSound(s_soundWoodFootstep);
	Block::s_ladder = (new BlockLadder(65, 83))->setHardness(0.4F)->setStepSound(s_soundWoodFootstep);
	Block::s_minecartTrack = (new BlockMinecartTrack(66, 128))->setHardness(0.7F)->setStepSound(s_soundMetalFootstep);
	Block::s_stairCompactCobblestone = new BlockStairs(67, s_cobblestone);
	Block::s_signWall = (new BlockSign(68, Class(), false))->setHardness(1.0F)->setStepSound(s_soundWoodFootstep);
	Block::s_lever = (new BlockLever(69, 96))->setHardness(0.5F)->setStepSound(s_soundWoodFootstep);
	Block::s_pressurePlateStone = (new BlockPressurePlate(70, s_stone->blockIndexInTexture, EnumMobType::mobs))->setHardness(0.5F)->setStepSound(s_soundStoneFootstep);
	Block::s_doorSteel = (new BlockDoor(71, Material::s_iron))->setHardness(5.0F)->setStepSound(s_soundMetalFootstep);
	Block::s_pressurePlatePlanks = (new BlockPressurePlate(72, s_planks->blockIndexInTexture, EnumMobType::everything))->setHardness(0.5F)->setStepSound(s_soundWoodFootstep);
	Block::s_oreRedstone = (new BlockRedstoneOre(73, 51, false))->setHardness(3.0F)->setResistance(5.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_oreRedstoneGlowing = (new BlockRedstoneOre(74, 51, true))->setLightValue(10.0F / 16.0F)->setHardness(3.0F)->setResistance(5.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_torchRedstoneIdle = (new BlockRedstoneTorch(75, 115, false))->setHardness(0.0F)->setStepSound(s_soundWoodFootstep);
	Block::s_torchRedstoneActive = (new BlockRedstoneTorch(76, 99, true))->setHardness(0.0F)->setLightValue(0.5F)->setStepSound(s_soundWoodFootstep);
	Block::s_button = (new BlockButton(77, s_stone->blockIndexInTexture))->setHardness(0.5F)->setStepSound(s_soundStoneFootstep);
	Block::s_snow = (new BlockSnow(78, 66))->setHardness(0.1F)->setStepSound(s_soundClothFootstep);
	Block::s_blockIce = (new BlockIce(79, 67))->setHardness(0.5F)->setLightOpacity(3)->setStepSound(s_soundGlassFootstep);
	Block::s_blockSnow = (new BlockSnowBlock(80, 66))->setHardness(0.2F)->setStepSound(s_soundClothFootstep);
	Block::s_cactus = (new BlockCactus(81, 70))->setHardness(0.4F)->setStepSound(s_soundClothFootstep);
	Block::s_blockClay = (new BlockClay(82, 72))->setHardness(0.6F)->setStepSound(s_soundGravelFootstep);
	Block::s_reed = (new BlockReed(83, 73))->setHardness(0.0F)->setStepSound(s_soundGrassFootstep);
	Block::s_jukebox = (new BlockJukeBox(84, 74))->setHardness(2.0F)->setResistance(10.0F)->setStepSound(s_soundStoneFootstep);
	Block::s_fence = (new BlockFence(85, 4))->setHardness(2.0F)->setResistance(5.0F)->setStepSound(s_soundWoodFootstep);
	Block::s_pumpkin = (new BlockPumpkin(86, 102, false))->setHardness(1.0F)->setStepSound(s_soundWoodFootstep);
	Block::s_bloodStone = (new BlockBloodStone(87, 103))->setHardness(0.4F)->setStepSound(s_soundStoneFootstep);
	Block::s_slowSand = (new BlockSlowSand(88, 104))->setHardness(0.5F)->setStepSound(s_soundSandFootstep);
	Block::s_lightStone = (new BlockLightStone(89, 105, Material::s_glass))->setHardness(0.3F)->setStepSound(s_soundGlassFootstep)->setLightValue(1.0F);
	Block::s_portal = (BlockPortal*)((BlockPortal*)(new BlockPortal(90, 14))->setHardness(-1.0F)->setStepSound(s_soundGlassFootstep)->setLightValue(12.0F / 16.0F));
	Block::s_pumpkinLantern = (new BlockPumpkin(91, 102, true))->setHardness(1.0F)->setStepSound(s_soundWoodFootstep)->setLightValue(1.0F);
	// CONSOLE (stale-override fix): Java `static { for(i 0..256) if(blocksList[i]) itemsList[i]=new ItemBlock(i-256); }`
	// was dropped by this handwritten override -> every block had NO item (Item::s_itemsList[blockId] NULL):
	// empty hotbar/inventory for blocks + null ItemBlock derefs. The converter emits this correctly
	// (render_source); restore it AFTER all blocks are registered (needs Item::initStatics already run).
	for(int var0 = 0; var0 < 256; ++var0) {
	if(s_blocksList[var0] != NULL) {
	Item::s_itemsList[var0] = new ItemBlock(var0 - 256);
	}
	}
	// CONSOLE (virtual-call-in-ctor opacity fix): the Block ctor set s_opaqueCubeLookup/s_lightOpacity
	// from this->isOpaqueCube(), but a virtual call in a ctor binds to Block::isOpaqueCube() (always
	// true) and IGNORES overrides like BlockSnow/glass/flowers (false). Non-opaque blocks were left
	// marked opacity 255 -> they BLOCKED skylight -> snow-covered terrain rendered BLACK. Recompute now
	// that all vtables are live. Keep explicit setLightOpacity values (leaves=1, water/ice=3): only drop
	// a wrongly-255 non-opaque block to 0.
	for(int var0b = 0; var0b < 256; ++var0b) {
	if(s_blocksList[var0b] != NULL) {
	bool _op = s_blocksList[var0b]->isOpaqueCube();
	s_opaqueCubeLookup[var0b] = _op;
	if(!_op && s_lightOpacity[var0b] == 255) {
	s_lightOpacity[var0b] = 0;
	}
	}
	}
}