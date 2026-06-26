#include "stdafx.h"
// Auto-generated from Block::s_java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"
#include "BlockSign.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "IBlockAccess.h"
#include "Material.h"
#include "MovingObjectPosition.h"
#include "Random.h"
#include "StepSound.h"
#include "Vec3D.h"
#include "World.h"
#include "EntityItem.h"
#include "Item.h"
#include "ItemBlock.h"
#include "ItemStack.h"
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
#include "BlockFlowing.h"
#include "BlockFurnace.h"
#include "BlockGlass.h"
#include "BlockGravel.h"
#include "BlockIce.h"
#include "BlockJukeBox.h"
#include "BlockLadder.h"
#include "BlockLever.h"
#include "BlockLightStone.h"
#include "BlockLog.h"
#include "BlockMinecartTrack.h"
#include "BlockMobSpawner.h"
#include "BlockMushroom.h"
#include "BlockObsidian.h"
#include "BlockOre.h"
#include "BlockOreBlock.h"
#include "BlockPressurePlate.h"
#include "BlockPumpkin.h"
#include "BlockRedstoneOre.h"
#include "BlockRedstoneTorch.h"
#include "BlockRedstoneWire.h"
#include "BlockReed.h"
#include "BlockSand.h"
#include "BlockSapling.h"
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
#include "EnumMobType.h"
#include "InventoryPlayer.h"
#include "StepSoundSand.h"
#include "StepSoundStone.h"

StepSound* Block::s_soundPowderFootstep = new StepSound("s_stone", 1.0f, 1.0f);
StepSound* Block::s_soundWoodFootstep = new StepSound("s_wood", 1.0f, 1.0f);
StepSound* Block::s_soundGravelFootstep = new StepSound("s_gravel", 1.0f, 1.0f);
StepSound* Block::s_soundGrassFootstep = new StepSound("s_grass", 1.0f, 1.0f);
StepSound* Block::s_soundStoneFootstep = new StepSound("s_stone", 1.0f, 1.0f);
StepSound* Block::s_soundMetalFootstep = new StepSound("s_stone", 1.0f, 1.5f);
StepSound* Block::s_soundGlassFootstep = new StepSoundStone("s_stone", 1.0f, 1.0f);
StepSound* Block::s_soundClothFootstep = new StepSound("s_cloth", 1.0f, 1.0f);
StepSound* Block::s_soundSandFootstep = new StepSoundSand("s_sand", 1.0f, 1.0f);
Block** Block::blocksList = new Block*[256]();
bool* Block::s_tickOnLoad = new bool[256];
bool* Block::s_opaqueCubeLookup = new bool[256];
bool* Block::s_isBlockContainer = new bool[256];
int* Block::s_lightOpacity = new int[256];
bool* Block::s_field_340_s = new bool[256];
int* Block::s_lightValue = new int[256];
Block* Block::s_stone = (new BlockStone(1, 1))->setHardness(1.5f)->setResistance(10.0f)->setStepSound(s_soundStoneFootstep);
BlockGrass* Block::s_grass = (BlockGrass*)(new BlockGrass(2))->setHardness(0.6f)->setStepSound(s_soundGrassFootstep);
Block* Block::s_dirt = (new BlockDirt(3, 2))->setHardness(0.5f)->setStepSound(s_soundGravelFootstep);
Block* Block::s_cobblestone = (new Block(4, 16, Material::s_rock))->setHardness(2.0f)->setResistance(10.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_planks = (new Block(5, 4, Material::s_wood))->setHardness(2.0f)->setResistance(5.0f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_sapling = (new BlockSapling(6, 15))->setHardness(0.0f)->setStepSound(s_soundGrassFootstep);
Block* Block::s_bedrock = (new Block(7, 17, Material::s_rock))->setHardness(-1.0f)->setResistance(6000000.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_waterStill = (new BlockFlowing(8, Material::s_water))->setHardness(100.0f)->setLightOpacity(3);
Block* Block::s_waterMoving = (new BlockStationary(9, Material::s_water))->setHardness(100.0f)->setLightOpacity(3);
Block* Block::s_lavaStill = (new BlockFlowing(10, Material::s_lava))->setHardness(0.0f)->setLightValue(1.0f)->setLightOpacity(255);
Block* Block::s_lavaMoving = (new BlockStationary(11, Material::s_lava))->setHardness(100.0f)->setLightValue(1.0f)->setLightOpacity(255);
Block* Block::s_sand = (new BlockSand(12, 18))->setHardness(0.5f)->setStepSound(s_soundSandFootstep);
Block* Block::s_gravel = (new BlockGravel(13, 19))->setHardness(0.6f)->setStepSound(s_soundGravelFootstep);
Block* Block::s_oreGold = (new BlockOre(14, 32))->setHardness(3.0f)->setResistance(5.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_oreIron = (new BlockOre(15, 33))->setHardness(3.0f)->setResistance(5.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_oreCoal = (new BlockOre(16, 34))->setHardness(3.0f)->setResistance(5.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_wood = (new BlockLog(17))->setHardness(2.0f)->setStepSound(s_soundWoodFootstep);
BlockLeaves* Block::s_leaves = (BlockLeaves*)(new BlockLeaves(18, 52))->setHardness(0.2f)->setLightOpacity(1)->setStepSound(s_soundGrassFootstep);
Block* Block::s_sponge = (new BlockSponge(19))->setHardness(0.6f)->setStepSound(s_soundGrassFootstep);
Block* Block::s_glass = (new BlockGlass(20, 49, Material::s_glass, false))->setHardness(0.3f)->setStepSound(s_soundGlassFootstep);
Block* Block::s_field_9267_N = NULL;
Block* Block::s_field_9266_O = NULL;
Block* Block::s_field_9265_P = NULL;
Block* Block::s_field_9264_Q = NULL;
Block* Block::s_field_9263_R = NULL;
Block* Block::s_field_9262_S = NULL;
Block* Block::s_field_9261_T = NULL;
Block* Block::s_field_9260_U = NULL;
Block* Block::s_field_9259_V = NULL;
Block* Block::s_field_9258_W = NULL;
Block* Block::s_field_9257_X = NULL;
Block* Block::s_field_9256_Y = NULL;
Block* Block::s_field_9255_Z = NULL;
Block* Block::s_field_9269_aa = NULL;
Block* Block::s_cloth = (new Block(35, 64, Material::s_cloth))->setHardness(0.8f)->setStepSound(s_soundClothFootstep);
Block* Block::s_field_9268_ac = NULL;
BlockFlower* Block::s_plantYellow = (BlockFlower*)(new BlockFlower(37, 13))->setHardness(0.0f)->setStepSound(s_soundGrassFootstep);
BlockFlower* Block::s_plantRed = (BlockFlower*)(new BlockFlower(38, 12))->setHardness(0.0f)->setStepSound(s_soundGrassFootstep);
BlockFlower* Block::s_mushroomBrown = (BlockFlower*)(new BlockMushroom(39, 29))->setHardness(0.0f)->setStepSound(s_soundGrassFootstep)->setLightValue(2.0f / 16.0f);
BlockFlower* Block::s_mushroomRed = (BlockFlower*)(new BlockMushroom(40, 28))->setHardness(0.0f)->setStepSound(s_soundGrassFootstep);
Block* Block::s_blockGold = (new BlockOreBlock(41, 39))->setHardness(3.0f)->setResistance(10.0f)->setStepSound(s_soundMetalFootstep);
Block* Block::s_blockSteel = (new BlockOreBlock(42, 38))->setHardness(5.0f)->setResistance(10.0f)->setStepSound(s_soundMetalFootstep);
Block* Block::s_stairDouble = (new BlockStep(43, true))->setHardness(2.0f)->setResistance(10.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_stairSingle = (new BlockStep(44, false))->setHardness(2.0f)->setResistance(10.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_brick = (new Block(45, 7, Material::s_rock))->setHardness(2.0f)->setResistance(10.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_tnt = (new BlockTNT(46, 8))->setHardness(0.0f)->setStepSound(s_soundGrassFootstep);
Block* Block::s_bookShelf = (new BlockBookshelf(47, 35))->setHardness(1.5f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_cobblestoneMossy = (new Block(48, 36, Material::s_rock))->setHardness(2.0f)->setResistance(10.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_obsidian = (new BlockObsidian(49, 37))->setHardness(10.0f)->setResistance(2000.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_torchWood = (new BlockTorch(50, 80))->setHardness(0.0f)->setLightValue(15.0f / 16.0f)->setStepSound(s_soundWoodFootstep);
BlockFire* Block::s_fire = (BlockFire*)((BlockFire*)(new BlockFire(51, 31))->setHardness(0.0f)->setLightValue(1.0f)->setStepSound(s_soundWoodFootstep));
Block* Block::s_mobSpawner = (new BlockMobSpawner(52, 65))->setHardness(5.0f)->setStepSound(s_soundMetalFootstep);
Block* Block::s_stairCompactPlanks = new BlockStairs(53, s_planks);
Block* Block::s_crate = (new BlockChest(54))->setHardness(2.5f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_redstoneWire = (new BlockRedstoneWire(55, 84))->setHardness(0.0f)->setStepSound(s_soundPowderFootstep);
Block* Block::s_oreDiamond = (new BlockOre(56, 50))->setHardness(3.0f)->setResistance(5.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_blockDiamond = (new BlockOreBlock(57, 40))->setHardness(5.0f)->setResistance(10.0f)->setStepSound(s_soundMetalFootstep);
Block* Block::s_workbench = (new BlockWorkbench(58))->setHardness(2.5f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_crops = (new BlockCrops(59, 88))->setHardness(0.0f)->setStepSound(s_soundGrassFootstep);
Block* Block::s_tilledField = (new BlockSoil(60))->setHardness(0.6f)->setStepSound(s_soundGravelFootstep);
Block* Block::s_stoneOvenIdle = (new BlockFurnace(61, false))->setHardness(3.5f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_stoneOvenActive = (new BlockFurnace(62, true))->setHardness(3.5f)->setStepSound(s_soundStoneFootstep)->setLightValue(14.0f / 16.0f);
Block* Block::s_signPost = (new BlockSign(63, NULL, true))->setHardness(1.0f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_doorWood = (new BlockDoor(64, Material::s_wood))->setHardness(3.0f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_ladder = (new BlockLadder(65, 83))->setHardness(0.4f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_minecartTrack = (new BlockMinecartTrack(66, 128))->setHardness(0.7f)->setStepSound(s_soundMetalFootstep);
Block* Block::s_stairCompactCobblestone = new BlockStairs(67, s_cobblestone);
Block* Block::s_signWall = (new BlockSign(68, NULL, false))->setHardness(1.0f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_lever = (new BlockLever(69, 96))->setHardness(0.5f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_pressurePlateStone = (new BlockPressurePlate(70, s_stone->blockIndexInTexture, EnumMobType::s_mobs))->setHardness(0.5f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_doorSteel = (new BlockDoor(71, Material::s_iron))->setHardness(5.0f)->setStepSound(s_soundMetalFootstep);
Block* Block::s_pressurePlatePlanks = (new BlockPressurePlate(72, s_planks->blockIndexInTexture, EnumMobType::s_everything))->setHardness(0.5f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_oreRedstone = (new BlockRedstoneOre(73, 51, false))->setHardness(3.0f)->setResistance(5.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_oreRedstoneGlowing = (new BlockRedstoneOre(74, 51, true))->setLightValue(10.0f / 16.0f)->setHardness(3.0f)->setResistance(5.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_torchRedstoneIdle = (new BlockRedstoneTorch(75, 115, false))->setHardness(0.0f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_torchRedstoneActive = (new BlockRedstoneTorch(76, 99, true))->setHardness(0.0f)->setLightValue(0.5f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_button = (new BlockButton(77, s_stone->blockIndexInTexture))->setHardness(0.5f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_snow = (new BlockSnow(78, 66))->setHardness(0.1f)->setStepSound(s_soundClothFootstep);
Block* Block::s_blockIce = (new BlockIce(79, 67))->setHardness(0.5f)->setLightOpacity(3)->setStepSound(s_soundGlassFootstep);
Block* Block::s_blockSnow = (new BlockSnowBlock(80, 66))->setHardness(0.2f)->setStepSound(s_soundClothFootstep);
Block* Block::s_cactus = (new BlockCactus(81, 70))->setHardness(0.4f)->setStepSound(s_soundClothFootstep);
Block* Block::s_blockClay = (new BlockClay(82, 72))->setHardness(0.6f)->setStepSound(s_soundGravelFootstep);
Block* Block::s_reed = (new BlockReed(83, 73))->setHardness(0.0f)->setStepSound(s_soundGrassFootstep);
Block* Block::s_jukebox = (new BlockJukeBox(84, 74))->setHardness(2.0f)->setResistance(10.0f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_fence = (new BlockFence(85, 4))->setHardness(2.0f)->setResistance(5.0f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_pumpkin = (new BlockPumpkin(86, 102, false))->setHardness(1.0f)->setStepSound(s_soundWoodFootstep);
Block* Block::s_bloodStone = (new BlockBloodStone(87, 103))->setHardness(0.4f)->setStepSound(s_soundStoneFootstep);
Block* Block::s_slowSand = (new BlockSlowSand(88, 104))->setHardness(0.5f)->setStepSound(s_soundSandFootstep);
Block* Block::s_lightStone = (new BlockLightStone(89, 105, Material::s_glass))->setHardness(0.3f)->setStepSound(s_soundGlassFootstep)->setLightValue(1.0f);
BlockPortal* Block::s_portal = (BlockPortal*)((BlockPortal*)(new BlockPortal(90, 14))->setHardness(-1.0f)->setStepSound(s_soundGlassFootstep)->setLightValue(12.0f / 16.0f));
Block* Block::s_pumpkinLantern = (new BlockPumpkin(91, 102, true))->setHardness(1.0f)->setStepSound(s_soundWoodFootstep)->setLightValue(1.0f);

Block::Block()
	: blockIndexInTexture(0)
	, blockID(0)
	, blockHardness(0)
	, blockResistance(0)
	, m_field_370_bf(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
	, stepSound(NULL)
	, m_field_357_bm(0)
	, m_blockMaterial(NULL)
	, slipperiness(0)
{
}

Block::Block(int var1, Material* var2)
	: blockIndexInTexture(0)
	, blockID(0)
	, blockHardness(0)
	, blockResistance(0)
	, m_field_370_bf(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
	, stepSound(NULL)
	, m_field_357_bm(0)
	, m_blockMaterial(NULL)
	, slipperiness(0)
{
		stepSound = s_soundPowderFootstep;
		m_field_357_bm = 1.0F;
		slipperiness = 0.6F;
		if(blocksList[var1] != NULL) {
			DbgPrintF("ERROR [IllegalArgumentException]: %s\n", "Slot " + toStr(var1) + " is already occupied");
		} else {
			m_blockMaterial = var2;
			blocksList[var1] = this;
			blockID = var1;
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
			s_opaqueCubeLookup[var1] = isOpaqueCube();
			s_lightOpacity[var1] = isOpaqueCube() ? 255 : 0;
			s_field_340_s[var1] = unusedMethod();
			s_isBlockContainer[var1] = false;
		}
}

Block::Block(int var1, int var2, Material* var3)
	: blockIndexInTexture(0)
	, blockID(0)
	, blockHardness(0)
	, blockResistance(0)
	, m_field_370_bf(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
	, stepSound(NULL)
	, m_field_357_bm(0)
	, m_blockMaterial(NULL)
	, slipperiness(0)
{
  // PORT: constructor delegation  // was: this(var1, var3);
		blockIndexInTexture = var2;
}

Block::~Block() {
	stepSound = NULL;
	m_blockMaterial = NULL;
}

Block* Block::setStepSound(StepSound* var1) {
		stepSound = var1;
		return this;
}

Block* Block::setLightOpacity(int var1) {
		s_lightOpacity[blockID] = var1;
		return this;
}

Block* Block::setLightValue(float var1) {
		s_lightValue[blockID] = (int)(15.0F * var1);
		return this;
}

Block* Block::setResistance(float var1) {
		blockResistance = var1 * 3.0F;
		return this;
}

bool Block::unusedMethod() {
		return false;
}

bool Block::renderAsNormalBlock() {
		return true;
}

int Block::getRenderType() {
		return 0;
}

Block* Block::setHardness(float var1) {
		blockHardness = var1;
		if(blockResistance < var1 * 5.0F) {
			blockResistance = var1 * 5.0F;
		}

		return this;
}

void Block::setTickOnLoad(bool var1) {
		s_tickOnLoad[blockID] = var1;
}

void Block::setBlockBounds(float var1, float var2, float var3, float var4, float var5, float var6) {
		m_field_370_bf = (double)var1;
		minY = (double)var2;
		minZ = (double)var3;
		maxX = (double)var4;
		maxY = (double)var5;
		maxZ = (double)var6;
}

float Block::getBlockBrightness(IBlockAccess* var1, int var2, int var3, int var4) {
		return var1->getLightBrightness(var2, var3, var4);
}

bool Block::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return var5 == 0 && minY > 0.0f ? true : (var5 == 1 && maxY < 1.0f ? true : (var5 == 2 && minZ > 0.0f ? true : (var5 == 3 && maxZ < 1.0f ? true : (var5 == 4 && m_field_370_bf > 0.0f ? true : (var5 == 5 && maxX < 1.0f ? true : !var1->isBlockOpaqueCube(var2, var3, var4))))));
}

int Block::getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return getBlockTextureFromSideAndMetadata(var5, var1->getBlockMetadata(var2, var3, var4));
}

int Block::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		return getBlockTextureFromSide(var1);
}

int Block::getBlockTextureFromSide(int var1) {
		return blockIndexInTexture;
}

AxisAlignedBB Block::getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB::getBoundingBoxFromPool((double)var2 + m_field_370_bf, (double)var3 + minY, (double)var4 + minZ, (double)var2 + maxX, (double)var3 + maxY, (double)var4 + maxZ);
}

void Block::getCollidingBoundingBoxes(World* var1, int var2, int var3, int var4, AxisAlignedBB var5, std::vector<AxisAlignedBB>& var6) {
		AxisAlignedBB var7 = getCollisionBoundingBoxFromPool(var1, var2, var3, var4);
		if(true && var5.intersectsWith(var7)) {
			var6.push_back(var7);  // PORT: value-vector, was Java List.add
		}

}

AxisAlignedBB Block::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB::getBoundingBoxFromPool((double)var2 + m_field_370_bf, (double)var3 + minY, (double)var4 + minZ, (double)var2 + maxX, (double)var3 + maxY, (double)var4 + maxZ);
}

bool Block::isOpaqueCube() {
		return true;
}

bool Block::canCollideCheck(int var1, bool var2) {
		return isCollidable();
}

bool Block::isCollidable() {
		return true;
}

void Block::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
	// TODO: implement
}

void Block::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
	// TODO: implement
}

void Block::onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5) {
	// TODO: implement
}

void Block::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
	// TODO: implement
}

int Block::tickRate() {
		return 10;
}

void Block::onBlockAdded(World* var1, int var2, int var3, int var4) {
	// TODO: implement
}

void Block::onBlockRemoval(World* var1, int var2, int var3, int var4) {
	// TODO: implement
}

int Block::quantityDropped(Random* var1) {
		return 1;
}

int Block::idDropped(int var1, Random* var2) {
		return blockID;
}

float Block::func_225_a(EntityPlayer* var1) {
		return blockHardness < 0.0F ? 0.0F : (!var1->canHarvestBlock(this) ? 1.0F / blockHardness / 100.0F : var1->getCurrentPlayerStrVsBlock(this) / blockHardness / 30.0F);
}

void Block::dropBlockAsItem(World* var1, int var2, int var3, int var4, int var5) {
		dropBlockAsItemWithChance(var1, var2, var3, var4, var5, 1.0F);
}

void Block::dropBlockAsItemWithChance(World* var1, int var2, int var3, int var4, int var5, float var6) {
		if(!var1->m_multiplayerWorld) {
			int var7 = quantityDropped(var1->rand);

			for(int var8 = 0; var8 < var7; ++var8) {
				if(var1->rand->nextFloat() <= var6) {
					int var9 = idDropped(var5, var1->rand);
					if(var9 > 0) {
						float var10 = 0.7F;
						double var11 = (double)(var1->rand->nextFloat() * var10) + (double)(1.0F - var10) * 0.5f;
						double var13 = (double)(var1->rand->nextFloat() * var10) + (double)(1.0F - var10) * 0.5f;
						double var15 = (double)(var1->rand->nextFloat() * var10) + (double)(1.0F - var10) * 0.5f;
						EntityItem* var17 = new EntityItem(var1, (double)var2 + var11, (double)var3 + var13, (double)var4 + var15, new ItemStack(var9));
						var17->m_field_805_c = 10;
						var1->entityJoinedWorld(var17);
					}
				}
			}

		}
}

float Block::func_227_a(Entity* var1) {
		return blockResistance / 5.0F;
}

MovingObjectPosition* Block::collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6) {
		setBlockBoundsBasedOnState(var1, var2, var3, var4);
		var5 = var5->addVector((double)(-var2), (double)(-var3), (double)(-var4));
		var6 = var6->addVector((double)(-var2), (double)(-var3), (double)(-var4));
		Vec3D* var7 = var5->getIntermediateWithXValue(var6, m_field_370_bf);
		Vec3D* var8 = var5->getIntermediateWithXValue(var6, maxX);
		Vec3D* var9 = var5->getIntermediateWithYValue(var6, minY);
		Vec3D* var10 = var5->getIntermediateWithYValue(var6, maxY);
		Vec3D* var11 = var5->getIntermediateWithZValue(var6, minZ);
		Vec3D* var12 = var5->getIntermediateWithZValue(var6, maxZ);
		if(!isVecInsideYZBounds(var7)) {
			var7 = NULL;
		}

		if(!isVecInsideYZBounds(var8)) {
			var8 = NULL;
		}

		if(!isVecInsideXZBounds(var9)) {
			var9 = NULL;
		}

		if(!isVecInsideXZBounds(var10)) {
			var10 = NULL;
		}

		if(!isVecInsideXYBounds(var11)) {
			var11 = NULL;
		}

		if(!isVecInsideXYBounds(var12)) {
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

bool Block::isVecInsideYZBounds(Vec3D* var1) {
		return var1 == NULL ? false : var1->yCoord >= minY && var1->yCoord <= maxY && var1->zCoord >= minZ && var1->zCoord <= maxZ;
}

bool Block::isVecInsideXZBounds(Vec3D* var1) {
		return var1 == NULL ? false : var1->xCoord >= m_field_370_bf && var1->xCoord <= maxX && var1->zCoord >= minZ && var1->zCoord <= maxZ;
}

bool Block::isVecInsideXYBounds(Vec3D* var1) {
		return var1 == NULL ? false : var1->xCoord >= m_field_370_bf && var1->xCoord <= maxX && var1->yCoord >= minY && var1->yCoord <= maxY;
}

void Block::onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4) {
	// TODO: implement
}

int Block::func_234_g() {
		return 0;
}

bool Block::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockId(var2, var3, var4);
		return var5 == 0 || blocksList[var5]->m_blockMaterial->getIsLiquid();
}

bool Block::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		return false;
}

void Block::onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5) {
	// TODO: implement
}

void Block::onBlockPlaced(World* var1, int var2, int var3, int var4, int var5) {
	// TODO: implement
}

void Block::onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
	// TODO: implement
}

void Block::velocityToAddToEntity(World* var1, int var2, int var3, int var4, Entity* var5, Vec3D* var6) {
	// TODO: implement
}

void Block::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4) {
	// TODO: implement
}

int Block::colorMultiplier(IBlockAccess* var1, int var2, int var3, int var4) {
		return 16777215;
}

bool Block::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return false;
}

bool Block::canProvidePower() {
		return false;
}

void Block::onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5) {
	// TODO: implement
}

bool Block::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5) {
		return false;
}

void Block::func_237_e() {
	// TODO: implement
}

void Block::harvestBlock(World* var1, int var2, int var3, int var4, int var5) {
		dropBlockAsItem(var1, var2, var3, var4, var5);
}

bool Block::canBlockStay(World* var1, int var2, int var3, int var4) {
		return true;
}

void Block::onBlockPlacedBy(World* var1, int var2, int var3, int var4, EntityLiving* var5) {
	// TODO: implement
}

void Block::initStatics() {
		for(int var0 = 0; var0 < 256; ++var0) {
			if(blocksList[var0] != NULL) {
				Item::itemsList[var0] = new ItemBlock(var0 - 256);
			}
		}

}
