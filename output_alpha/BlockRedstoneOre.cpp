#include "stdafx.h"
// Auto-generated from BlockRedstoneOre.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockRedstoneOre.h"
#include "Common.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "Item.h"

BlockRedstoneOre::BlockRedstoneOre()
	: m_field_468_a(false)
{
}

BlockRedstoneOre::BlockRedstoneOre(int var1, int var2, bool var3)
	: Block(var1, var2, Material::s_rock)
	, m_field_468_a(false)
{
		if(var3) {
			setTickOnLoad(true);
		}

		m_field_468_a = var3;
}

BlockRedstoneOre::~BlockRedstoneOre() {
	// No dynamic resources
}

int BlockRedstoneOre::tickRate() {
		return 30;
}

void BlockRedstoneOre::onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		func_320_h(var1, var2, var3, var4);
		Block::onBlockClicked(var1, var2, var3, var4, var5);
}

void BlockRedstoneOre::onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5) {
		func_320_h(var1, var2, var3, var4);
		Block::onEntityWalking(var1, var2, var3, var4, var5);
}

bool BlockRedstoneOre::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		func_320_h(var1, var2, var3, var4);
		return Block::blockActivated(var1, var2, var3, var4, var5);
}

void BlockRedstoneOre::func_320_h(World* var1, int var2, int var3, int var4) {
		func_319_i(var1, var2, var3, var4);
		if(blockID == Block::s_oreRedstone->blockID) {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_oreRedstoneGlowing->blockID);
		}

}

void BlockRedstoneOre::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(blockID == Block::s_oreRedstoneGlowing->blockID) {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_oreRedstone->blockID);
		}

}

int BlockRedstoneOre::idDropped(int var1, Random* var2) {
		return Item::s_redstone->shiftedIndex;
}

int BlockRedstoneOre::quantityDropped(Random* var1) {
		return 4 + var1->nextInt(2);
}

void BlockRedstoneOre::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(m_field_468_a) {
			func_319_i(var1, var2, var3, var4);
		}

}

void BlockRedstoneOre::func_319_i(World* var1, int var2, int var3, int var4) {
		Random* var5 = var1->rand;
		double var6 = 1.0f / 16.0f;

		for(int var8 = 0; var8 < 6; ++var8) {
			double var9 = (double)((float)var2 + var5->nextFloat());
			double var11 = (double)((float)var3 + var5->nextFloat());
			double var13 = (double)((float)var4 + var5->nextFloat());
			if(var8 == 0 && !var1->isBlockOpaqueCube(var2, var3 + 1, var4)) {
				var11 = (double)(var3 + 1) + var6;
			}

			if(var8 == 1 && !var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
				var11 = (double)(var3 + 0) - var6;
			}

			if(var8 == 2 && !var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
				var13 = (double)(var4 + 1) + var6;
			}

			if(var8 == 3 && !var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
				var13 = (double)(var4 + 0) - var6;
			}

			if(var8 == 4 && !var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
				var9 = (double)(var2 + 1) + var6;
			}

			if(var8 == 5 && !var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
				var9 = (double)(var2 + 0) - var6;
			}

			if(var9 < (double)var2 || var9 > (double)(var2 + 1) || var11 < 0.0f || var11 > (double)(var3 + 1) || var13 < (double)var4 || var13 > (double)(var4 + 1)) {
				var1->spawnParticle("reddust", var9, var11, var13, 0.0f, 0.0f, 0.0f);
			}
		}

}
