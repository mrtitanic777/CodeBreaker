#include "stdafx.h"
// Auto-generated from BlockSource.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSource.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockSource::BlockSource()
	: m_fluid(0)
{
}

BlockSource::BlockSource(int var1, int var2)
	: Block(var1, Block::blocksList[var2].blockIndexInTexture, Material::s_water)
	, m_fluid(0)
{
		m_fluid = var2;
		setTickOnLoad(true);
}

BlockSource::~BlockSource() {
	// No dynamic resources
}

void BlockSource::onBlockAdded(World* var1, int var2, int var3, int var4) {
		Block::onBlockAdded(var1, var2, var3, var4);
		if(var1->getBlockId(var2 - 1, var3, var4) == 0) {
			var1->setBlockWithNotify(var2 - 1, var3, var4, m_fluid);
		}

		if(var1->getBlockId(var2 + 1, var3, var4) == 0) {
			var1->setBlockWithNotify(var2 + 1, var3, var4, m_fluid);
		}

		if(var1->getBlockId(var2, var3, var4 - 1) == 0) {
			var1->setBlockWithNotify(var2, var3, var4 - 1, m_fluid);
		}

		if(var1->getBlockId(var2, var3, var4 + 1) == 0) {
			var1->setBlockWithNotify(var2, var3, var4 + 1, m_fluid);
		}

}

void BlockSource::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		Block::updateTick(var1, var2, var3, var4, var5);
		if(var1->getBlockId(var2 - 1, var3, var4) == 0) {
			var1->setBlockWithNotify(var2 - 1, var3, var4, m_fluid);
		}

		if(var1->getBlockId(var2 + 1, var3, var4) == 0) {
			var1->setBlockWithNotify(var2 + 1, var3, var4, m_fluid);
		}

		if(var1->getBlockId(var2, var3, var4 - 1) == 0) {
			var1->setBlockWithNotify(var2, var3, var4 - 1, m_fluid);
		}

		if(var1->getBlockId(var2, var3, var4 + 1) == 0) {
			var1->setBlockWithNotify(var2, var3, var4 + 1, m_fluid);
		}

}
