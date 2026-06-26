#include "stdafx.h"
// Auto-generated from BlockGrass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockGrass.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockGrass::BlockGrass()
{
}

BlockGrass::BlockGrass(int var1)
	: Block(2, Material::s_ground)
{
		blockIndexInTexture = 3;
		setTickOnLoad(true);
}

BlockGrass::~BlockGrass() {
	// No dynamic resources
}

int BlockGrass::getBlockTextureFromSide(int var1) {
		return var1 == 1 ? 0 : (var1 == 0 ? 2 : 3);
}

void BlockGrass::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var1->getBlockLightValue(var2, var3 + 1, var4) < 4 && var1->getBlockMaterial(var2, var3 + 1, var4)->getCanBlockGrass()) {
			if(var5->nextInt(4) == 0) {
				var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
			}
		} else {
			if(var1->getBlockLightValue(var2, var3 + 1, var4) >= 9) {
				var2 = var2 + var5->nextInt(3) - 1;
				var3 = var3 + var5->nextInt(5) - 3;
				var4 = var4 + var5->nextInt(3) - 1;
				if(var1->getBlockId(var2, var3, var4) == Block::s_dirt->blockID && var1->getBlockLightValue(var2, var3 + 1, var4) >= 4 && !var1->getBlockMaterial(var2, var3 + 1, var4)->getCanBlockGrass()) {
					var1->setBlockWithNotify(var2, var3, var4, Block::s_grass->blockID);
				}
			}

		}
}

int BlockGrass::idDropped(int var1) {
		return Block::s_dirt->idDropped(0);
}
