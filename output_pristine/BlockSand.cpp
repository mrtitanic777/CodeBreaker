#include "stdafx.h"
// Auto-generated from BlockSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSand.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "EntityItem.h"
#include "ItemStack.h"
#include "Material.h"

BlockSand::BlockSand()
	: rand(new Random())
{
}

BlockSand::BlockSand(int var1, int var2)
	: Block(var1, var2, Material::s_sand)
	, rand(new Random())
{
}

BlockSand::~BlockSand() {
	rand = NULL;
}

void BlockSand::onBlockAdded(World* var1, int var2, int var3, int var4) {
		tryToFall(var1, var2, var3, var4);
}

void BlockSand::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		tryToFall(var1, var2, var3, var4);
}

void BlockSand::tryToFall(World* var1, int var2, int var3, int var4) {
		int var5 = var3;

		while(true) {
			int var8 = var5 - 1;
			int var6 = var1->getBlockId(var2, var8, var4);
			bool var10000;
			if(var6 == 0) {
				var10000 = true;
			} else if(var6 == Block::s_fire->blockID) {
				var10000 = true;
			} else {
				Material var10 = Block::blocksList[var6].material;
				var10000 = var10 == Material::s_water ? true : var10 == Material::s_lava;
			}

			if(!var10000 || var5 < 0) {
				if(var5 < 0) {
					var1->setTileNoUpdate(var2, var3, var4, 0);
				}

				if(var5 != var3) {
					var6 = var1->getBlockId(var2, var5, var4);
					if(var6 > 0 && Block::blocksList[var6].material != Material::s_air) {
						var1->setTileNoUpdate(var2, var5, var4, 0);
					}

					var1->swap(var2, var3, var4, var2, var5, var4);
				}

				return;
			}

			--var5;
			if(var1->getBlockId(var2, var5, var4) == Block::s_fire->blockID) {
				var1->setBlock(var2, var5, var4, 0);
			}
		}
}

bool BlockSand::onBlockPlaced(World* var1, float var2, float var3, float var4) {
		int var5 = Block::s_glass->blockID;
		int var6 = rand->nextInt(3) + 1;

		for(int var7 = 0; var7 < var6; ++var7) {
			if(var1->m_random->nextFloat() <= 1.0F) {
				float var8 = var1->m_random->nextFloat() * 0.7F + 0.15F;
				float var9 = var1->m_random->nextFloat() * 0.7F + 0.15F;
				float var10 = var1->m_random->nextFloat() * 0.7F + 0.15F;
				EntityItem* var11 = new EntityItem(var1, var2 + var8, var3 + var9, var4 + var10, new ItemStack(var5));
				var11->m_delayBeforeCanPickup = 10;
				var1->spawnEntityInWorld(var11);
			}
		}

		return true;
}
