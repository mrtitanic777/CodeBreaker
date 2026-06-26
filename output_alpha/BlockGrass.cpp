#include "stdafx.h"
// Auto-generated from BlockGrass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockGrass.h"
#include "Common.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "WorldChunkManager.h"

BlockGrass::BlockGrass()
{
}

BlockGrass::BlockGrass(int var1)
	: Block(var1, Material::s_ground)
{
		blockIndexInTexture = 3;
		setTickOnLoad(true);
}

BlockGrass::~BlockGrass() {
	// No dynamic resources
}

int BlockGrass::getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		if(var5 == 1) {
			return 0;
		} else if(var5 == 0) {
			return 2;
		} else {
			Material* var6 = var1->getBlockMaterial(var2, var3 + 1, var4);
			return var6 != Material::s_snow && var6 != Material::s_builtSnow ? 3 : 68;
		}
}

int BlockGrass::colorMultiplier(IBlockAccess* var1, int var2, int var3, int var4) {
		// PORT: Java used ColorizerGrass.getGrassColor(temperature, humidity)
		// which samples grasscolor.png (256x256) by biome temp/humidity. That
		// colormap asset is not available in this port, so we read the biome
		// temp/humidity (so the call still primes the chunk-manager buffers)
		// and return the canonical default grass tint (plains biome).
		var1->func_4075_a()->func_4069_a(var2, var4, 1, 1);
		// double var5 = var1->func_4075_a()->m_temperature[0];
		// double var7 = var1->func_4075_a()->m_humidity[0];
		return 0x91BD59;  // default grass color (9551193)
}

void BlockGrass::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var1->getBlockLightValue(var2, var3 + 1, var4) < 4 && var1->getBlockMaterial(var2, var3 + 1, var4)->getCanBlockGrass()) {
			if(var5->nextInt(4) != 0) {
				return;
			}

			var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
		} else if(var1->getBlockLightValue(var2, var3 + 1, var4) >= 9) {
			int var6 = var2 + var5->nextInt(3) - 1;
			int var7 = var3 + var5->nextInt(5) - 3;
			int var8 = var4 + var5->nextInt(3) - 1;
			if(var1->getBlockId(var6, var7, var8) == Block::s_dirt->blockID && var1->getBlockLightValue(var6, var7 + 1, var8) >= 4 && !var1->getBlockMaterial(var6, var7 + 1, var8)->getCanBlockGrass()) {
				var1->setBlockWithNotify(var6, var7, var8, Block::s_grass->blockID);
			}
		}

}

int BlockGrass::idDropped(int var1, Random* var2) {
		return Block::s_dirt->idDropped(0, var2);
}
