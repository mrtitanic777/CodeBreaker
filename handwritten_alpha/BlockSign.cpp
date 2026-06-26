#include "stdafx.h"
// Auto-generated from BlockSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSign.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "TileEntity.h"
#include "TileEntitySign.h"  // PORT: reflection (newInstance) replaced by explicit construction
#include "World.h"
#include "Material.h"
#include "BlockContainer.h"
#include "Item.h"

BlockSign::BlockSign()
	: m_signEntityClass(NULL)
	, m_isFreestanding(false)
{
}

BlockSign::BlockSign(int var1, void* var2, bool var3)
	: BlockContainer(var1, Material::s_wood)
	, m_signEntityClass(NULL)
	, m_isFreestanding(false)
{
		m_isFreestanding = var3;
		blockIndexInTexture = 4;
		m_signEntityClass = var2;
		float var4 = 0.25F;
		float var5 = 1.0F;
		setBlockBounds(0.5F - var4, 0.0F, 0.5F - var4, 0.5F + var4, var5, 0.5F + var4);
}

BlockSign::~BlockSign() {
	m_signEntityClass = NULL;
}

AxisAlignedBB BlockSign::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

AxisAlignedBB BlockSign::getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		setBlockBoundsBasedOnState(var1, var2, var3, var4);
		return BlockContainer::getSelectedBoundingBoxFromPool(var1, var2, var3, var4);
}

void BlockSign::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4) {
		if(!m_isFreestanding) {
			int var5 = var1->getBlockMetadata(var2, var3, var4);
			float var6 = 9.0F / 32.0F;
			float var7 = 25.0F / 32.0F;
			float var8 = 0.0F;
			float var9 = 1.0F;
			float var10 = 2.0F / 16.0F;
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
			if(var5 == 2) {
				setBlockBounds(var8, var6, 1.0F - var10, var9, var7, 1.0F);
			}

			if(var5 == 3) {
				setBlockBounds(var8, var6, 0.0F, var9, var7, var10);
			}

			if(var5 == 4) {
				setBlockBounds(1.0F - var10, var6, var8, 1.0F, var7, var9);
			}

			if(var5 == 5) {
				setBlockBounds(0.0F, var6, var8, var10, var7, var9);
			}

		}
}

int BlockSign::getRenderType() {
		return -1;
}

bool BlockSign::renderAsNormalBlock() {
		return false;
}

bool BlockSign::isOpaqueCube() {
		return false;
}

TileEntity* BlockSign::SetBlockEntity() {
		// PORT: Java did m_signEntityClass.newInstance() via reflection. No RTTI
		// reflection on Xbox 360 -> sign blocks always create a TileEntitySign.
		return new TileEntitySign();
}

int BlockSign::idDropped(int var1, Random* var2) {
		return Item::s_sign->shiftedIndex;
}

void BlockSign::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		bool var6 = false;
		if(m_isFreestanding) {
			if(!var1->getBlockMaterial(var2, var3 - 1, var4)->func_878_a()) {
				var6 = true;
			}
		} else {
			int var7 = var1->getBlockMetadata(var2, var3, var4);
			var6 = true;
			if(var7 == 2 && var1->getBlockMaterial(var2, var3, var4 + 1)->func_878_a()) {
				var6 = false;
			}

			if(var7 == 3 && var1->getBlockMaterial(var2, var3, var4 - 1)->func_878_a()) {
				var6 = false;
			}

			if(var7 == 4 && var1->getBlockMaterial(var2 + 1, var3, var4)->func_878_a()) {
				var6 = false;
			}

			if(var7 == 5 && var1->getBlockMaterial(var2 - 1, var3, var4)->func_878_a()) {
				var6 = false;
			}
		}

		if(var6) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}

		BlockContainer::onNeighborBlockChange(var1, var2, var3, var4, var5);
}
