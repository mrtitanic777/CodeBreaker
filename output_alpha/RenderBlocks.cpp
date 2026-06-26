#include "stdafx.h"
// Hand-written PORT override for RenderBlocks.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Block-geometry tessellation. Started from the converter's output_alpha/RenderBlocks.cpp
// (Alpha's real per-block logic: standard cube, fluids, torch, fire, redstone, rails,
// ladder, reed, crops, cactus, fence, stairs, door, lever) and fixed to compile against
// Alpha's RenderBlocks.h, using the indev RenderBlocks.cpp as the how-to-port guide.
//
// Fixes applied vs the raw converter output:
//   * Tesselator::getInstance() is taken by REFERENCE everywhere. The converter emitted
//     `Tesselator varN = Tesselator::getInstance();` (by value) -- TesselatorVertex
//     m_vertices[262144] is ~9.4MB, so a by-value copy blows the 256KB Xbox stack on the
//     first chunk mesh. Same fix indev made.
//   * renderBlockLever's `Vec3D* var21 = new Vec3D[8]` (array of values, assigned pointers
//     from createVector + used by value) -> `Vec3D** var21 = new Vec3D*[8]`, member access
//     via `->`. (Matches Vec3D.h: createVector returns Vec3D*, rotate* are instance methods.)
//   * `var1 == Block::s_grass` -> cast (s_grass is BlockGrass*).
//   * Tesselator has no setTranslationF (the cactus/render-type-13 1px inset). Stubbed with
//     // PORT: -- the cube still renders at full bounds; the inset is a cosmetic offset.

#include "RenderBlocks.h"
#include "Common.h"
#include "Block.h"
#include "IBlockAccess.h"
#include "Material.h"
#include "World.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Vec3D.h"
#include "BlockDoor.h"
#include "BlockFire.h"
#include "BlockFluids.h"
#include "BlockRedstoneWire.h"
#include <math.h>

RenderBlocks::RenderBlocks(IBlockAccess* var1)
	: m_blockAccess(NULL)
	, m_overrideBlockTexture(-1)
	, m_flipTexture(false)
	, m_renderAllFaces(false)
{
		m_blockAccess = var1;
}

RenderBlocks::RenderBlocks()
	: m_blockAccess(NULL)
	, m_overrideBlockTexture(-1)
	, m_flipTexture(false)
	, m_renderAllFaces(false)
{
}

RenderBlocks::~RenderBlocks() {
	m_blockAccess = NULL;
}

void RenderBlocks::renderBlockUsingTexture(Block* var1, int var2, int var3, int var4, int var5) {
		m_overrideBlockTexture = var5;
		renderBlockByRenderType(var1, var2, var3, var4);
		m_overrideBlockTexture = -1;
}

bool RenderBlocks::renderBlockByRenderType(Block* var1, int var2, int var3, int var4) {
		int var5 = var1->getRenderType();
		var1->setBlockBoundsBasedOnState(m_blockAccess, var2, var3, var4);
		return var5 == 0 ? renderStandardBlock(var1, var2, var3, var4) : (var5 == 4 ? renderBlockFluids(var1, var2, var3, var4) : (var5 == 13 ? renderBlockCactus(var1, var2, var3, var4) : (var5 == 1 ? renderBlockReed(var1, var2, var3, var4) : (var5 == 6 ? renderBlockCrops(var1, var2, var3, var4) : (var5 == 2 ? renderBlockTorch(var1, var2, var3, var4) : (var5 == 3 ? renderBlockFire(var1, var2, var3, var4) : (var5 == 5 ? renderBlockRedstoneWire(var1, var2, var3, var4) : (var5 == 8 ? renderBlockLadder(var1, var2, var3, var4) : (var5 == 7 ? renderBlockDoor(var1, var2, var3, var4) : (var5 == 9 ? renderBlockMinecartTrack(var1, var2, var3, var4) : (var5 == 10 ? renderBlockStairs(var1, var2, var3, var4) : (var5 == 11 ? renderBlockFence(var1, var2, var3, var4) : (var5 == 12 ? renderBlockLever(var1, var2, var3, var4) : false)))))))))))));
}

bool RenderBlocks::renderBlockTorch(Block* var1, int var2, int var3, int var4) {
		int var5 = m_blockAccess->getBlockMetadata(var2, var3, var4);
		Tesselator& var6 = Tesselator::getInstance();
		float var7 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		if(Block::s_lightValue[var1->blockID] > 0) {
			var7 = 1.0F;
		}

		var6.color3f(var7, var7, var7);
		double var8 = (double)0.4F;
		double var10 = 0.5f - var8;
		double var12 = (double)0.2F;
		if(var5 == 1) {
			renderTorchAtAngle(var1, (double)var2 - var10, (double)var3 + var12, (double)var4, -var8, 0.0f);
		} else if(var5 == 2) {
			renderTorchAtAngle(var1, (double)var2 + var10, (double)var3 + var12, (double)var4, var8, 0.0f);
		} else if(var5 == 3) {
			renderTorchAtAngle(var1, (double)var2, (double)var3 + var12, (double)var4 - var10, 0.0f, -var8);
		} else if(var5 == 4) {
			renderTorchAtAngle(var1, (double)var2, (double)var3 + var12, (double)var4 + var10, 0.0f, var8);
		} else {
			renderTorchAtAngle(var1, (double)var2, (double)var3, (double)var4, 0.0f, 0.0f);
		}

		return true;
}

bool RenderBlocks::renderBlockLever(Block* var1, int var2, int var3, int var4) {
		int var5 = m_blockAccess->getBlockMetadata(var2, var3, var4);
		int var6 = var5 & 7;
		bool var7 = (var5 & 8) > 0;
		Tesselator& var8 = Tesselator::getInstance();
		bool var9 = m_overrideBlockTexture >= 0;
		if(!var9) {
			m_overrideBlockTexture = Block::s_cobblestone->blockIndexInTexture;
		}

		float var10 = 0.25F;
		float var11 = 3.0F / 16.0F;
		float var12 = 3.0F / 16.0F;
		if(var6 == 5) {
			var1->setBlockBounds(0.5F - var11, 0.0F, 0.5F - var10, 0.5F + var11, var12, 0.5F + var10);
		} else if(var6 == 6) {
			var1->setBlockBounds(0.5F - var10, 0.0F, 0.5F - var11, 0.5F + var10, var12, 0.5F + var11);
		} else if(var6 == 4) {
			var1->setBlockBounds(0.5F - var11, 0.5F - var10, 1.0F - var12, 0.5F + var11, 0.5F + var10, 1.0F);
		} else if(var6 == 3) {
			var1->setBlockBounds(0.5F - var11, 0.5F - var10, 0.0F, 0.5F + var11, 0.5F + var10, var12);
		} else if(var6 == 2) {
			var1->setBlockBounds(1.0F - var12, 0.5F - var10, 0.5F - var11, 1.0F, 0.5F + var10, 0.5F + var11);
		} else if(var6 == 1) {
			var1->setBlockBounds(0.0F, 0.5F - var10, 0.5F - var11, var12, 0.5F + var10, 0.5F + var11);
		}

		renderStandardBlock(var1, var2, var3, var4);
		if(!var9) {
			m_overrideBlockTexture = -1;
		}

		float var13 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		if(Block::s_lightValue[var1->blockID] > 0) {
			var13 = 1.0F;
		}

		var8.color3f(var13, var13, var13);
		int var14 = var1->getBlockTextureFromSide(0);
		if(m_overrideBlockTexture >= 0) {
			var14 = m_overrideBlockTexture;
		}

		int var15 = (var14 & 15) << 4;
		int var16 = var14 & 240;
		float var17 = (float)var15 / 256.0F;
		float var18 = ((float)var15 + 15.99F) / 256.0F;
		float var19 = (float)var16 / 256.0F;
		float var20 = ((float)var16 + 15.99F) / 256.0F;
		// PORT: was `Vec3D* var21 = new Vec3D[8]` (array of VALUES) but createVector returns
		// Vec3D* and the rotate*/coord ops below need pointers -> array of Vec3D*.
		Vec3D** var21 = new Vec3D*[8];
		float var22 = 1.0F / 16.0F;
		float var23 = 1.0F / 16.0F;
		float var24 = 10.0F / 16.0F;
		var21[0] = Vec3D::createVector((double)(-var22), 0.0f, (double)(-var23));
		var21[1] = Vec3D::createVector((double)var22, 0.0f, (double)(-var23));
		var21[2] = Vec3D::createVector((double)var22, 0.0f, (double)var23);
		var21[3] = Vec3D::createVector((double)(-var22), 0.0f, (double)var23);
		var21[4] = Vec3D::createVector((double)(-var22), (double)var24, (double)(-var23));
		var21[5] = Vec3D::createVector((double)var22, (double)var24, (double)(-var23));
		var21[6] = Vec3D::createVector((double)var22, (double)var24, (double)var23);
		var21[7] = Vec3D::createVector((double)(-var22), (double)var24, (double)var23);

		for(int var25 = 0; var25 < 8; ++var25) {
			if(var7) {
				var21[var25]->zCoord -= 1.0f / 16.0f;
				var21[var25]->rotateAroundX(3.14159265358979323846f * 2.0F / 9.0F);
			} else {
				var21[var25]->zCoord += 1.0f / 16.0f;
				var21[var25]->rotateAroundX(-(3.14159265358979323846f * 2.0F / 9.0F));
			}

			if(var6 == 6) {
				var21[var25]->rotateAroundY(3.14159265358979323846f * 0.5F);
			}

			if(var6 < 5) {
				var21[var25]->yCoord -= 0.375f;
				var21[var25]->rotateAroundX(3.14159265358979323846f * 0.5F);
				if(var6 == 4) {
					var21[var25]->rotateAroundY(0.0F);
				}

				if(var6 == 3) {
					var21[var25]->rotateAroundY(3.14159265358979323846f);
				}

				if(var6 == 2) {
					var21[var25]->rotateAroundY(3.14159265358979323846f * 0.5F);
				}

				if(var6 == 1) {
					var21[var25]->rotateAroundY(3.14159265358979323846f * -0.5F);
				}

				var21[var25]->xCoord += (double)var2 + 0.5f;
				var21[var25]->yCoord += (double)((float)var3 + 0.5F);
				var21[var25]->zCoord += (double)var4 + 0.5f;
			} else {
				var21[var25]->xCoord += (double)var2 + 0.5f;
				var21[var25]->yCoord += (double)((float)var3 + 2.0F / 16.0F);
				var21[var25]->zCoord += (double)var4 + 0.5f;
			}
		}

		Vec3D* var30 = NULL;
		Vec3D* var26 = NULL;
		Vec3D* var27 = NULL;
		Vec3D* var28 = NULL;

		for(int var29 = 0; var29 < 6; ++var29) {
			if(var29 == 0) {
				var17 = (float)(var15 + 7) / 256.0F;
				var18 = ((float)(var15 + 9) - 0.01F) / 256.0F;
				var19 = (float)(var16 + 6) / 256.0F;
				var20 = ((float)(var16 + 8) - 0.01F) / 256.0F;
			} else if(var29 == 2) {
				var17 = (float)(var15 + 7) / 256.0F;
				var18 = ((float)(var15 + 9) - 0.01F) / 256.0F;
				var19 = (float)(var16 + 6) / 256.0F;
				var20 = ((float)(var16 + 16) - 0.01F) / 256.0F;
			}

			if(var29 == 0) {
				var30 = var21[0];
				var26 = var21[1];
				var27 = var21[2];
				var28 = var21[3];
			} else if(var29 == 1) {
				var30 = var21[7];
				var26 = var21[6];
				var27 = var21[5];
				var28 = var21[4];
			} else if(var29 == 2) {
				var30 = var21[1];
				var26 = var21[0];
				var27 = var21[4];
				var28 = var21[5];
			} else if(var29 == 3) {
				var30 = var21[2];
				var26 = var21[1];
				var27 = var21[5];
				var28 = var21[6];
			} else if(var29 == 4) {
				var30 = var21[3];
				var26 = var21[2];
				var27 = var21[6];
				var28 = var21[7];
			} else if(var29 == 5) {
				var30 = var21[0];
				var26 = var21[3];
				var27 = var21[7];
				var28 = var21[4];
			}

			var8.vertexUV(var30->xCoord, var30->yCoord, var30->zCoord, (double)var17, (double)var20);
			var8.vertexUV(var26->xCoord, var26->yCoord, var26->zCoord, (double)var18, (double)var20);
			var8.vertexUV(var27->xCoord, var27->yCoord, var27->zCoord, (double)var18, (double)var19);
			var8.vertexUV(var28->xCoord, var28->yCoord, var28->zCoord, (double)var17, (double)var19);
		}

		for(int var31 = 0; var31 < 8; ++var31) {
			delete var21[var31];
		}
		delete[] var21;
		return true;
}

bool RenderBlocks::renderBlockFire(Block* var1, int var2, int var3, int var4) {
		Tesselator& var5 = Tesselator::getInstance();
		int var6 = var1->getBlockTextureFromSide(0);
		if(m_overrideBlockTexture >= 0) {
			var6 = m_overrideBlockTexture;
		}

		float var7 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		var5.color3f(var7, var7, var7);
		int var8 = (var6 & 15) << 4;
		int var9 = var6 & 240;
		double var10 = (double)((float)var8 / 256.0F);
		double var12 = (double)(((float)var8 + 15.99F) / 256.0F);
		double var14 = (double)((float)var9 / 256.0F);
		double var16 = (double)(((float)var9 + 15.99F) / 256.0F);
		float var18 = 1.4F;
		double var21;
		double var23;
		double var25;
		double var27;
		double var29;
		double var31;
		double var33;
		if(!m_blockAccess->isBlockOpaqueCube(var2, var3 - 1, var4) && !Block::s_fire->canBlockCatchFire(m_blockAccess, var2, var3 - 1, var4)) {
			float var37 = 0.2F;
			float var20 = 1.0F / 16.0F;
			if((var2 + var3 + var4 & 1) == 1) {
				var10 = (double)((float)var8 / 256.0F);
				var12 = (double)(((float)var8 + 15.99F) / 256.0F);
				var14 = (double)((float)(var9 + 16) / 256.0F);
				var16 = (double)(((float)var9 + 15.99F + 16.0F) / 256.0F);
			}

			if((var2 / 2 + var3 / 2 + var4 / 2 & 1) == 1) {
				var21 = var12;
				var12 = var10;
				var10 = var21;
			}

			if(Block::s_fire->canBlockCatchFire(m_blockAccess, var2 - 1, var3, var4)) {
				var5.vertexUV((double)((float)var2 + var37), (double)((float)var3 + var18 + var20), (double)(var4 + 1), var12, var14);
				var5.vertexUV((double)(var2 + 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 1), var12, var16);
				var5.vertexUV((double)(var2 + 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 0), var10, var16);
				var5.vertexUV((double)((float)var2 + var37), (double)((float)var3 + var18 + var20), (double)(var4 + 0), var10, var14);
				var5.vertexUV((double)((float)var2 + var37), (double)((float)var3 + var18 + var20), (double)(var4 + 0), var10, var14);
				var5.vertexUV((double)(var2 + 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 0), var10, var16);
				var5.vertexUV((double)(var2 + 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 1), var12, var16);
				var5.vertexUV((double)((float)var2 + var37), (double)((float)var3 + var18 + var20), (double)(var4 + 1), var12, var14);
			}

			if(Block::s_fire->canBlockCatchFire(m_blockAccess, var2 + 1, var3, var4)) {
				var5.vertexUV((double)((float)(var2 + 1) - var37), (double)((float)var3 + var18 + var20), (double)(var4 + 0), var10, var14);
				var5.vertexUV((double)(var2 + 1 - 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 0), var10, var16);
				var5.vertexUV((double)(var2 + 1 - 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 1), var12, var16);
				var5.vertexUV((double)((float)(var2 + 1) - var37), (double)((float)var3 + var18 + var20), (double)(var4 + 1), var12, var14);
				var5.vertexUV((double)((float)(var2 + 1) - var37), (double)((float)var3 + var18 + var20), (double)(var4 + 1), var12, var14);
				var5.vertexUV((double)(var2 + 1 - 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 1), var12, var16);
				var5.vertexUV((double)(var2 + 1 - 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 0), var10, var16);
				var5.vertexUV((double)((float)(var2 + 1) - var37), (double)((float)var3 + var18 + var20), (double)(var4 + 0), var10, var14);
			}

			if(Block::s_fire->canBlockCatchFire(m_blockAccess, var2, var3, var4 - 1)) {
				var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18 + var20), (double)((float)var4 + var37), var12, var14);
				var5.vertexUV((double)(var2 + 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 0), var12, var16);
				var5.vertexUV((double)(var2 + 1), (double)((float)(var3 + 0) + var20), (double)(var4 + 0), var10, var16);
				var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18 + var20), (double)((float)var4 + var37), var10, var14);
				var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18 + var20), (double)((float)var4 + var37), var10, var14);
				var5.vertexUV((double)(var2 + 1), (double)((float)(var3 + 0) + var20), (double)(var4 + 0), var10, var16);
				var5.vertexUV((double)(var2 + 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 0), var12, var16);
				var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18 + var20), (double)((float)var4 + var37), var12, var14);
			}

			if(Block::s_fire->canBlockCatchFire(m_blockAccess, var2, var3, var4 + 1)) {
				var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18 + var20), (double)((float)(var4 + 1) - var37), var10, var14);
				var5.vertexUV((double)(var2 + 1), (double)((float)(var3 + 0) + var20), (double)(var4 + 1 - 0), var10, var16);
				var5.vertexUV((double)(var2 + 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 1 - 0), var12, var16);
				var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18 + var20), (double)((float)(var4 + 1) - var37), var12, var14);
				var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18 + var20), (double)((float)(var4 + 1) - var37), var12, var14);
				var5.vertexUV((double)(var2 + 0), (double)((float)(var3 + 0) + var20), (double)(var4 + 1 - 0), var12, var16);
				var5.vertexUV((double)(var2 + 1), (double)((float)(var3 + 0) + var20), (double)(var4 + 1 - 0), var10, var16);
				var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18 + var20), (double)((float)(var4 + 1) - var37), var10, var14);
			}

			if(Block::s_fire->canBlockCatchFire(m_blockAccess, var2, var3 + 1, var4)) {
				var21 = (double)var2 + 0.5f + 0.5f;
				var23 = (double)var2 + 0.5f - 0.5f;
				var25 = (double)var4 + 0.5f + 0.5f;
				var27 = (double)var4 + 0.5f - 0.5f;
				var29 = (double)var2 + 0.5f - 0.5f;
				var31 = (double)var2 + 0.5f + 0.5f;
				var33 = (double)var4 + 0.5f - 0.5f;
				double var35 = (double)var4 + 0.5f + 0.5f;
				var10 = (double)((float)var8 / 256.0F);
				var12 = (double)(((float)var8 + 15.99F) / 256.0F);
				var14 = (double)((float)var9 / 256.0F);
				var16 = (double)(((float)var9 + 15.99F) / 256.0F);
				++var3;
				var18 = -0.2F;
				if((var2 + var3 + var4 & 1) == 0) {
					var5.vertexUV(var29, (double)((float)var3 + var18), (double)(var4 + 0), var12, var14);
					var5.vertexUV(var21, (double)(var3 + 0), (double)(var4 + 0), var12, var16);
					var5.vertexUV(var21, (double)(var3 + 0), (double)(var4 + 1), var10, var16);
					var5.vertexUV(var29, (double)((float)var3 + var18), (double)(var4 + 1), var10, var14);
					var10 = (double)((float)var8 / 256.0F);
					var12 = (double)(((float)var8 + 15.99F) / 256.0F);
					var14 = (double)((float)(var9 + 16) / 256.0F);
					var16 = (double)(((float)var9 + 15.99F + 16.0F) / 256.0F);
					var5.vertexUV(var31, (double)((float)var3 + var18), (double)(var4 + 1), var12, var14);
					var5.vertexUV(var23, (double)(var3 + 0), (double)(var4 + 1), var12, var16);
					var5.vertexUV(var23, (double)(var3 + 0), (double)(var4 + 0), var10, var16);
					var5.vertexUV(var31, (double)((float)var3 + var18), (double)(var4 + 0), var10, var14);
				} else {
					var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18), var35, var12, var14);
					var5.vertexUV((double)(var2 + 0), (double)(var3 + 0), var27, var12, var16);
					var5.vertexUV((double)(var2 + 1), (double)(var3 + 0), var27, var10, var16);
					var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18), var35, var10, var14);
					var10 = (double)((float)var8 / 256.0F);
					var12 = (double)(((float)var8 + 15.99F) / 256.0F);
					var14 = (double)((float)(var9 + 16) / 256.0F);
					var16 = (double)(((float)var9 + 15.99F + 16.0F) / 256.0F);
					var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18), var33, var12, var14);
					var5.vertexUV((double)(var2 + 1), (double)(var3 + 0), var25, var12, var16);
					var5.vertexUV((double)(var2 + 0), (double)(var3 + 0), var25, var10, var16);
					var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18), var33, var10, var14);
				}
			}
		} else {
			double var19 = (double)var2 + 0.5f + 0.2f;
			var21 = (double)var2 + 0.5f - 0.2f;
			var23 = (double)var4 + 0.5f + 0.2f;
			var25 = (double)var4 + 0.5f - 0.2f;
			var27 = (double)var2 + 0.5f - 0.3f;
			var29 = (double)var2 + 0.5f + 0.3f;
			var31 = (double)var4 + 0.5f - 0.3f;
			var33 = (double)var4 + 0.5f + 0.3f;
			var5.vertexUV(var27, (double)((float)var3 + var18), (double)(var4 + 1), var12, var14);
			var5.vertexUV(var19, (double)(var3 + 0), (double)(var4 + 1), var12, var16);
			var5.vertexUV(var19, (double)(var3 + 0), (double)(var4 + 0), var10, var16);
			var5.vertexUV(var27, (double)((float)var3 + var18), (double)(var4 + 0), var10, var14);
			var5.vertexUV(var29, (double)((float)var3 + var18), (double)(var4 + 0), var12, var14);
			var5.vertexUV(var21, (double)(var3 + 0), (double)(var4 + 0), var12, var16);
			var5.vertexUV(var21, (double)(var3 + 0), (double)(var4 + 1), var10, var16);
			var5.vertexUV(var29, (double)((float)var3 + var18), (double)(var4 + 1), var10, var14);
			var10 = (double)((float)var8 / 256.0F);
			var12 = (double)(((float)var8 + 15.99F) / 256.0F);
			var14 = (double)((float)(var9 + 16) / 256.0F);
			var16 = (double)(((float)var9 + 15.99F + 16.0F) / 256.0F);
			var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18), var33, var12, var14);
			var5.vertexUV((double)(var2 + 1), (double)(var3 + 0), var25, var12, var16);
			var5.vertexUV((double)(var2 + 0), (double)(var3 + 0), var25, var10, var16);
			var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18), var33, var10, var14);
			var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18), var31, var12, var14);
			var5.vertexUV((double)(var2 + 0), (double)(var3 + 0), var23, var12, var16);
			var5.vertexUV((double)(var2 + 1), (double)(var3 + 0), var23, var10, var16);
			var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18), var31, var10, var14);
			var19 = (double)var2 + 0.5f - 0.5f;
			var21 = (double)var2 + 0.5f + 0.5f;
			var23 = (double)var4 + 0.5f - 0.5f;
			var25 = (double)var4 + 0.5f + 0.5f;
			var27 = (double)var2 + 0.5f - 0.4f;
			var29 = (double)var2 + 0.5f + 0.4f;
			var31 = (double)var4 + 0.5f - 0.4f;
			var33 = (double)var4 + 0.5f + 0.4f;
			var5.vertexUV(var27, (double)((float)var3 + var18), (double)(var4 + 0), var10, var14);
			var5.vertexUV(var19, (double)(var3 + 0), (double)(var4 + 0), var10, var16);
			var5.vertexUV(var19, (double)(var3 + 0), (double)(var4 + 1), var12, var16);
			var5.vertexUV(var27, (double)((float)var3 + var18), (double)(var4 + 1), var12, var14);
			var5.vertexUV(var29, (double)((float)var3 + var18), (double)(var4 + 1), var10, var14);
			var5.vertexUV(var21, (double)(var3 + 0), (double)(var4 + 1), var10, var16);
			var5.vertexUV(var21, (double)(var3 + 0), (double)(var4 + 0), var12, var16);
			var5.vertexUV(var29, (double)((float)var3 + var18), (double)(var4 + 0), var12, var14);
			var10 = (double)((float)var8 / 256.0F);
			var12 = (double)(((float)var8 + 15.99F) / 256.0F);
			var14 = (double)((float)var9 / 256.0F);
			var16 = (double)(((float)var9 + 15.99F) / 256.0F);
			var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18), var33, var10, var14);
			var5.vertexUV((double)(var2 + 0), (double)(var3 + 0), var25, var10, var16);
			var5.vertexUV((double)(var2 + 1), (double)(var3 + 0), var25, var12, var16);
			var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18), var33, var12, var14);
			var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var18), var31, var10, var14);
			var5.vertexUV((double)(var2 + 1), (double)(var3 + 0), var23, var10, var16);
			var5.vertexUV((double)(var2 + 0), (double)(var3 + 0), var23, var12, var16);
			var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var18), var31, var12, var14);
		}

		return true;
}

bool RenderBlocks::renderBlockRedstoneWire(Block* var1, int var2, int var3, int var4) {
		Tesselator& var5 = Tesselator::getInstance();
		int var6 = var1->getBlockTextureFromSideAndMetadata(1, m_blockAccess->getBlockMetadata(var2, var3, var4));
		if(m_overrideBlockTexture >= 0) {
			var6 = m_overrideBlockTexture;
		}

		float var7 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		var5.color3f(var7, var7, var7);
		int var8 = (var6 & 15) << 4;
		int var9 = var6 & 240;
		double var10 = (double)((float)var8 / 256.0F);
		double var12 = (double)(((float)var8 + 15.99F) / 256.0F);
		double var14 = (double)((float)var9 / 256.0F);
		double var16 = (double)(((float)var9 + 15.99F) / 256.0F);
		float var18 = 0.0F;
		float var19 = 0.03125F;
		bool var20 = BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2 - 1, var3, var4) || !m_blockAccess->isBlockOpaqueCube(var2 - 1, var3, var4) && BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2 - 1, var3 - 1, var4);
		bool var21 = BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2 + 1, var3, var4) || !m_blockAccess->isBlockOpaqueCube(var2 + 1, var3, var4) && BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2 + 1, var3 - 1, var4);
		bool var22 = BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2, var3, var4 - 1) || !m_blockAccess->isBlockOpaqueCube(var2, var3, var4 - 1) && BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2, var3 - 1, var4 - 1);
		bool var23 = BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2, var3, var4 + 1) || !m_blockAccess->isBlockOpaqueCube(var2, var3, var4 + 1) && BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2, var3 - 1, var4 + 1);
		if(!m_blockAccess->isBlockOpaqueCube(var2, var3 + 1, var4)) {
			if(m_blockAccess->isBlockOpaqueCube(var2 - 1, var3, var4) && BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2 - 1, var3 + 1, var4)) {
				var20 = true;
			}

			if(m_blockAccess->isBlockOpaqueCube(var2 + 1, var3, var4) && BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2 + 1, var3 + 1, var4)) {
				var21 = true;
			}

			if(m_blockAccess->isBlockOpaqueCube(var2, var3, var4 - 1) && BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2, var3 + 1, var4 - 1)) {
				var22 = true;
			}

			if(m_blockAccess->isBlockOpaqueCube(var2, var3, var4 + 1) && BlockRedstoneWire::isPowerProviderOrWire(m_blockAccess, var2, var3 + 1, var4 + 1)) {
				var23 = true;
			}
		}

		float var24 = 5.0F / 16.0F;
		float var25 = (float)(var2 + 0);
		float var26 = (float)(var2 + 1);
		float var27 = (float)(var4 + 0);
		float var28 = (float)(var4 + 1);
		unsigned char var29 = 0;
		if((var20 || var21) && !var22 && !var23) {
			var29 = 1;
		}

		if((var22 || var23) && !var21 && !var20) {
			var29 = 2;
		}

		if(var29 != 0) {
			var10 = (double)((float)(var8 + 16) / 256.0F);
			var12 = (double)(((float)(var8 + 16) + 15.99F) / 256.0F);
			var14 = (double)((float)var9 / 256.0F);
			var16 = (double)(((float)var9 + 15.99F) / 256.0F);
		}

		if(var29 == 0) {
			if(var21 || var22 || var23 || var20) {
				if(!var20) {
					var25 += var24;
				}

				if(!var20) {
					var10 += (double)(var24 / 16.0F);
				}

				if(!var21) {
					var26 -= var24;
				}

				if(!var21) {
					var12 -= (double)(var24 / 16.0F);
				}

				if(!var22) {
					var27 += var24;
				}

				if(!var22) {
					var14 += (double)(var24 / 16.0F);
				}

				if(!var23) {
					var28 -= var24;
				}

				if(!var23) {
					var16 -= (double)(var24 / 16.0F);
				}
			}

			var5.vertexUV((double)(var26 + var18), (double)((float)var3 + var19), (double)(var28 + var18), var12, var16);
			var5.vertexUV((double)(var26 + var18), (double)((float)var3 + var19), (double)(var27 - var18), var12, var14);
			var5.vertexUV((double)(var25 - var18), (double)((float)var3 + var19), (double)(var27 - var18), var10, var14);
			var5.vertexUV((double)(var25 - var18), (double)((float)var3 + var19), (double)(var28 + var18), var10, var16);
		}

		if(var29 == 1) {
			var5.vertexUV((double)(var26 + var18), (double)((float)var3 + var19), (double)(var28 + var18), var12, var16);
			var5.vertexUV((double)(var26 + var18), (double)((float)var3 + var19), (double)(var27 - var18), var12, var14);
			var5.vertexUV((double)(var25 - var18), (double)((float)var3 + var19), (double)(var27 - var18), var10, var14);
			var5.vertexUV((double)(var25 - var18), (double)((float)var3 + var19), (double)(var28 + var18), var10, var16);
		}

		if(var29 == 2) {
			var5.vertexUV((double)(var26 + var18), (double)((float)var3 + var19), (double)(var28 + var18), var12, var16);
			var5.vertexUV((double)(var26 + var18), (double)((float)var3 + var19), (double)(var27 - var18), var10, var16);
			var5.vertexUV((double)(var25 - var18), (double)((float)var3 + var19), (double)(var27 - var18), var10, var14);
			var5.vertexUV((double)(var25 - var18), (double)((float)var3 + var19), (double)(var28 + var18), var12, var14);
		}

		var10 = (double)((float)(var8 + 16) / 256.0F);
		var12 = (double)(((float)(var8 + 16) + 15.99F) / 256.0F);
		var14 = (double)((float)var9 / 256.0F);
		var16 = (double)(((float)var9 + 15.99F) / 256.0F);
		if(!m_blockAccess->isBlockOpaqueCube(var2, var3 + 1, var4)) {
			if(m_blockAccess->isBlockOpaqueCube(var2 - 1, var3, var4) && m_blockAccess->getBlockId(var2 - 1, var3 + 1, var4) == Block::s_redstoneWire->blockID) {
				var5.vertexUV((double)((float)var2 + var19), (double)((float)(var3 + 1) + var18), (double)((float)(var4 + 1) + var18), var12, var14);
				var5.vertexUV((double)((float)var2 + var19), (double)((float)(var3 + 0) - var18), (double)((float)(var4 + 1) + var18), var10, var14);
				var5.vertexUV((double)((float)var2 + var19), (double)((float)(var3 + 0) - var18), (double)((float)(var4 + 0) - var18), var10, var16);
				var5.vertexUV((double)((float)var2 + var19), (double)((float)(var3 + 1) + var18), (double)((float)(var4 + 0) - var18), var12, var16);
			}

			if(m_blockAccess->isBlockOpaqueCube(var2 + 1, var3, var4) && m_blockAccess->getBlockId(var2 + 1, var3 + 1, var4) == Block::s_redstoneWire->blockID) {
				var5.vertexUV((double)((float)(var2 + 1) - var19), (double)((float)(var3 + 0) - var18), (double)((float)(var4 + 1) + var18), var10, var16);
				var5.vertexUV((double)((float)(var2 + 1) - var19), (double)((float)(var3 + 1) + var18), (double)((float)(var4 + 1) + var18), var12, var16);
				var5.vertexUV((double)((float)(var2 + 1) - var19), (double)((float)(var3 + 1) + var18), (double)((float)(var4 + 0) - var18), var12, var14);
				var5.vertexUV((double)((float)(var2 + 1) - var19), (double)((float)(var3 + 0) - var18), (double)((float)(var4 + 0) - var18), var10, var14);
			}

			if(m_blockAccess->isBlockOpaqueCube(var2, var3, var4 - 1) && m_blockAccess->getBlockId(var2, var3 + 1, var4 - 1) == Block::s_redstoneWire->blockID) {
				var5.vertexUV((double)((float)(var2 + 1) + var18), (double)((float)(var3 + 0) - var18), (double)((float)var4 + var19), var10, var16);
				var5.vertexUV((double)((float)(var2 + 1) + var18), (double)((float)(var3 + 1) + var18), (double)((float)var4 + var19), var12, var16);
				var5.vertexUV((double)((float)(var2 + 0) - var18), (double)((float)(var3 + 1) + var18), (double)((float)var4 + var19), var12, var14);
				var5.vertexUV((double)((float)(var2 + 0) - var18), (double)((float)(var3 + 0) - var18), (double)((float)var4 + var19), var10, var14);
			}

			if(m_blockAccess->isBlockOpaqueCube(var2, var3, var4 + 1) && m_blockAccess->getBlockId(var2, var3 + 1, var4 + 1) == Block::s_redstoneWire->blockID) {
				var5.vertexUV((double)((float)(var2 + 1) + var18), (double)((float)(var3 + 1) + var18), (double)((float)(var4 + 1) - var19), var12, var14);
				var5.vertexUV((double)((float)(var2 + 1) + var18), (double)((float)(var3 + 0) - var18), (double)((float)(var4 + 1) - var19), var10, var14);
				var5.vertexUV((double)((float)(var2 + 0) - var18), (double)((float)(var3 + 0) - var18), (double)((float)(var4 + 1) - var19), var10, var16);
				var5.vertexUV((double)((float)(var2 + 0) - var18), (double)((float)(var3 + 1) + var18), (double)((float)(var4 + 1) - var19), var12, var16);
			}
		}

		return true;
}

bool RenderBlocks::renderBlockMinecartTrack(Block* var1, int var2, int var3, int var4) {
		Tesselator& var5 = Tesselator::getInstance();
		int var6 = m_blockAccess->getBlockMetadata(var2, var3, var4);
		int var7 = var1->getBlockTextureFromSideAndMetadata(0, var6);
		if(m_overrideBlockTexture >= 0) {
			var7 = m_overrideBlockTexture;
		}

		float var8 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		var5.color3f(var8, var8, var8);
		int var9 = (var7 & 15) << 4;
		int var10 = var7 & 240;
		double var11 = (double)((float)var9 / 256.0F);
		double var13 = (double)(((float)var9 + 15.99F) / 256.0F);
		double var15 = (double)((float)var10 / 256.0F);
		double var17 = (double)(((float)var10 + 15.99F) / 256.0F);
		float var19 = 1.0F / 16.0F;
		float var20 = (float)(var2 + 1);
		float var21 = (float)(var2 + 1);
		float var22 = (float)(var2 + 0);
		float var23 = (float)(var2 + 0);
		float var24 = (float)(var4 + 0);
		float var25 = (float)(var4 + 1);
		float var26 = (float)(var4 + 1);
		float var27 = (float)(var4 + 0);
		float var28 = (float)var3 + var19;
		float var29 = (float)var3 + var19;
		float var30 = (float)var3 + var19;
		float var31 = (float)var3 + var19;
		if(var6 != 1 && var6 != 2 && var6 != 3 && var6 != 7) {
			if(var6 == 8) {
				var21 = (float)(var2 + 0);
				var20 = var21;
				var23 = (float)(var2 + 1);
				var22 = var23;
				var27 = (float)(var4 + 1);
				var24 = var27;
				var26 = (float)(var4 + 0);
				var25 = var26;
			} else if(var6 == 9) {
				var23 = (float)(var2 + 0);
				var20 = var23;
				var22 = (float)(var2 + 1);
				var21 = var22;
				var25 = (float)(var4 + 0);
				var24 = var25;
				var27 = (float)(var4 + 1);
				var26 = var27;
			}
		} else {
			var23 = (float)(var2 + 1);
			var20 = var23;
			var22 = (float)(var2 + 0);
			var21 = var22;
			var25 = (float)(var4 + 1);
			var24 = var25;
			var27 = (float)(var4 + 0);
			var26 = var27;
		}

		if(var6 != 2 && var6 != 4) {
			if(var6 == 3 || var6 == 5) {
				++var29;
				++var30;
			}
		} else {
			++var28;
			++var31;
		}

		var5.vertexUV((double)var20, (double)var28, (double)var24, var13, var15);
		var5.vertexUV((double)var21, (double)var29, (double)var25, var13, var17);
		var5.vertexUV((double)var22, (double)var30, (double)var26, var11, var17);
		var5.vertexUV((double)var23, (double)var31, (double)var27, var11, var15);
		var5.vertexUV((double)var23, (double)var31, (double)var27, var11, var15);
		var5.vertexUV((double)var22, (double)var30, (double)var26, var11, var17);
		var5.vertexUV((double)var21, (double)var29, (double)var25, var13, var17);
		var5.vertexUV((double)var20, (double)var28, (double)var24, var13, var15);
		return true;
}

bool RenderBlocks::renderBlockLadder(Block* var1, int var2, int var3, int var4) {
		Tesselator& var5 = Tesselator::getInstance();
		int var6 = var1->getBlockTextureFromSide(0);
		if(m_overrideBlockTexture >= 0) {
			var6 = m_overrideBlockTexture;
		}

		float var7 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		var5.color3f(var7, var7, var7);
		int var8 = (var6 & 15) << 4;
		int var9 = var6 & 240;
		double var10 = (double)((float)var8 / 256.0F);
		double var12 = (double)(((float)var8 + 15.99F) / 256.0F);
		double var14 = (double)((float)var9 / 256.0F);
		double var16 = (double)(((float)var9 + 15.99F) / 256.0F);
		int var18 = m_blockAccess->getBlockMetadata(var2, var3, var4);
		float var19 = 0.0F;
		float var20 = 0.05F;
		if(var18 == 5) {
			var5.vertexUV((double)((float)var2 + var20), (double)((float)(var3 + 1) + var19), (double)((float)(var4 + 1) + var19), var10, var14);
			var5.vertexUV((double)((float)var2 + var20), (double)((float)(var3 + 0) - var19), (double)((float)(var4 + 1) + var19), var10, var16);
			var5.vertexUV((double)((float)var2 + var20), (double)((float)(var3 + 0) - var19), (double)((float)(var4 + 0) - var19), var12, var16);
			var5.vertexUV((double)((float)var2 + var20), (double)((float)(var3 + 1) + var19), (double)((float)(var4 + 0) - var19), var12, var14);
		}

		if(var18 == 4) {
			var5.vertexUV((double)((float)(var2 + 1) - var20), (double)((float)(var3 + 0) - var19), (double)((float)(var4 + 1) + var19), var12, var16);
			var5.vertexUV((double)((float)(var2 + 1) - var20), (double)((float)(var3 + 1) + var19), (double)((float)(var4 + 1) + var19), var12, var14);
			var5.vertexUV((double)((float)(var2 + 1) - var20), (double)((float)(var3 + 1) + var19), (double)((float)(var4 + 0) - var19), var10, var14);
			var5.vertexUV((double)((float)(var2 + 1) - var20), (double)((float)(var3 + 0) - var19), (double)((float)(var4 + 0) - var19), var10, var16);
		}

		if(var18 == 3) {
			var5.vertexUV((double)((float)(var2 + 1) + var19), (double)((float)(var3 + 0) - var19), (double)((float)var4 + var20), var12, var16);
			var5.vertexUV((double)((float)(var2 + 1) + var19), (double)((float)(var3 + 1) + var19), (double)((float)var4 + var20), var12, var14);
			var5.vertexUV((double)((float)(var2 + 0) - var19), (double)((float)(var3 + 1) + var19), (double)((float)var4 + var20), var10, var14);
			var5.vertexUV((double)((float)(var2 + 0) - var19), (double)((float)(var3 + 0) - var19), (double)((float)var4 + var20), var10, var16);
		}

		if(var18 == 2) {
			var5.vertexUV((double)((float)(var2 + 1) + var19), (double)((float)(var3 + 1) + var19), (double)((float)(var4 + 1) - var20), var10, var14);
			var5.vertexUV((double)((float)(var2 + 1) + var19), (double)((float)(var3 + 0) - var19), (double)((float)(var4 + 1) - var20), var10, var16);
			var5.vertexUV((double)((float)(var2 + 0) - var19), (double)((float)(var3 + 0) - var19), (double)((float)(var4 + 1) - var20), var12, var16);
			var5.vertexUV((double)((float)(var2 + 0) - var19), (double)((float)(var3 + 1) + var19), (double)((float)(var4 + 1) - var20), var12, var14);
		}

		return true;
}

bool RenderBlocks::renderBlockReed(Block* var1, int var2, int var3, int var4) {
		Tesselator& var5 = Tesselator::getInstance();
		float var6 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		var5.color3f(var6, var6, var6);
		func_1239_a(var1, m_blockAccess->getBlockMetadata(var2, var3, var4), (double)var2, (double)var3, (double)var4);
		return true;
}

bool RenderBlocks::renderBlockCrops(Block* var1, int var2, int var3, int var4) {
		Tesselator& var5 = Tesselator::getInstance();
		float var6 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		var5.color3f(var6, var6, var6);
		func_1245_b(var1, m_blockAccess->getBlockMetadata(var2, var3, var4), (double)var2, (double)((float)var3 - 1.0F / 16.0F), (double)var4);
		return true;
}

void RenderBlocks::renderTorchAtAngle(Block* var1, double var2, double var4, double var6, double var8, double var10) {
		Tesselator& var12 = Tesselator::getInstance();
		int var13 = var1->getBlockTextureFromSide(0);
		if(m_overrideBlockTexture >= 0) {
			var13 = m_overrideBlockTexture;
		}

		int var14 = (var13 & 15) << 4;
		int var15 = var13 & 240;
		float var16 = (float)var14 / 256.0F;
		float var17 = ((float)var14 + 15.99F) / 256.0F;
		float var18 = (float)var15 / 256.0F;
		float var19 = ((float)var15 + 15.99F) / 256.0F;
		double var20 = (double)var16 + 1.75f / 64.0f;
		double var22 = (double)var18 + 6.0f / 256.0f;
		double var24 = (double)var16 + 9.0f / 256.0f;
		double var26 = (double)var18 + 1.0f / 32.0f;
		var2 += 0.5f;
		var6 += 0.5f;
		double var28 = var2 - 0.5f;
		double var30 = var2 + 0.5f;
		double var32 = var6 - 0.5f;
		double var34 = var6 + 0.5f;
		double var36 = 1.0f / 16.0f;
		double var38 = 0.625f;
		var12.vertexUV(var2 + var8 * (1.0f - var38) - var36, var4 + var38, var6 + var10 * (1.0f - var38) - var36, var20, var22);
		var12.vertexUV(var2 + var8 * (1.0f - var38) - var36, var4 + var38, var6 + var10 * (1.0f - var38) + var36, var20, var26);
		var12.vertexUV(var2 + var8 * (1.0f - var38) + var36, var4 + var38, var6 + var10 * (1.0f - var38) + var36, var24, var26);
		var12.vertexUV(var2 + var8 * (1.0f - var38) + var36, var4 + var38, var6 + var10 * (1.0f - var38) - var36, var24, var22);
		var12.vertexUV(var2 - var36, var4 + 1.0f, var32, (double)var16, (double)var18);
		var12.vertexUV(var2 - var36 + var8, var4 + 0.0f, var32 + var10, (double)var16, (double)var19);
		var12.vertexUV(var2 - var36 + var8, var4 + 0.0f, var34 + var10, (double)var17, (double)var19);
		var12.vertexUV(var2 - var36, var4 + 1.0f, var34, (double)var17, (double)var18);
		var12.vertexUV(var2 + var36, var4 + 1.0f, var34, (double)var16, (double)var18);
		var12.vertexUV(var2 + var8 + var36, var4 + 0.0f, var34 + var10, (double)var16, (double)var19);
		var12.vertexUV(var2 + var8 + var36, var4 + 0.0f, var32 + var10, (double)var17, (double)var19);
		var12.vertexUV(var2 + var36, var4 + 1.0f, var32, (double)var17, (double)var18);
		var12.vertexUV(var28, var4 + 1.0f, var6 + var36, (double)var16, (double)var18);
		var12.vertexUV(var28 + var8, var4 + 0.0f, var6 + var36 + var10, (double)var16, (double)var19);
		var12.vertexUV(var30 + var8, var4 + 0.0f, var6 + var36 + var10, (double)var17, (double)var19);
		var12.vertexUV(var30, var4 + 1.0f, var6 + var36, (double)var17, (double)var18);
		var12.vertexUV(var30, var4 + 1.0f, var6 - var36, (double)var16, (double)var18);
		var12.vertexUV(var30 + var8, var4 + 0.0f, var6 - var36 + var10, (double)var16, (double)var19);
		var12.vertexUV(var28 + var8, var4 + 0.0f, var6 - var36 + var10, (double)var17, (double)var19);
		var12.vertexUV(var28, var4 + 1.0f, var6 - var36, (double)var17, (double)var18);
}

void RenderBlocks::func_1239_a(Block* var1, int var2, double var3, double var5, double var7) {
		Tesselator& var9 = Tesselator::getInstance();
		int var10 = var1->getBlockTextureFromSideAndMetadata(0, var2);
		if(m_overrideBlockTexture >= 0) {
			var10 = m_overrideBlockTexture;
		}

		int var11 = (var10 & 15) << 4;
		int var12 = var10 & 240;
		double var13 = (double)((float)var11 / 256.0F);
		double var15 = (double)(((float)var11 + 15.99F) / 256.0F);
		double var17 = (double)((float)var12 / 256.0F);
		double var19 = (double)(((float)var12 + 15.99F) / 256.0F);
		double var21 = var3 + 0.5f - (double)0.45F;
		double var23 = var3 + 0.5f + (double)0.45F;
		double var25 = var7 + 0.5f - (double)0.45F;
		double var27 = var7 + 0.5f + (double)0.45F;
		var9.vertexUV(var21, var5 + 1.0f, var25, var13, var17);
		var9.vertexUV(var21, var5 + 0.0f, var25, var13, var19);
		var9.vertexUV(var23, var5 + 0.0f, var27, var15, var19);
		var9.vertexUV(var23, var5 + 1.0f, var27, var15, var17);
		var9.vertexUV(var23, var5 + 1.0f, var27, var13, var17);
		var9.vertexUV(var23, var5 + 0.0f, var27, var13, var19);
		var9.vertexUV(var21, var5 + 0.0f, var25, var15, var19);
		var9.vertexUV(var21, var5 + 1.0f, var25, var15, var17);
		var9.vertexUV(var21, var5 + 1.0f, var27, var13, var17);
		var9.vertexUV(var21, var5 + 0.0f, var27, var13, var19);
		var9.vertexUV(var23, var5 + 0.0f, var25, var15, var19);
		var9.vertexUV(var23, var5 + 1.0f, var25, var15, var17);
		var9.vertexUV(var23, var5 + 1.0f, var25, var13, var17);
		var9.vertexUV(var23, var5 + 0.0f, var25, var13, var19);
		var9.vertexUV(var21, var5 + 0.0f, var27, var15, var19);
		var9.vertexUV(var21, var5 + 1.0f, var27, var15, var17);
}

void RenderBlocks::func_1245_b(Block* var1, int var2, double var3, double var5, double var7) {
		Tesselator& var9 = Tesselator::getInstance();
		int var10 = var1->getBlockTextureFromSideAndMetadata(0, var2);
		if(m_overrideBlockTexture >= 0) {
			var10 = m_overrideBlockTexture;
		}

		int var11 = (var10 & 15) << 4;
		int var12 = var10 & 240;
		double var13 = (double)((float)var11 / 256.0F);
		double var15 = (double)(((float)var11 + 15.99F) / 256.0F);
		double var17 = (double)((float)var12 / 256.0F);
		double var19 = (double)(((float)var12 + 15.99F) / 256.0F);
		double var21 = var3 + 0.5f - 0.25f;
		double var23 = var3 + 0.5f + 0.25f;
		double var25 = var7 + 0.5f - 0.5f;
		double var27 = var7 + 0.5f + 0.5f;
		var9.vertexUV(var21, var5 + 1.0f, var25, var13, var17);
		var9.vertexUV(var21, var5 + 0.0f, var25, var13, var19);
		var9.vertexUV(var21, var5 + 0.0f, var27, var15, var19);
		var9.vertexUV(var21, var5 + 1.0f, var27, var15, var17);
		var9.vertexUV(var21, var5 + 1.0f, var27, var13, var17);
		var9.vertexUV(var21, var5 + 0.0f, var27, var13, var19);
		var9.vertexUV(var21, var5 + 0.0f, var25, var15, var19);
		var9.vertexUV(var21, var5 + 1.0f, var25, var15, var17);
		var9.vertexUV(var23, var5 + 1.0f, var27, var13, var17);
		var9.vertexUV(var23, var5 + 0.0f, var27, var13, var19);
		var9.vertexUV(var23, var5 + 0.0f, var25, var15, var19);
		var9.vertexUV(var23, var5 + 1.0f, var25, var15, var17);
		var9.vertexUV(var23, var5 + 1.0f, var25, var13, var17);
		var9.vertexUV(var23, var5 + 0.0f, var25, var13, var19);
		var9.vertexUV(var23, var5 + 0.0f, var27, var15, var19);
		var9.vertexUV(var23, var5 + 1.0f, var27, var15, var17);
		var21 = var3 + 0.5f - 0.5f;
		var23 = var3 + 0.5f + 0.5f;
		var25 = var7 + 0.5f - 0.25f;
		var27 = var7 + 0.5f + 0.25f;
		var9.vertexUV(var21, var5 + 1.0f, var25, var13, var17);
		var9.vertexUV(var21, var5 + 0.0f, var25, var13, var19);
		var9.vertexUV(var23, var5 + 0.0f, var25, var15, var19);
		var9.vertexUV(var23, var5 + 1.0f, var25, var15, var17);
		var9.vertexUV(var23, var5 + 1.0f, var25, var13, var17);
		var9.vertexUV(var23, var5 + 0.0f, var25, var13, var19);
		var9.vertexUV(var21, var5 + 0.0f, var25, var15, var19);
		var9.vertexUV(var21, var5 + 1.0f, var25, var15, var17);
		var9.vertexUV(var23, var5 + 1.0f, var27, var13, var17);
		var9.vertexUV(var23, var5 + 0.0f, var27, var13, var19);
		var9.vertexUV(var21, var5 + 0.0f, var27, var15, var19);
		var9.vertexUV(var21, var5 + 1.0f, var27, var15, var17);
		var9.vertexUV(var21, var5 + 1.0f, var27, var13, var17);
		var9.vertexUV(var21, var5 + 0.0f, var27, var13, var19);
		var9.vertexUV(var23, var5 + 0.0f, var27, var15, var19);
		var9.vertexUV(var23, var5 + 1.0f, var27, var15, var17);
}

bool RenderBlocks::renderBlockFluids(Block* var1, int var2, int var3, int var4) {
		Tesselator& var5 = Tesselator::getInstance();
		bool var6 = var1->shouldSideBeRendered(m_blockAccess, var2, var3 + 1, var4, 1);
		bool var7 = var1->shouldSideBeRendered(m_blockAccess, var2, var3 - 1, var4, 0);
		bool var8[4] = {
			var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 - 1, 2),
			var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 + 1, 3),
			var1->shouldSideBeRendered(m_blockAccess, var2 - 1, var3, var4, 4),
			var1->shouldSideBeRendered(m_blockAccess, var2 + 1, var3, var4, 5)
		};
		if(!var6 && !var7 && !var8[0] && !var8[1] && !var8[2] && !var8[3]) {
			return false;
		} else {
			bool var9 = false;
			float var10 = 0.5F;
			float var11 = 1.0F;
			float var12 = 0.8F;
			float var13 = 0.6F;
			double var14 = 0.0f;
			double var16 = 1.0f;
			Material* var18 = var1->m_blockMaterial;
			int var19 = m_blockAccess->getBlockMetadata(var2, var3, var4);
			float var20 = func_1224_a(var2, var3, var4, var18);
			float var21 = func_1224_a(var2, var3, var4 + 1, var18);
			float var22 = func_1224_a(var2 + 1, var3, var4 + 1, var18);
			float var23 = func_1224_a(var2 + 1, var3, var4, var18);
			int var24;
			int var27;
			float var32;
			float var33;
			float var34;
			if(m_renderAllFaces || var6) {
				var9 = true;
				var24 = var1->getBlockTextureFromSideAndMetadata(1, var19);
				float var25 = (float)BlockFluids::func_293_a(m_blockAccess, var2, var3, var4, var18);
				if(var25 > -999.0F) {
					var24 = var1->getBlockTextureFromSideAndMetadata(2, var19);
				}

				int var26 = (var24 & 15) << 4;
				var27 = var24 & 240;
				double var28 = ((double)var26 + 8.0f) / 256.0f;
				double var30 = ((double)var27 + 8.0f) / 256.0f;
				if(var25 < -999.0F) {
					var25 = 0.0F;
				} else {
					var28 = (double)((float)(var26 + 16) / 256.0F);
					var30 = (double)((float)(var27 + 16) / 256.0F);
				}

				var32 = sinf(var25) * 8.0F / 256.0F;
				var33 = cosf(var25) * 8.0F / 256.0F;
				var34 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
				var5.color3f(var11 * var34, var11 * var34, var11 * var34);
				var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var20), (double)(var4 + 0), var28 - (double)var33 - (double)var32, var30 - (double)var33 + (double)var32);
				var5.vertexUV((double)(var2 + 0), (double)((float)var3 + var21), (double)(var4 + 1), var28 - (double)var33 + (double)var32, var30 + (double)var33 + (double)var32);
				var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var22), (double)(var4 + 1), var28 + (double)var33 + (double)var32, var30 + (double)var33 - (double)var32);
				var5.vertexUV((double)(var2 + 1), (double)((float)var3 + var23), (double)(var4 + 0), var28 + (double)var33 - (double)var32, var30 - (double)var33 - (double)var32);
			}

			if(m_renderAllFaces || var7) {
				float var48 = var1->getBlockBrightness(m_blockAccess, var2, var3 - 1, var4);
				var5.color3f(var10 * var48, var10 * var48, var10 * var48);
				renderBottomFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTextureFromSide(0));
				var9 = true;
			}

			for(var24 = 0; var24 < 4; ++var24) {
				int var49 = var2;
				var27 = var4;
				if(var24 == 0) {
					var27 = var4 - 1;
				}

				if(var24 == 1) {
					++var27;
				}

				if(var24 == 2) {
					var49 = var2 - 1;
				}

				if(var24 == 3) {
					++var49;
				}

				int var50 = var1->getBlockTextureFromSideAndMetadata(var24 + 2, var19);
				int var29 = (var50 & 15) << 4;
				int var51 = var50 & 240;
				if(m_renderAllFaces || var8[var24]) {
					float var31;
					float var35;
					float var36;
					if(var24 == 0) {
						var31 = var20;
						var32 = var23;
						var33 = (float)var2;
						var35 = (float)(var2 + 1);
						var34 = (float)var4;
						var36 = (float)var4;
					} else if(var24 == 1) {
						var31 = var22;
						var32 = var21;
						var33 = (float)(var2 + 1);
						var35 = (float)var2;
						var34 = (float)(var4 + 1);
						var36 = (float)(var4 + 1);
					} else if(var24 == 2) {
						var31 = var21;
						var32 = var20;
						var33 = (float)var2;
						var35 = (float)var2;
						var34 = (float)(var4 + 1);
						var36 = (float)var4;
					} else {
						var31 = var23;
						var32 = var22;
						var33 = (float)(var2 + 1);
						var35 = (float)(var2 + 1);
						var34 = (float)var4;
						var36 = (float)(var4 + 1);
					}

					var9 = true;
					double var37 = (double)((float)(var29 + 0) / 256.0F);
					double var39 = ((double)(var29 + 16) - 0.01f) / 256.0f;
					double var41 = (double)(((float)var51 + (1.0F - var31) * 16.0F) / 256.0F);
					double var43 = (double)(((float)var51 + (1.0F - var32) * 16.0F) / 256.0F);
					double var45 = ((double)(var51 + 16) - 0.01f) / 256.0f;
					float var47 = var1->getBlockBrightness(m_blockAccess, var49, var3, var27);
					if(var24 < 2) {
						var47 *= var12;
					} else {
						var47 *= var13;
					}

					var5.color3f(var11 * var47, var11 * var47, var11 * var47);
					var5.vertexUV((double)var33, (double)((float)var3 + var31), (double)var34, var37, var41);
					var5.vertexUV((double)var35, (double)((float)var3 + var32), (double)var36, var39, var43);
					var5.vertexUV((double)var35, (double)(var3 + 0), (double)var36, var39, var45);
					var5.vertexUV((double)var33, (double)(var3 + 0), (double)var34, var37, var45);
				}
			}

			var1->minY = var14;
			var1->maxY = var16;
			return var9;
		}
}

float RenderBlocks::func_1224_a(int var1, int var2, int var3, Material* var4) {
		int var5 = 0;
		float var6 = 0.0F;

		for(int var7 = 0; var7 < 4; ++var7) {
			int var8 = var1 - (var7 & 1);
			int var10 = var3 - (var7 >> 1 & 1);
			if(m_blockAccess->getBlockMaterial(var8, var2 + 1, var10) == var4) {
				return 1.0F;
			}

			Material* var11 = m_blockAccess->getBlockMaterial(var8, var2, var10);
			if(var11 != var4) {
				if(!var11->func_878_a()) {
					++var6;
					++var5;
				}
			} else {
				int var12 = m_blockAccess->getBlockMetadata(var8, var2, var10);
				if(var12 >= 8 || var12 == 0) {
					var6 += BlockFluids::func_288_b(var12) * 10.0F;
					var5 += 10;
				}

				var6 += BlockFluids::func_288_b(var12);
				++var5;
			}
		}

		return 1.0F - var6 / (float)var5;
}

void RenderBlocks::func_1243_a(Block* var1, World* var2, int var3, int var4, int var5) {
		float var6 = 0.5F;
		float var7 = 1.0F;
		float var8 = 0.8F;
		float var9 = 0.6F;
		Tesselator& var10 = Tesselator::getInstance();
		var10.init();
		float var11 = var1->getBlockBrightness(var2, var3, var4, var5);
		float var12 = var1->getBlockBrightness(var2, var3, var4 - 1, var5);
		if(var12 < var11) {
			var12 = var11;
		}

		var10.color3f(var6 * var12, var6 * var12, var6 * var12);
		renderBottomFace(var1, -0.5f, -0.5f, -0.5f, var1->getBlockTextureFromSide(0));
		var12 = var1->getBlockBrightness(var2, var3, var4 + 1, var5);
		if(var12 < var11) {
			var12 = var11;
		}

		var10.color3f(var7 * var12, var7 * var12, var7 * var12);
		renderTopFace(var1, -0.5f, -0.5f, -0.5f, var1->getBlockTextureFromSide(1));
		var12 = var1->getBlockBrightness(var2, var3, var4, var5 - 1);
		if(var12 < var11) {
			var12 = var11;
		}

		var10.color3f(var8 * var12, var8 * var12, var8 * var12);
		renderEastFace(var1, -0.5f, -0.5f, -0.5f, var1->getBlockTextureFromSide(2));
		var12 = var1->getBlockBrightness(var2, var3, var4, var5 + 1);
		if(var12 < var11) {
			var12 = var11;
		}

		var10.color3f(var8 * var12, var8 * var12, var8 * var12);
		renderWestFace(var1, -0.5f, -0.5f, -0.5f, var1->getBlockTextureFromSide(3));
		var12 = var1->getBlockBrightness(var2, var3 - 1, var4, var5);
		if(var12 < var11) {
			var12 = var11;
		}

		var10.color3f(var9 * var12, var9 * var12, var9 * var12);
		renderNorthFace(var1, -0.5f, -0.5f, -0.5f, var1->getBlockTextureFromSide(4));
		var12 = var1->getBlockBrightness(var2, var3 + 1, var4, var5);
		if(var12 < var11) {
			var12 = var11;
		}

		var10.color3f(var9 * var12, var9 * var12, var9 * var12);
		renderSouthFace(var1, -0.5f, -0.5f, -0.5f, var1->getBlockTextureFromSide(5));
		var10.flush();
}

bool RenderBlocks::renderStandardBlock(Block* var1, int var2, int var3, int var4) {
		int var5 = var1->colorMultiplier(m_blockAccess, var2, var3, var4);
		float var6 = (float)(var5 >> 16 & 255) / 255.0F;
		float var7 = (float)(var5 >> 8 & 255) / 255.0F;
		float var8 = (float)(var5 & 255) / 255.0F;
		return renderStandardBlockWithColorMultiplier(var1, var2, var3, var4, var6, var7, var8);
}

bool RenderBlocks::renderStandardBlockWithColorMultiplier(Block* var1, int var2, int var3, int var4, float var5, float var6, float var7) {
		Tesselator& var8 = Tesselator::getInstance();
		bool var9 = false;
		float var10 = 0.5F;
		float var11 = 1.0F;
		float var12 = 0.8F;
		float var13 = 0.6F;
		float var14 = var11 * var5;
		float var15 = var11 * var6;
		float var16 = var11 * var7;
		if(var1 == (Block*)Block::s_grass) {
			var7 = 1.0F;
			var6 = var7;
			var5 = var7;
		}

		float var17 = var10 * var5;
		float var18 = var12 * var5;
		float var19 = var13 * var5;
		float var20 = var10 * var6;
		float var21 = var12 * var6;
		float var22 = var13 * var6;
		float var23 = var10 * var7;
		float var24 = var12 * var7;
		float var25 = var13 * var7;
		float var26 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		float var27;
		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2, var3 - 1, var4, 0)) {
			var27 = var1->getBlockBrightness(m_blockAccess, var2, var3 - 1, var4);
			var8.color3f(var17 * var27, var20 * var27, var23 * var27);
			renderBottomFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 0));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2, var3 + 1, var4, 1)) {
			var27 = var1->getBlockBrightness(m_blockAccess, var2, var3 + 1, var4);
			if(var1->maxY != 1.0f && !var1->m_blockMaterial->getIsLiquid()) {
				var27 = var26;
			}

			var8.color3f(var14 * var27, var15 * var27, var16 * var27);
			renderTopFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 1));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 - 1, 2)) {
			var27 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 - 1);
			if(var1->minZ > 0.0f) {
				var27 = var26;
			}

			var8.color3f(var18 * var27, var21 * var27, var24 * var27);
			renderEastFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 2));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 + 1, 3)) {
			var27 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 + 1);
			if(var1->maxZ < 1.0f) {
				var27 = var26;
			}

			var8.color3f(var18 * var27, var21 * var27, var24 * var27);
			renderWestFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 3));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2 - 1, var3, var4, 4)) {
			var27 = var1->getBlockBrightness(m_blockAccess, var2 - 1, var3, var4);
			if(var1->m_field_370_bf > 0.0f) {
				var27 = var26;
			}

			var8.color3f(var19 * var27, var22 * var27, var25 * var27);
			renderNorthFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 4));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2 + 1, var3, var4, 5)) {
			var27 = var1->getBlockBrightness(m_blockAccess, var2 + 1, var3, var4);
			if(var1->maxX < 1.0f) {
				var27 = var26;
			}

			var8.color3f(var19 * var27, var22 * var27, var25 * var27);
			renderSouthFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 5));
			var9 = true;
		}

		return var9;
}

bool RenderBlocks::renderBlockCactus(Block* var1, int var2, int var3, int var4) {
		int var5 = var1->colorMultiplier(m_blockAccess, var2, var3, var4);
		float var6 = (float)(var5 >> 16 & 255) / 255.0F;
		float var7 = (float)(var5 >> 8 & 255) / 255.0F;
		float var8 = (float)(var5 & 255) / 255.0F;
		return func_1230_b(var1, var2, var3, var4, var6, var7, var8);
}

bool RenderBlocks::func_1230_b(Block* var1, int var2, int var3, int var4, float var5, float var6, float var7) {
		Tesselator& var8 = Tesselator::getInstance();
		bool var9 = false;
		float var10 = 0.5F;
		float var11 = 1.0F;
		float var12 = 0.8F;
		float var13 = 0.6F;
		float var14 = var10 * var5;
		float var15 = var11 * var5;
		float var16 = var12 * var5;
		float var17 = var13 * var5;
		float var18 = var10 * var6;
		float var19 = var11 * var6;
		float var20 = var12 * var6;
		float var21 = var13 * var6;
		float var22 = var10 * var7;
		float var23 = var11 * var7;
		float var24 = var12 * var7;
		float var25 = var13 * var7;
		float var26 = 1.0F / 16.0F;
		float var27 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		float var28;
		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2, var3 - 1, var4, 0)) {
			var28 = var1->getBlockBrightness(m_blockAccess, var2, var3 - 1, var4);
			var8.color3f(var14 * var28, var18 * var28, var22 * var28);
			renderBottomFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 0));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2, var3 + 1, var4, 1)) {
			var28 = var1->getBlockBrightness(m_blockAccess, var2, var3 + 1, var4);
			if(var1->maxY != 1.0f && !var1->m_blockMaterial->getIsLiquid()) {
				var28 = var27;
			}

			var8.color3f(var15 * var28, var19 * var28, var23 * var28);
			renderTopFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 1));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 - 1, 2)) {
			var28 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 - 1);
			if(var1->minZ > 0.0f) {
				var28 = var27;
			}

			var8.color3f(var16 * var28, var20 * var28, var24 * var28);
			// PORT: Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
			renderEastFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 2));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 + 1, 3)) {
			var28 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 + 1);
			if(var1->maxZ < 1.0f) {
				var28 = var27;
			}

			var8.color3f(var16 * var28, var20 * var28, var24 * var28);
			// PORT: Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
			renderWestFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 3));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2 - 1, var3, var4, 4)) {
			var28 = var1->getBlockBrightness(m_blockAccess, var2 - 1, var3, var4);
			if(var1->m_field_370_bf > 0.0f) {
				var28 = var27;
			}

			var8.color3f(var17 * var28, var21 * var28, var25 * var28);
			// PORT: Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
			renderNorthFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 4));
			var9 = true;
		}

		if(m_renderAllFaces || var1->shouldSideBeRendered(m_blockAccess, var2 + 1, var3, var4, 5)) {
			var28 = var1->getBlockBrightness(m_blockAccess, var2 + 1, var3, var4);
			if(var1->maxX < 1.0f) {
				var28 = var27;
			}

			var8.color3f(var17 * var28, var21 * var28, var25 * var28);
			// PORT: Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
			renderSouthFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 5));
			var9 = true;
		}

		(void)var26;
		return var9;
}

bool RenderBlocks::renderBlockFence(Block* var1, int var2, int var3, int var4) {
		bool var5 = false;
		float var6 = 6.0F / 16.0F;
		float var7 = 10.0F / 16.0F;
		var1->setBlockBounds(var6, 0.0F, var6, var7, 1.0F, var7);
		renderStandardBlock(var1, var2, var3, var4);
		bool var8 = false;
		bool var9 = false;
		if(m_blockAccess->getBlockId(var2 - 1, var3, var4) == var1->blockID || m_blockAccess->getBlockId(var2 + 1, var3, var4) == var1->blockID) {
			var8 = true;
		}

		if(m_blockAccess->getBlockId(var2, var3, var4 - 1) == var1->blockID || m_blockAccess->getBlockId(var2, var3, var4 + 1) == var1->blockID) {
			var9 = true;
		}

		bool var10 = m_blockAccess->getBlockId(var2 - 1, var3, var4) == var1->blockID;
		bool var11 = m_blockAccess->getBlockId(var2 + 1, var3, var4) == var1->blockID;
		bool var12 = m_blockAccess->getBlockId(var2, var3, var4 - 1) == var1->blockID;
		bool var13 = m_blockAccess->getBlockId(var2, var3, var4 + 1) == var1->blockID;
		if(!var8 && !var9) {
			var8 = true;
		}

		var6 = 7.0F / 16.0F;
		var7 = 9.0F / 16.0F;
		float var14 = 12.0F / 16.0F;
		float var15 = 15.0F / 16.0F;
		float var16 = var10 ? 0.0F : var6;
		float var17 = var11 ? 1.0F : var7;
		float var18 = var12 ? 0.0F : var6;
		float var19 = var13 ? 1.0F : var7;
		if(var8) {
			var1->setBlockBounds(var16, var14, var6, var17, var15, var7);
			renderStandardBlock(var1, var2, var3, var4);
		}

		if(var9) {
			var1->setBlockBounds(var6, var14, var18, var7, var15, var19);
			renderStandardBlock(var1, var2, var3, var4);
		}

		var14 = 6.0F / 16.0F;
		var15 = 9.0F / 16.0F;
		if(var8) {
			var1->setBlockBounds(var16, var14, var6, var17, var15, var7);
			renderStandardBlock(var1, var2, var3, var4);
		}

		if(var9) {
			var1->setBlockBounds(var6, var14, var18, var7, var15, var19);
			renderStandardBlock(var1, var2, var3, var4);
		}

		var1->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
		return var5;
}

bool RenderBlocks::renderBlockStairs(Block* var1, int var2, int var3, int var4) {
		bool var5 = false;
		int var6 = m_blockAccess->getBlockMetadata(var2, var3, var4);
		if(var6 == 0) {
			var1->setBlockBounds(0.0F, 0.0F, 0.0F, 0.5F, 0.5F, 1.0F);
			renderStandardBlock(var1, var2, var3, var4);
			var1->setBlockBounds(0.5F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
			renderStandardBlock(var1, var2, var3, var4);
		} else if(var6 == 1) {
			var1->setBlockBounds(0.0F, 0.0F, 0.0F, 0.5F, 1.0F, 1.0F);
			renderStandardBlock(var1, var2, var3, var4);
			var1->setBlockBounds(0.5F, 0.0F, 0.0F, 1.0F, 0.5F, 1.0F);
			renderStandardBlock(var1, var2, var3, var4);
		} else if(var6 == 2) {
			var1->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 0.5F, 0.5F);
			renderStandardBlock(var1, var2, var3, var4);
			var1->setBlockBounds(0.0F, 0.0F, 0.5F, 1.0F, 1.0F, 1.0F);
			renderStandardBlock(var1, var2, var3, var4);
		} else if(var6 == 3) {
			var1->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.5F);
			renderStandardBlock(var1, var2, var3, var4);
			var1->setBlockBounds(0.0F, 0.0F, 0.5F, 1.0F, 0.5F, 1.0F);
			renderStandardBlock(var1, var2, var3, var4);
		}

		var1->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
		return var5;
}

bool RenderBlocks::renderBlockDoor(Block* var1, int var2, int var3, int var4) {
		Tesselator& var5 = Tesselator::getInstance();
		BlockDoor* var6 = (BlockDoor*)var1;
		bool var7 = false;
		float var8 = 0.5F;
		float var9 = 1.0F;
		float var10 = 0.8F;
		float var11 = 0.6F;
		float var12 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
		float var13 = var1->getBlockBrightness(m_blockAccess, var2, var3 - 1, var4);
		if(var6->minY > 0.0f) {
			var13 = var12;
		}

		if(Block::s_lightValue[var1->blockID] > 0) {
			var13 = 1.0F;
		}

		var5.color3f(var8 * var13, var8 * var13, var8 * var13);
		renderBottomFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 0));
		var7 = true;
		var13 = var1->getBlockBrightness(m_blockAccess, var2, var3 + 1, var4);
		if(var6->maxY < 1.0f) {
			var13 = var12;
		}

		if(Block::s_lightValue[var1->blockID] > 0) {
			var13 = 1.0F;
		}

		var5.color3f(var9 * var13, var9 * var13, var9 * var13);
		renderTopFace(var1, (double)var2, (double)var3, (double)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 1));
		var7 = true;
		var13 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 - 1);
		if(var6->minZ > 0.0f) {
			var13 = var12;
		}

		if(Block::s_lightValue[var1->blockID] > 0) {
			var13 = 1.0F;
		}

		var5.color3f(var10 * var13, var10 * var13, var10 * var13);
		int var14 = var1->getBlockTexture(m_blockAccess, var2, var3, var4, 2);
		if(var14 < 0) {
			m_flipTexture = true;
			var14 = -var14;
		}

		renderEastFace(var1, (double)var2, (double)var3, (double)var4, var14);
		var7 = true;
		m_flipTexture = false;
		var13 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 + 1);
		if(var6->maxZ < 1.0f) {
			var13 = var12;
		}

		if(Block::s_lightValue[var1->blockID] > 0) {
			var13 = 1.0F;
		}

		var5.color3f(var10 * var13, var10 * var13, var10 * var13);
		var14 = var1->getBlockTexture(m_blockAccess, var2, var3, var4, 3);
		if(var14 < 0) {
			m_flipTexture = true;
			var14 = -var14;
		}

		renderWestFace(var1, (double)var2, (double)var3, (double)var4, var14);
		var7 = true;
		m_flipTexture = false;
		var13 = var1->getBlockBrightness(m_blockAccess, var2 - 1, var3, var4);
		if(var6->m_field_370_bf > 0.0f) {
			var13 = var12;
		}

		if(Block::s_lightValue[var1->blockID] > 0) {
			var13 = 1.0F;
		}

		var5.color3f(var11 * var13, var11 * var13, var11 * var13);
		var14 = var1->getBlockTexture(m_blockAccess, var2, var3, var4, 4);
		if(var14 < 0) {
			m_flipTexture = true;
			var14 = -var14;
		}

		renderNorthFace(var1, (double)var2, (double)var3, (double)var4, var14);
		var7 = true;
		m_flipTexture = false;
		var13 = var1->getBlockBrightness(m_blockAccess, var2 + 1, var3, var4);
		if(var6->maxX < 1.0f) {
			var13 = var12;
		}

		if(Block::s_lightValue[var1->blockID] > 0) {
			var13 = 1.0F;
		}

		var5.color3f(var11 * var13, var11 * var13, var11 * var13);
		var14 = var1->getBlockTexture(m_blockAccess, var2, var3, var4, 5);
		if(var14 < 0) {
			m_flipTexture = true;
			var14 = -var14;
		}

		renderSouthFace(var1, (double)var2, (double)var3, (double)var4, var14);
		var7 = true;
		m_flipTexture = false;
		return var7;
}

void RenderBlocks::renderBottomFace(Block* var1, double var2, double var4, double var6, int var8) {
		Tesselator& var9 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var8 = m_overrideBlockTexture;
		}

		int var10 = (var8 & 15) << 4;
		int var11 = var8 & 240;
		double var12 = ((double)var10 + var1->m_field_370_bf * 16.0f) / 256.0f;
		double var14 = ((double)var10 + var1->maxX * 16.0f - 0.01f) / 256.0f;
		double var16 = ((double)var11 + var1->minZ * 16.0f) / 256.0f;
		double var18 = ((double)var11 + var1->maxZ * 16.0f - 0.01f) / 256.0f;
		if(var1->m_field_370_bf < 0.0f || var1->maxX > 1.0f) {
			var12 = (double)(((float)var10 + 0.0F) / 256.0F);
			var14 = (double)(((float)var10 + 15.99F) / 256.0F);
		}

		if(var1->minZ < 0.0f || var1->maxZ > 1.0f) {
			var16 = (double)(((float)var11 + 0.0F) / 256.0F);
			var18 = (double)(((float)var11 + 15.99F) / 256.0F);
		}

		double var20 = var2 + var1->m_field_370_bf;
		double var22 = var2 + var1->maxX;
		double var24 = var4 + var1->minY;
		double var26 = var6 + var1->minZ;
		double var28 = var6 + var1->maxZ;
		var9.vertexUV(var20, var24, var28, var12, var18);
		var9.vertexUV(var20, var24, var26, var12, var16);
		var9.vertexUV(var22, var24, var26, var14, var16);
		var9.vertexUV(var22, var24, var28, var14, var18);
}

void RenderBlocks::renderTopFace(Block* var1, double var2, double var4, double var6, int var8) {
		Tesselator& var9 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var8 = m_overrideBlockTexture;
		}

		int var10 = (var8 & 15) << 4;
		int var11 = var8 & 240;
		double var12 = ((double)var10 + var1->m_field_370_bf * 16.0f) / 256.0f;
		double var14 = ((double)var10 + var1->maxX * 16.0f - 0.01f) / 256.0f;
		double var16 = ((double)var11 + var1->minZ * 16.0f) / 256.0f;
		double var18 = ((double)var11 + var1->maxZ * 16.0f - 0.01f) / 256.0f;
		if(var1->m_field_370_bf < 0.0f || var1->maxX > 1.0f) {
			var12 = (double)(((float)var10 + 0.0F) / 256.0F);
			var14 = (double)(((float)var10 + 15.99F) / 256.0F);
		}

		if(var1->minZ < 0.0f || var1->maxZ > 1.0f) {
			var16 = (double)(((float)var11 + 0.0F) / 256.0F);
			var18 = (double)(((float)var11 + 15.99F) / 256.0F);
		}

		double var20 = var2 + var1->m_field_370_bf;
		double var22 = var2 + var1->maxX;
		double var24 = var4 + var1->maxY;
		double var26 = var6 + var1->minZ;
		double var28 = var6 + var1->maxZ;
		var9.vertexUV(var22, var24, var28, var14, var18);
		var9.vertexUV(var22, var24, var26, var14, var16);
		var9.vertexUV(var20, var24, var26, var12, var16);
		var9.vertexUV(var20, var24, var28, var12, var18);
}

void RenderBlocks::renderEastFace(Block* var1, double var2, double var4, double var6, int var8) {
		Tesselator& var9 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var8 = m_overrideBlockTexture;
		}

		int var10 = (var8 & 15) << 4;
		int var11 = var8 & 240;
		double var12 = ((double)var10 + var1->m_field_370_bf * 16.0f) / 256.0f;
		double var14 = ((double)var10 + var1->maxX * 16.0f - 0.01f) / 256.0f;
		double var16 = ((double)var11 + var1->minY * 16.0f) / 256.0f;
		double var18 = ((double)var11 + var1->maxY * 16.0f - 0.01f) / 256.0f;
		double var20;
		if(m_flipTexture) {
			var20 = var12;
			var12 = var14;
			var14 = var20;
		}

		if(var1->m_field_370_bf < 0.0f || var1->maxX > 1.0f) {
			var12 = (double)(((float)var10 + 0.0F) / 256.0F);
			var14 = (double)(((float)var10 + 15.99F) / 256.0F);
		}

		if(var1->minY < 0.0f || var1->maxY > 1.0f) {
			var16 = (double)(((float)var11 + 0.0F) / 256.0F);
			var18 = (double)(((float)var11 + 15.99F) / 256.0F);
		}

		var20 = var2 + var1->m_field_370_bf;
		double var22 = var2 + var1->maxX;
		double var24 = var4 + var1->minY;
		double var26 = var4 + var1->maxY;
		double var28 = var6 + var1->minZ;
		var9.vertexUV(var20, var26, var28, var14, var16);
		var9.vertexUV(var22, var26, var28, var12, var16);
		var9.vertexUV(var22, var24, var28, var12, var18);
		var9.vertexUV(var20, var24, var28, var14, var18);
}

void RenderBlocks::renderWestFace(Block* var1, double var2, double var4, double var6, int var8) {
		Tesselator& var9 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var8 = m_overrideBlockTexture;
		}

		int var10 = (var8 & 15) << 4;
		int var11 = var8 & 240;
		double var12 = ((double)var10 + var1->m_field_370_bf * 16.0f) / 256.0f;
		double var14 = ((double)var10 + var1->maxX * 16.0f - 0.01f) / 256.0f;
		double var16 = ((double)var11 + var1->minY * 16.0f) / 256.0f;
		double var18 = ((double)var11 + var1->maxY * 16.0f - 0.01f) / 256.0f;
		double var20;
		if(m_flipTexture) {
			var20 = var12;
			var12 = var14;
			var14 = var20;
		}

		if(var1->m_field_370_bf < 0.0f || var1->maxX > 1.0f) {
			var12 = (double)(((float)var10 + 0.0F) / 256.0F);
			var14 = (double)(((float)var10 + 15.99F) / 256.0F);
		}

		if(var1->minY < 0.0f || var1->maxY > 1.0f) {
			var16 = (double)(((float)var11 + 0.0F) / 256.0F);
			var18 = (double)(((float)var11 + 15.99F) / 256.0F);
		}

		var20 = var2 + var1->m_field_370_bf;
		double var22 = var2 + var1->maxX;
		double var24 = var4 + var1->minY;
		double var26 = var4 + var1->maxY;
		double var28 = var6 + var1->maxZ;
		var9.vertexUV(var20, var26, var28, var12, var16);
		var9.vertexUV(var20, var24, var28, var12, var18);
		var9.vertexUV(var22, var24, var28, var14, var18);
		var9.vertexUV(var22, var26, var28, var14, var16);
}

void RenderBlocks::renderNorthFace(Block* var1, double var2, double var4, double var6, int var8) {
		Tesselator& var9 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var8 = m_overrideBlockTexture;
		}

		int var10 = (var8 & 15) << 4;
		int var11 = var8 & 240;
		double var12 = ((double)var10 + var1->minZ * 16.0f) / 256.0f;
		double var14 = ((double)var10 + var1->maxZ * 16.0f - 0.01f) / 256.0f;
		double var16 = ((double)var11 + var1->minY * 16.0f) / 256.0f;
		double var18 = ((double)var11 + var1->maxY * 16.0f - 0.01f) / 256.0f;
		double var20;
		if(m_flipTexture) {
			var20 = var12;
			var12 = var14;
			var14 = var20;
		}

		if(var1->minZ < 0.0f || var1->maxZ > 1.0f) {
			var12 = (double)(((float)var10 + 0.0F) / 256.0F);
			var14 = (double)(((float)var10 + 15.99F) / 256.0F);
		}

		if(var1->minY < 0.0f || var1->maxY > 1.0f) {
			var16 = (double)(((float)var11 + 0.0F) / 256.0F);
			var18 = (double)(((float)var11 + 15.99F) / 256.0F);
		}

		var20 = var2 + var1->m_field_370_bf;
		double var22 = var4 + var1->minY;
		double var24 = var4 + var1->maxY;
		double var26 = var6 + var1->minZ;
		double var28 = var6 + var1->maxZ;
		var9.vertexUV(var20, var24, var28, var14, var16);
		var9.vertexUV(var20, var24, var26, var12, var16);
		var9.vertexUV(var20, var22, var26, var12, var18);
		var9.vertexUV(var20, var22, var28, var14, var18);
}

void RenderBlocks::renderSouthFace(Block* var1, double var2, double var4, double var6, int var8) {
		Tesselator& var9 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var8 = m_overrideBlockTexture;
		}

		int var10 = (var8 & 15) << 4;
		int var11 = var8 & 240;
		double var12 = ((double)var10 + var1->minZ * 16.0f) / 256.0f;
		double var14 = ((double)var10 + var1->maxZ * 16.0f - 0.01f) / 256.0f;
		double var16 = ((double)var11 + var1->minY * 16.0f) / 256.0f;
		double var18 = ((double)var11 + var1->maxY * 16.0f - 0.01f) / 256.0f;
		double var20;
		if(m_flipTexture) {
			var20 = var12;
			var12 = var14;
			var14 = var20;
		}

		if(var1->minZ < 0.0f || var1->maxZ > 1.0f) {
			var12 = (double)(((float)var10 + 0.0F) / 256.0F);
			var14 = (double)(((float)var10 + 15.99F) / 256.0F);
		}

		if(var1->minY < 0.0f || var1->maxY > 1.0f) {
			var16 = (double)(((float)var11 + 0.0F) / 256.0F);
			var18 = (double)(((float)var11 + 15.99F) / 256.0F);
		}

		var20 = var2 + var1->maxX;
		double var22 = var4 + var1->minY;
		double var24 = var4 + var1->maxY;
		double var26 = var6 + var1->minZ;
		double var28 = var6 + var1->maxZ;
		var9.vertexUV(var20, var22, var28, var12, var18);
		var9.vertexUV(var20, var22, var26, var14, var18);
		var9.vertexUV(var20, var24, var26, var14, var16);
		var9.vertexUV(var20, var24, var28, var12, var16);
}

void RenderBlocks::func_1238_a(Block* var1, float var2) {
		int var3 = var1->getRenderType();
		Tesselator& var4 = Tesselator::getInstance();
		if(var3 == 0) {
			var1->func_237_e();
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -0.5F, -0.5F, -0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			float var5 = 0.5F;
			float var6 = 1.0F;
			float var7 = 0.8F;
			float var8 = 0.6F;
			var4.init();
			var4.color4f(var6, var6, var6, var2);
			renderBottomFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(0));
			var4.color4f(var5, var5, var5, var2);
			renderTopFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(1));
			var4.color4f(var7, var7, var7, var2);
			renderEastFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(2));
			renderWestFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(3));
			var4.color4f(var8, var8, var8, var2);
			renderNorthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(4));
			renderSouthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(5));
			var4.flush();
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.5F, 0.5F, 0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

}

void RenderBlocks::func_1227_a(Block* var1) {
		int var2 = -1;
		Tesselator& var3 = Tesselator::getInstance();
		int var4 = var1->getRenderType();
		if(var4 == 0) {
			var1->func_237_e();
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -0.5F, -0.5F, -0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var3.init();
			var3.normal(0.0F, -1.0F, 0.0F);
			renderBottomFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(0));
			var3.flush();
			var3.init();
			var3.normal(0.0F, 1.0F, 0.0F);
			renderTopFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(1));
			var3.flush();
			var3.init();
			var3.normal(0.0F, 0.0F, -1.0F);
			renderEastFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(2));
			var3.flush();
			var3.init();
			var3.normal(0.0F, 0.0F, 1.0F);
			renderWestFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(3));
			var3.flush();
			var3.init();
			var3.normal(-1.0F, 0.0F, 0.0F);
			renderNorthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(4));
			var3.flush();
			var3.init();
			var3.normal(1.0F, 0.0F, 0.0F);
			renderSouthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(5));
			var3.flush();
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.5F, 0.5F, 0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		} else if(var4 == 1) {
			var3.init();
			var3.normal(0.0F, -1.0F, 0.0F);
			func_1239_a(var1, var2, -0.5f, -0.5f, -0.5f);
			var3.flush();
		} else if(var4 == 13) {
			var1->func_237_e();
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -0.5F, -0.5F, -0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			float var5 = 1.0F / 16.0F;
			var3.init();
			var3.normal(0.0F, -1.0F, 0.0F);
			renderBottomFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(0));
			var3.flush();
			var3.init();
			var3.normal(0.0F, 1.0F, 0.0F);
			renderTopFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(1));
			var3.flush();
			var3.init();
			var3.normal(0.0F, 0.0F, -1.0F);
			// PORT: Tesselator has no setTranslationF; cactus 1px inset dropped (cosmetic).
			renderEastFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(2));
			var3.flush();
			var3.init();
			var3.normal(0.0F, 0.0F, 1.0F);
			renderWestFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(3));
			var3.flush();
			var3.init();
			var3.normal(-1.0F, 0.0F, 0.0F);
			renderNorthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(4));
			var3.flush();
			var3.init();
			var3.normal(1.0F, 0.0F, 0.0F);
			renderSouthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(5));
			var3.flush();
			(void)var5;
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.5F, 0.5F, 0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		} else if(var4 == 6) {
			var3.init();
			var3.normal(0.0F, -1.0F, 0.0F);
			func_1245_b(var1, var2, -0.5f, -0.5f, -0.5f);
			var3.flush();
		} else if(var4 == 2) {
			var3.init();
			var3.normal(0.0F, -1.0F, 0.0F);
			renderTorchAtAngle(var1, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
			var3.flush();
		} else {
			int var7;
			if(var4 == 10) {
				for(var7 = 0; var7 < 2; ++var7) {
					if(var7 == 0) {
						var1->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.5F);
					}

					if(var7 == 1) {
						var1->setBlockBounds(0.0F, 0.0F, 0.5F, 1.0F, 0.5F, 1.0F);
					}

     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -0.5F, -0.5F, -0.5F);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
					var3.init();
					var3.normal(0.0F, -1.0F, 0.0F);
					renderBottomFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(0));
					var3.flush();
					var3.init();
					var3.normal(0.0F, 1.0F, 0.0F);
					renderTopFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(1));
					var3.flush();
					var3.init();
					var3.normal(0.0F, 0.0F, -1.0F);
					renderEastFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(2));
					var3.flush();
					var3.init();
					var3.normal(0.0F, 0.0F, 1.0F);
					renderWestFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(3));
					var3.flush();
					var3.init();
					var3.normal(-1.0F, 0.0F, 0.0F);
					renderNorthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(4));
					var3.flush();
					var3.init();
					var3.normal(1.0F, 0.0F, 0.0F);
					renderSouthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(5));
					var3.flush();
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.5F, 0.5F, 0.5F);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}
			} else if(var4 == 11) {
				for(var7 = 0; var7 < 4; ++var7) {
					float var6 = 2.0F / 16.0F;
					if(var7 == 0) {
						var1->setBlockBounds(0.5F - var6, 0.0F, 0.0F, 0.5F + var6, 1.0F, var6 * 2.0F);
					}

					if(var7 == 1) {
						var1->setBlockBounds(0.5F - var6, 0.0F, 1.0F - var6 * 2.0F, 0.5F + var6, 1.0F, 1.0F);
					}

					var6 = 1.0F / 16.0F;
					if(var7 == 2) {
						var1->setBlockBounds(0.5F - var6, 1.0F - var6 * 3.0F, -var6 * 2.0F, 0.5F + var6, 1.0F - var6, 1.0F + var6 * 2.0F);
					}

					if(var7 == 3) {
						var1->setBlockBounds(0.5F - var6, 0.5F - var6 * 3.0F, -var6 * 2.0F, 0.5F + var6, 0.5F - var6, 1.0F + var6 * 2.0F);
					}

     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -0.5F, -0.5F, -0.5F);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
					var3.init();
					var3.normal(0.0F, -1.0F, 0.0F);
					renderBottomFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(0));
					var3.flush();
					var3.init();
					var3.normal(0.0F, 1.0F, 0.0F);
					renderTopFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(1));
					var3.flush();
					var3.init();
					var3.normal(0.0F, 0.0F, -1.0F);
					renderEastFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(2));
					var3.flush();
					var3.init();
					var3.normal(0.0F, 0.0F, 1.0F);
					renderWestFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(3));
					var3.flush();
					var3.init();
					var3.normal(-1.0F, 0.0F, 0.0F);
					renderNorthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(4));
					var3.flush();
					var3.init();
					var3.normal(1.0F, 0.0F, 0.0F);
					renderSouthFace(var1, 0.0f, 0.0f, 0.0f, var1->getBlockTextureFromSide(5));
					var3.flush();
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.5F, 0.5F, 0.5F);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}

				var1->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
			}
		}

}

bool RenderBlocks::func_1219_a(int var0) {
		return var0 == 0 ? true : (var0 == 13 ? true : (var0 == 10 ? true : var0 == 11));
}
