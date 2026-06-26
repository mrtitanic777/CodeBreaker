#include "stdafx.h"
// Auto-generated from Block.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "EntityPlayer.h"
#include "Material.h"
#include "MovingObjectPosition.h"
#include "Random.h"
#include "StepSound.h"
#include "Vec3D.h"
#include "World.h"
#include "EntityItem.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "StepSoundGlass.h"
#include "StepSoundGravel.h"

StepSound* Block::s_soundPowderFootstep = new StepSound("stone", 1.0f, 1.0f);
StepSound* Block::s_soundWoodFootstep = new StepSound("wood", 1.0f, 1.0f);
StepSound* Block::s_soundGravelFootstep = new StepSound("gravel", 1.0f, 1.0f);
StepSound* Block::s_soundGrassFootstep = new StepSound("grass", 1.0f, 1.0f);
StepSound* Block::s_soundStoneFootstep = new StepSound("stone", 1.0f, 1.0f);
StepSound* Block::s_soundMetalFootstep = new StepSound("stone", 1.0f, 1.5f);
StepSound* Block::s_soundGlassFootstep = new StepSoundGlass("stone", 1.0f, 1.0f);
StepSound* Block::s_soundClothFootstep = new StepSound("cloth", 1.0f, 1.0f);
StepSound* Block::s_soundSandFootstep = new StepSoundGravel("sand", 1.0f, 1.0f);
Block* Block::blocksList = new Block[256];
bool* Block::s_tickOnLoad = new bool[256];
bool* Block::s_opaqueCubeLookup = new bool[256];
int* Block::s_lightOpacity = new int[256];
bool* Block::s_canBlockGrass = new bool[256];
bool* Block::s_isBlockFluid = new bool[256];
int* Block::s_lightValue = new int[256];
Block* Block::s_stone;
BlockGrass* Block::s_grass;
Block* Block::s_dirt;
Block* Block::s_cobblestone;
Block* Block::s_planks;
Block* Block::s_sapling;
Block* Block::s_bedrock;
Block* Block::s_waterMoving;
Block* Block::s_waterStill;
Block* Block::s_lavaMoving;
Block* Block::s_lavaStill;
Block* Block::s_sand;
Block* Block::s_gravel;
Block* Block::s_oreGold;
Block* Block::s_oreIron;
Block* Block::s_oreCoal;
Block* Block::s_wood;
Block* Block::s_leaves;
Block* Block::s_sponge;
Block* Block::s_glass;
Block* Block::s_clothRed;
Block* Block::s_clothOrange;
Block* Block::s_clothYellow;
Block* Block::s_clothChartreuse;
Block* Block::s_clothGreen;
Block* Block::s_clothSpringGreen;
Block* Block::s_clothCyan;
Block* Block::s_clothCapri;
Block* Block::s_clothUltramarine;
Block* Block::s_clothViolet;
Block* Block::s_clothPurple;
Block* Block::s_clothMagenta;
Block* Block::s_clothRose;
Block* Block::s_clothDarkGray;
Block* Block::s_clothGray;
Block* Block::s_clothWhite;
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
Block* Block::s_torch;
BlockFire* Block::s_fire;
Block* Block::s_waterSource;
Block* Block::s_lavaSource;
Block* Block::s_crate;
Block* Block::s_cog;
Block* Block::s_oreDiamond;
Block* Block::s_blockDiamond;
Block* Block::s_workbench;
Block* Block::s_crops;
Block* Block::s_tilledField;

Block::Block()
	: blockIndexInTexture(0)
	, blockID(0)
	, m_hardness(0)
	, m_resistance(0)
	, minX(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
	, stepSound(NULL)
	, blockParticleGravity(0)
	, material(NULL)
	, m_var10000((new BlockStone(1, 1))->setHardness(1.5f).setResistance(10.0f))
	, m_var1(soundStoneFootstep)
	, m_var0(var10000)
{
}

Block::Block(int var1, Material* var2)
	: blockIndexInTexture(0)
	, blockID(0)
	, m_hardness(0)
	, m_resistance(0)
	, minX(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
	, stepSound(NULL)
	, blockParticleGravity(0)
	, material(NULL)
	, m_var10000((new BlockStone(1, 1))->setHardness(1.5f).setResistance(10.0f))
	, m_var1(soundStoneFootstep)
	, m_var0(var10000)
{
		stepSound = s_soundPowderFootstep;
		blockParticleGravity = 1.0F;
		if(blocksList[var1] != NULL) {
			DbgPrintF("ERROR [IllegalArgumentException]: %s\n", "Slot " + var1 + " is already occupied by " + blocksList[var1] + " when adding " + this);
		} else {
			material = var2;
			blocksList[var1] = this;
			blockID = var1;
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
			s_opaqueCubeLookup[var1] = isOpaqueCube();
			s_lightOpacity[var1] = isOpaqueCube() ? 255 : 0;
			s_canBlockGrass[var1] = renderAsNormalBlock();
			s_isBlockFluid[var1] = false;
		}
}

Block::Block(int var1, int var2, Material* var3)
	: blockIndexInTexture(0)
	, blockID(0)
	, m_hardness(0)
	, m_resistance(0)
	, minX(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
	, stepSound(NULL)
	, blockParticleGravity(0)
	, material(NULL)
	, m_var10000((new BlockStone(1, 1))->setHardness(1.5f).setResistance(10.0f))
	, m_var1(soundStoneFootstep)
	, m_var0(var10000)
{
  // PORT: constructor delegation  // was: this(var1, var3);
		blockIndexInTexture = var2;
}

Block::~Block() {
	stepSound = NULL;
	material = NULL;
	m_var10000 = NULL;
	m_var1 = NULL;
	m_var0 = NULL;
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
		m_resistance = var1 * 3.0F;
		return this;
}

bool Block::renderAsNormalBlock() {
		return true;
}

int Block::getRenderType() {
		return 0;
}

Block* Block::setHardness(float var1) {
		m_hardness = var1;
		if(m_resistance < var1 * 5.0F) {
			m_resistance = var1 * 5.0F;
		}

		return this;
}

void Block::setTickOnLoad(bool var1) {
		s_tickOnLoad[blockID] = var1;
}

void Block::setBlockBounds(float var1, float var2, float var3, float var4, float var5, float var6) {
		minX = var1;
		minY = var2;
		minZ = var3;
		maxX = var4;
		maxY = var5;
		maxZ = var6;
}

float Block::getBlockBrightness(World* var1, int var2, int var3, int var4) {
		return var1->getLightBrightness(var2, var3, var4);
}

bool Block::shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5) {
		return !var1->isBlockNormalCube(var2, var3, var4);
}

int Block::getBlockTexture(World* var1, int var2, int var3, int var4, int var5) {
		return getBlockTextureFromSideAndMetadata(var5, var1->getBlockMetadata(var2, var3, var4));
}

int Block::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		return getBlockTextureFromSide(var1);
}

int Block::getBlockTextureFromSide(int var1) {
		return blockIndexInTexture;
}

AxisAlignedBB Block::getSelectedBoundingBoxFromPool(int var1, int var2, int var3) {
		return AxisAlignedBB((float)var1 + minX, (float)var2 + minY, (float)var3 + minZ, (float)var1 + maxX, (float)var2 + maxY, (float)var3 + maxZ);
}

AxisAlignedBB Block::getCollisionBoundingBoxFromPool(int var1, int var2, int var3) {
		return AxisAlignedBB((float)var1 + minX, (float)var2 + minY, (float)var3 + minZ, (float)var1 + maxX, (float)var2 + maxY, (float)var3 + maxZ);
}

bool Block::isOpaqueCube() {
		return true;
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
		return 5;
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

int Block::idDropped(int var1) {
		return blockID;
}

float Block::blockStrength(EntityPlayer* var1) {
		if(m_hardness < 0.0F) {
			return 0.0F;
		} else {
			InventoryPlayer var3 = ((EntityPlayer*)var1).inventory;
			bool var10000;
			ItemStack var6;
			if(material != Material::s_rock && material != Material::s_iron) {
				m_var10000 = true;
			} else {
				var6 = var3.getStackInSlot(var3.m_currentItem);
				m_var10000 = var6 != NULL ? Item::itemsList[var6.itemID].canHarvestBlock(this) : false;
			}

			if(!m_var10000) {
				return 1.0F / m_hardness / 100.0F;
			} else {
				InventoryPlayer var4 = ((EntityPlayer*)var1).inventory;
				float var5 = 1.0F;
				if(var4.m_mainInventory[var4.m_currentItem] != NULL) {
					ItemStack var10001 = var4.m_mainInventory[var4.m_currentItem];
					var6 = var10001;
					var5 = 1.0F * var6.getItem().getStrVsBlock(this);
				}

				float var7 = var5;
				if(((EntityPlayer*)var1).isInsideOfMaterial()) {
					var7 = var5 / 5.0F;
				}

				if(!((EntityPlayer*)var1).onGround) {
					var7 /= 5.0F;
				}

				return var7 / m_hardness / 30.0F;
			}
		}
}

void Block::dropBlockAsItem(World* var1, int var2, int var3, int var4, int var5) {
		dropBlockAsItemWithChance(var1, var2, var3, var4, var5, 1.0F);
}

void Block::dropBlockAsItemWithChance(World* var1, int var2, int var3, int var4, int var5, float var6) {
		int var7 = quantityDropped(var1->m_random);

		for(int var8 = 0; var8 < var7; ++var8) {
			if(var1->m_random->nextFloat() <= var6) {
				int var9 = idDropped(var5);
				if(var9 > 0) {
					float var10 = var1->m_random->nextFloat() * 0.7F + 0.15F;
					float var11 = var1->m_random->nextFloat() * 0.7F + 0.15F;
					float var12 = var1->m_random->nextFloat() * 0.7F + 0.15F;
					EntityItem* var13 = new EntityItem(var1, (float)var2 + var10, (float)var3 + var11, (float)var4 + var12, ItemStack(var9));
					var13->m_delayBeforeCanPickup = 10;
					var1->spawnEntityInWorld(var13);
				}
			}
		}

}

float Block::getExplosionResistance() {
		return m_resistance / 5.0F;
}

MovingObjectPosition Block::collisionRayTrace(World* var1, int var2, int var3, int var4, const Vec3D& var5, const Vec3D& var6) {
		var5 = var5.addVector((float)(-var2), (float)(-var3), (float)(-var4));
		var6 = var6.addVector((float)(-var2), (float)(-var3), (float)(-var4));
		Vec3D var12 = var5.getIntermediateWithXValue(var6, minX);
		Vec3D var7 = var5.getIntermediateWithXValue(var6, maxX);
		Vec3D var8 = var5.getIntermediateWithYValue(var6, minY);
		Vec3D var9 = var5.getIntermediateWithYValue(var6, maxY);
		Vec3D var10 = var5.getIntermediateWithZValue(var6, minZ);
		var6 = var5.getIntermediateWithZValue(var6, maxZ);
		if(!isVecInsideYZBounds(var12)) {
			var12 = NULL;
		}

		if(!isVecInsideYZBounds(var7)) {
			var7 = NULL;
		}

		if(!isVecInsideXZBounds(var8)) {
			var8 = NULL;
		}

		if(!isVecInsideXZBounds(var9)) {
			var9 = NULL;
		}

		if(!isVecInsideXYBounds(var10)) {
			var10 = NULL;
		}

		if(!isVecInsideXYBounds(var6)) {
			var6 = NULL;
		}

		Vec3D var11 = NULL;
		if(var12 != NULL) {
			var11 = var12;
		}

		if(var7 != NULL && (var11 == NULL || var5.distance(var7) < var5.distance(var11))) {
			var11 = var7;
		}

		if(var8 != NULL && (var11 == NULL || var5.distance(var8) < var5.distance(var11))) {
			var11 = var8;
		}

		if(var9 != NULL && (var11 == NULL || var5.distance(var9) < var5.distance(var11))) {
			var11 = var9;
		}

		if(var10 != NULL && (var11 == NULL || var5.distance(var10) < var5.distance(var11))) {
			var11 = var10;
		}

		if(var6 != NULL && (var11 == NULL || var5.distance(var6) < var5.distance(var11))) {
			var11 = var6;
		}

		if(var11 == NULL) {
			return NULL;
		} else {
			unsigned char var13 = -1;
			if(var11 == var12) {
				var13 = 4;
			}

			if(var11 == var7) {
				var13 = 5;
			}

			if(var11 == var8) {
				var13 = 0;
			}

			if(var11 == var9) {
				var13 = 1;
			}

			if(var11 == var10) {
				var13 = 2;
			}

			if(var11 == var6) {
				var13 = 3;
			}

			return MovingObjectPosition(var2, var3, var4, var13, var11.addVector((float)var2, (float)var3, (float)var4));
		}
}

bool Block::isVecInsideYZBounds(const Vec3D& var1) {
		return var1 == NULL ? false : var1.m_yCoord >= minY && var1.m_yCoord <= maxY && var1.m_zCoord >= minZ && var1.m_zCoord <= maxZ;
}

bool Block::isVecInsideXZBounds(const Vec3D& var1) {
		return var1 == NULL ? false : var1.m_xCoord >= minX && var1.m_xCoord <= maxX && var1.m_zCoord >= minZ && var1.m_zCoord <= maxZ;
}

bool Block::isVecInsideXYBounds(const Vec3D& var1) {
		return var1 == NULL ? false : var1.m_xCoord >= minX && var1.m_xCoord <= maxX && var1.m_yCoord >= minY && var1.m_yCoord <= maxY;
}

void Block::onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4) {
	// TODO: implement
}

int Block::getRenderBlockPass() {
		return 0;
}

bool Block::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return true;
}

bool Block::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		return false;
}

bool Block::onBlockPlaced(World* var1, float var2, float var3, float var4) {
		return false;
}

void Block::onEntityWalking(World* var1, int var2, int var3, int var4) {
	// TODO: implement
}

void Block::onBlockPlaced(World* var1, int var2, int var3, int var4, int var5) {
	// TODO: implement
}
