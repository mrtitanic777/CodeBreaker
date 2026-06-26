#include "stdafx.h"
// Auto-generated from WorldGenLiquids.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenLiquids.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"

WorldGenLiquids::WorldGenLiquids()
	: m_liquidBlockId(0)
{
}

WorldGenLiquids::WorldGenLiquids(int var1)
	: m_liquidBlockId(0)
{
		m_liquidBlockId = var1;
}

WorldGenLiquids::~WorldGenLiquids() {
	// No dynamic resources
}

bool WorldGenLiquids::generate(World* var1, Random* var2, int var3, int var4, int var5) {
		if(var1->getBlockId(var3, var4 + 1, var5) != Block::s_stone->blockID) {
			return false;
		} else if(var1->getBlockId(var3, var4 - 1, var5) != Block::s_stone->blockID) {
			return false;
		} else if(var1->getBlockId(var3, var4, var5) != 0 && var1->getBlockId(var3, var4, var5) != Block::s_stone->blockID) {
			return false;
		} else {
			int var6 = 0;
			if(var1->getBlockId(var3 - 1, var4, var5) == Block::s_stone->blockID) {
				++var6;
			}

			if(var1->getBlockId(var3 + 1, var4, var5) == Block::s_stone->blockID) {
				++var6;
			}

			if(var1->getBlockId(var3, var4, var5 - 1) == Block::s_stone->blockID) {
				++var6;
			}

			if(var1->getBlockId(var3, var4, var5 + 1) == Block::s_stone->blockID) {
				++var6;
			}

			int var7 = 0;
			if(var1->getBlockId(var3 - 1, var4, var5) == 0) {
				++var7;
			}

			if(var1->getBlockId(var3 + 1, var4, var5) == 0) {
				++var7;
			}

			if(var1->getBlockId(var3, var4, var5 - 1) == 0) {
				++var7;
			}

			if(var1->getBlockId(var3, var4, var5 + 1) == 0) {
				++var7;
			}

			if(var6 == 3 && var7 == 1) {
				var1->setBlockWithNotify(var3, var4, var5, m_liquidBlockId);
				var1->m_field_4214_a = true;
				Block::blocksList[m_liquidBlockId]->updateTick(var1, var3, var4, var5, var2);
				var1->m_field_4214_a = false;
			}

			return true;
		}
}
