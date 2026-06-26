#include "stdafx.h"
// Auto-generated from WorldGenClay.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenClay.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

WorldGenClay::WorldGenClay()
	: m_clayBlockId(Block::s_blockClay->blockID)
	, m_numberOfBlocks(0)
{
}

WorldGenClay::WorldGenClay(int var1)
	: m_clayBlockId(Block::s_blockClay->blockID)
	, m_numberOfBlocks(0)
{
		m_numberOfBlocks = var1;
}

WorldGenClay::~WorldGenClay() {
	// No dynamic resources
}

bool WorldGenClay::generate(World* var1, Random* var2, int var3, int var4, int var5) {
		if(var1->getBlockMaterial(var3, var4, var5) != Material::s_water) {
			return false;
		} else {
			float var6 = var2->nextFloat() * 3.14159265358979323846f;
			double var7 = (double)((float)(var3 + 8) + sinf(var6) * (float)m_numberOfBlocks / 8.0F);
			double var9 = (double)((float)(var3 + 8) - sinf(var6) * (float)m_numberOfBlocks / 8.0F);
			double var11 = (double)((float)(var5 + 8) + cosf(var6) * (float)m_numberOfBlocks / 8.0F);
			double var13 = (double)((float)(var5 + 8) - cosf(var6) * (float)m_numberOfBlocks / 8.0F);
			double var15 = (double)(var4 + var2->nextInt(3) + 2);
			double var17 = (double)(var4 + var2->nextInt(3) + 2);

			for(int var19 = 0; var19 <= m_numberOfBlocks; ++var19) {
				double var20 = var7 + (var9 - var7) * (double)var19 / (double)m_numberOfBlocks;
				double var22 = var15 + (var17 - var15) * (double)var19 / (double)m_numberOfBlocks;
				double var24 = var11 + (var13 - var11) * (double)var19 / (double)m_numberOfBlocks;
				double var26 = var2->nextDouble() * (double)m_numberOfBlocks / 16.0f;
				double var28 = (double)(sinf((float)var19 * 3.14159265358979323846f / (float)m_numberOfBlocks) + 1.0F) * var26 + 1.0f;
				double var30 = (double)(sinf((float)var19 * 3.14159265358979323846f / (float)m_numberOfBlocks) + 1.0F) * var26 + 1.0f;

				for(int var32 = (int)(var20 - var28 / 2.0f); var32 <= (int)(var20 + var28 / 2.0f); ++var32) {
					for(int var33 = (int)(var22 - var30 / 2.0f); var33 <= (int)(var22 + var30 / 2.0f); ++var33) {
						for(int var34 = (int)(var24 - var28 / 2.0f); var34 <= (int)(var24 + var28 / 2.0f); ++var34) {
							double var35 = ((double)var32 + 0.5f - var20) / (var28 / 2.0f);
							double var37 = ((double)var33 + 0.5f - var22) / (var30 / 2.0f);
							double var39 = ((double)var34 + 0.5f - var24) / (var28 / 2.0f);
							if(var35 * var35 + var37 * var37 + var39 * var39 < 1.0f) {
								int var41 = var1->getBlockId(var32, var33, var34);
								if(var41 == Block::s_sand->blockID) {
									var1->setBlock(var32, var33, var34, m_clayBlockId);
								}
							}
						}
					}
				}
			}

			return true;
		}
}
