#include "stdafx.h"
// Auto-generated from BlockStep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockStep.h"
#include "Common.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockStep::BlockStep()
	: m_blockType(false)
{
}

BlockStep::BlockStep(int var1, bool var2)
	: Block(var1, 6, Material::s_rock)
	, m_blockType(false)
{
		m_blockType = var2;
		if(!var2) {
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 0.5F, 1.0F);
		}

		setLightOpacity(255);
}

BlockStep::~BlockStep() {
	// No dynamic resources
}

int BlockStep::getBlockTextureFromSide(int var1) {
		return var1 <= 1 ? 6 : 5;
}

bool BlockStep::isOpaqueCube() {
		return m_blockType;
}

void BlockStep::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(this == Block::s_stairSingle) {
		}
}

void BlockStep::onBlockAdded(World* var1, int var2, int var3, int var4) {
		if(this != Block::s_stairSingle) {
			Block::onBlockAdded(var1, var2, var3, var4);
		}

		int var5 = var1->getBlockId(var2, var3 - 1, var4);
		if(var5 == stairSingle.blockID) {
			var1->setBlockWithNotify(var2, var3, var4, 0);
			var1->setBlockWithNotify(var2, var3 - 1, var4, Block::s_stairDouble->blockID);
		}

}

int BlockStep::idDropped(int var1) {
		return Block::s_stairSingle->blockID;
}

bool BlockStep::renderAsNormalBlock() {
		return m_blockType;
}

bool BlockStep::shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5) {
		return var5 == 1 ? true : (!Block::shouldSideBeRendered(var1, var2, var3, var4, var5) ? false : (var5 == 0 ? true : var1->getBlockId(var2, var3, var4) != blockID));
}
