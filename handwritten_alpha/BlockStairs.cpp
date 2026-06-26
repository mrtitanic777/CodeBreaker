#include "stdafx.h"
// Auto-generated from BlockStairs.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockStairs.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "Vec3D.h"
#include "World.h"
#include "Block.h"

BlockStairs::BlockStairs()
	: m_modelBlock(NULL)
{
}

BlockStairs::BlockStairs(int var1, Block* var2)
	: Block(var1, var2->blockIndexInTexture, var2->m_blockMaterial)
	, m_modelBlock(NULL)
{
		m_modelBlock = var2;
		setHardness(var2->blockHardness);
		setResistance(var2->blockResistance / 3.0F);
		setStepSound(var2->stepSound);
}

BlockStairs::~BlockStairs() {
	m_modelBlock = NULL;
}

void BlockStairs::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4) {
		setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
}

AxisAlignedBB BlockStairs::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return Block::getCollisionBoundingBoxFromPool(var1, var2, var3, var4);
}

bool BlockStairs::isOpaqueCube() {
		return false;
}

bool BlockStairs::renderAsNormalBlock() {
		return false;
}

int BlockStairs::getRenderType() {
		return 10;
}

bool BlockStairs::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return Block::shouldSideBeRendered(var1, var2, var3, var4, var5);
}

void BlockStairs::getCollidingBoundingBoxes(World* var1, int var2, int var3, int var4, AxisAlignedBB var5, std::vector<AxisAlignedBB>& var6) {
		int var7 = var1->getBlockMetadata(var2, var3, var4);
		if(var7 == 0) {
			setBlockBounds(0.0F, 0.0F, 0.0F, 0.5F, 0.5F, 1.0F);
			Block::getCollidingBoundingBoxes(var1, var2, var3, var4, var5, var6);
			setBlockBounds(0.5F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
			Block::getCollidingBoundingBoxes(var1, var2, var3, var4, var5, var6);
		} else if(var7 == 1) {
			setBlockBounds(0.0F, 0.0F, 0.0F, 0.5F, 1.0F, 1.0F);
			Block::getCollidingBoundingBoxes(var1, var2, var3, var4, var5, var6);
			setBlockBounds(0.5F, 0.0F, 0.0F, 1.0F, 0.5F, 1.0F);
			Block::getCollidingBoundingBoxes(var1, var2, var3, var4, var5, var6);
		} else if(var7 == 2) {
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 0.5F, 0.5F);
			Block::getCollidingBoundingBoxes(var1, var2, var3, var4, var5, var6);
			setBlockBounds(0.0F, 0.0F, 0.5F, 1.0F, 1.0F, 1.0F);
			Block::getCollidingBoundingBoxes(var1, var2, var3, var4, var5, var6);
		} else if(var7 == 3) {
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.5F);
			Block::getCollidingBoundingBoxes(var1, var2, var3, var4, var5, var6);
			setBlockBounds(0.0F, 0.0F, 0.5F, 1.0F, 0.5F, 1.0F);
			Block::getCollidingBoundingBoxes(var1, var2, var3, var4, var5, var6);
		}

		setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
}

void BlockStairs::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		m_modelBlock->randomDisplayTick(var1, var2, var3, var4, var5);
}

void BlockStairs::onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		m_modelBlock->onBlockClicked(var1, var2, var3, var4, var5);
}

void BlockStairs::onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5) {
		m_modelBlock->onBlockDestroyedByPlayer(var1, var2, var3, var4, var5);
}

float BlockStairs::getBlockBrightness(IBlockAccess* var1, int var2, int var3, int var4) {
		return m_modelBlock->getBlockBrightness(var1, var2, var3, var4);
}

float BlockStairs::func_227_a(Entity* var1) {
		return m_modelBlock->func_227_a(var1);
}

int BlockStairs::func_234_g() {
		return m_modelBlock->func_234_g();
}

int BlockStairs::idDropped(int var1, Random* var2) {
		return m_modelBlock->idDropped(var1, var2);
}

int BlockStairs::quantityDropped(Random* var1) {
		return m_modelBlock->quantityDropped(var1);
}

int BlockStairs::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		return m_modelBlock->getBlockTextureFromSideAndMetadata(var1, var2);
}

int BlockStairs::getBlockTextureFromSide(int var1) {
		return m_modelBlock->getBlockTextureFromSide(var1);
}

int BlockStairs::getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return m_modelBlock->getBlockTexture(var1, var2, var3, var4, var5);
}

int BlockStairs::tickRate() {
		return m_modelBlock->tickRate();
}

AxisAlignedBB BlockStairs::getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return m_modelBlock->getSelectedBoundingBoxFromPool(var1, var2, var3, var4);
}

void BlockStairs::velocityToAddToEntity(World* var1, int var2, int var3, int var4, Entity* var5, Vec3D* var6) {
		m_modelBlock->velocityToAddToEntity(var1, var2, var3, var4, var5, var6);
}

bool BlockStairs::isCollidable() {
		return m_modelBlock->isCollidable();
}

bool BlockStairs::canCollideCheck(int var1, bool var2) {
		return m_modelBlock->canCollideCheck(var1, var2);
}

bool BlockStairs::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return m_modelBlock->canPlaceBlockAt(var1, var2, var3, var4);
}

void BlockStairs::onBlockAdded(World* var1, int var2, int var3, int var4) {
		onNeighborBlockChange(var1, var2, var3, var4, 0);
		m_modelBlock->onBlockAdded(var1, var2, var3, var4);
}

void BlockStairs::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		m_modelBlock->onBlockRemoval(var1, var2, var3, var4);
}

void BlockStairs::dropBlockAsItemWithChance(World* var1, int var2, int var3, int var4, int var5, float var6) {
		m_modelBlock->dropBlockAsItemWithChance(var1, var2, var3, var4, var5, var6);
}

void BlockStairs::dropBlockAsItem(World* var1, int var2, int var3, int var4, int var5) {
		m_modelBlock->dropBlockAsItem(var1, var2, var3, var4, var5);
}

void BlockStairs::onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5) {
		m_modelBlock->onEntityWalking(var1, var2, var3, var4, var5);
}

void BlockStairs::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		m_modelBlock->updateTick(var1, var2, var3, var4, var5);
}

bool BlockStairs::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		return m_modelBlock->blockActivated(var1, var2, var3, var4, var5);
}

void BlockStairs::onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4) {
		m_modelBlock->onBlockDestroyedByExplosion(var1, var2, var3, var4);
}

void BlockStairs::onBlockPlacedBy(World* var1, int var2, int var3, int var4, EntityLiving* var5) {
		int var6 = (int)floor((double)(var5->rotationYaw * 4.0F / 360.0F) + 0.5f) & 3;
		if(var6 == 0) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 2);
		}

		if(var6 == 1) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 1);
		}

		if(var6 == 2) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 3);
		}

		if(var6 == 3) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 0);
		}

}
