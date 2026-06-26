#include "stdafx.h"
// Auto-generated from RenderBlocks.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderBlocks.h"
#include "Common.h"
#include "Block.h"
#include "World.h"
#include "Material.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "BlockFire.h"

RenderBlocks::RenderBlocks(World* var1)
	: m_blockAccess(NULL)
	, m_overrideBlockTexture(-1)
	, m_flipTexture(false)
{
		m_blockAccess = var1;
}

RenderBlocks::RenderBlocks()
	: m_blockAccess(NULL)
	, m_overrideBlockTexture(-1)
	, m_flipTexture(false)
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

void RenderBlocks::renderBlockAllFaces(Block* var1, int var2, int var3, int var4) {
		m_flipTexture = true;
		renderBlockByRenderType(var1, var2, var3, var4);
		m_flipTexture = false;
}

bool RenderBlocks::renderBlockByRenderType(Block* var1, int var2, int var3, int var4) {
		int var5 = var1->getRenderType();
		Tesselator var6;
		float var10;
		bool var26;
		if(var5 == 0) {
			var6 = Tesselator::getInstance();
			var26 = false;
			if(m_flipTexture || var1->shouldSideBeRendered(m_blockAccess, var2, var3 - 1, var4, 0)) {
				var10 = var1->getBlockBrightness(m_blockAccess, var2, var3 - 1, var4);
				var6.color3f(0.5F * var10, 0.5F * var10, 0.5F * var10);
				renderBlockBottom(var1, (float)var2, (float)var3, (float)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 0));
				var26 = true;
			}

			if(m_flipTexture || var1->shouldSideBeRendered(m_blockAccess, var2, var3 + 1, var4, 1)) {
				var10 = var1->getBlockBrightness(m_blockAccess, var2, var3 + 1, var4);
				var6.color3f(var10 * 1.0F, var10 * 1.0F, var10 * 1.0F);
				renderBlockTop(var1, (float)var2, (float)var3, (float)var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 1));
				var26 = true;
			}

			if(m_flipTexture || var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 - 1, 2)) {
				var10 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 - 1);
				var6.color3f(0.8F * var10, 0.8F * var10, 0.8F * var10);
				renderBlockNorth(var1, var2, var3, var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 2));
				var26 = true;
			}

			if(m_flipTexture || var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 + 1, 3)) {
				var10 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 + 1);
				var6.color3f(0.8F * var10, 0.8F * var10, 0.8F * var10);
				renderBlockSouth(var1, var2, var3, var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 3));
				var26 = true;
			}

			if(m_flipTexture || var1->shouldSideBeRendered(m_blockAccess, var2 - 1, var3, var4, 4)) {
				var10 = var1->getBlockBrightness(m_blockAccess, var2 - 1, var3, var4);
				var6.color3f(0.6F * var10, 0.6F * var10, 0.6F * var10);
				renderBlockWest(var1, var2, var3, var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 4));
				var26 = true;
			}

			if(m_flipTexture || var1->shouldSideBeRendered(m_blockAccess, var2 + 1, var3, var4, 5)) {
				var10 = var1->getBlockBrightness(m_blockAccess, var2 + 1, var3, var4);
				var6.color3f(0.6F * var10, 0.6F * var10, 0.6F * var10);
				renderBlockEast(var1, var2, var3, var4, var1->getBlockTexture(m_blockAccess, var2, var3, var4, 5));
				var26 = true;
			}

			return var26;
		} else {
			float var11;
			float var22;
			if(var5 == 4) {
				var6 = Tesselator::getInstance();
				var26 = false;
				var10 = var1->minY;
				var11 = var1->maxY;
				var1->maxY = var11 - materialNotWater(var2, var3, var4);
				if(m_flipTexture || var1->shouldSideBeRendered(m_blockAccess, var2, var3 - 1, var4, 0)) {
					var22 = var1->getBlockBrightness(m_blockAccess, var2, var3 - 1, var4);
					var6.color3f(0.5F * var22, 0.5F * var22, 0.5F * var22);
					renderBlockBottom(var1, (float)var2, (float)var3, (float)var4, var1->getBlockTextureFromSide(0));
					var26 = true;
				}

				if(m_flipTexture || var1->shouldSideBeRendered(m_blockAccess, var2, var3 + 1, var4, 1)) {
					var22 = var1->getBlockBrightness(m_blockAccess, var2, var3 + 1, var4);
					var6.color3f(var22 * 1.0F, var22 * 1.0F, var22 * 1.0F);
					renderBlockTop(var1, (float)var2, (float)var3, (float)var4, var1->getBlockTextureFromSide(1));
					var26 = true;
				}

				var1->minY = var11 - materialNotWater(var2, var3, var4 - 1);
				if(m_flipTexture || var1->maxY > var1->minY || var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 - 1, 2)) {
					var22 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 - 1);
					var6.color3f(0.8F * var22, 0.8F * var22, 0.8F * var22);
					renderBlockNorth(var1, var2, var3, var4, var1->getBlockTextureFromSide(2));
					var26 = true;
				}

				var1->minY = var11 - materialNotWater(var2, var3, var4 + 1);
				if(m_flipTexture || var1->maxY > var1->minY || var1->shouldSideBeRendered(m_blockAccess, var2, var3, var4 + 1, 3)) {
					var22 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4 + 1);
					var6.color3f(0.8F * var22, 0.8F * var22, 0.8F * var22);
					renderBlockSouth(var1, var2, var3, var4, var1->getBlockTextureFromSide(3));
					var26 = true;
				}

				var1->minY = var11 - materialNotWater(var2 - 1, var3, var4);
				if(m_flipTexture || var1->maxY > var1->minY || var1->shouldSideBeRendered(m_blockAccess, var2 - 1, var3, var4, 4)) {
					var22 = var1->getBlockBrightness(m_blockAccess, var2 - 1, var3, var4);
					var6.color3f(0.6F * var22, 0.6F * var22, 0.6F * var22);
					renderBlockWest(var1, var2, var3, var4, var1->getBlockTextureFromSide(4));
					var26 = true;
				}

				var1->minY = var11 - materialNotWater(var2 + 1, var3, var4);
				if(m_flipTexture || var1->maxY > var1->minY || var1->shouldSideBeRendered(m_blockAccess, var2 + 1, var3, var4, 5)) {
					var22 = var1->getBlockBrightness(m_blockAccess, var2 + 1, var3, var4);
					var6.color3f(0.6F * var22, 0.6F * var22, 0.6F * var22);
					renderBlockEast(var1, var2, var3, var4, var1->getBlockTextureFromSide(5));
					var26 = true;
				}

				var1->minY = var10;
				var1->maxY = var11;
				return var26;
			} else if(var5 == 1) {
				var6 = Tesselator::getInstance();
				var22 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
				var6.color3f(var22, var22, var22);
				renderBlockPlant(var1, m_blockAccess->getBlockMetadata(var2, var3, var4), (float)var2, (float)var3, (float)var4);
				return true;
			} else if(var5 == 6) {
				var6 = Tesselator::getInstance();
				var22 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
				var6.color3f(var22, var22, var22);
				renderBlockCrops(var1, m_blockAccess->getBlockMetadata(var2, var3, var4), (float)var2, (float)var3 - 1.0F / 16.0F, (float)var4);
				return true;
			} else {
				float var8;
				if(var5 == 2) {
					unsigned char var21 = m_blockAccess->getBlockMetadata(var2, var3, var4);
					Tesselator var25 = Tesselator::getInstance();
					var8 = var1->getBlockBrightness(m_blockAccess, var2, var3, var4);
					if(Block::s_lightValue[var1->blockID] > 0) {
						var8 = 1.0F;
					}

					var25.color3f(var8, var8, var8);
					if(var21 == 1) {
						renderBlockTorch(var1, (float)var2 - 10.0F * 0.01F, (float)var3 + 0.2F, (float)var4, -0.4F, 0.0F);
					} else if(var21 == 2) {
						renderBlockTorch(var1, (float)var2 + 10.0F * 0.01F, (float)var3 + 0.2F, (float)var4, 0.4F, 0.0F);
					} else if(var21 == 3) {
						renderBlockTorch(var1, (float)var2, (float)var3 + 0.2F, (float)var4 - 10.0F * 0.01F, 0.0F, -0.4F);
					} else if(var21 == 4) {
						renderBlockTorch(var1, (float)var2, (float)var3 + 0.2F, (float)var4 + 10.0F * 0.01F, 0.0F, 0.4F);
					} else {
						renderBlockTorch(var1, (float)var2, (float)var3, (float)var4, 0.0F, 0.0F);
					}

					return true;
				} else {
					int var7;
					float var12;
					float var13;
					float var14;
					float var15;
					int var23;
					if(var5 == 3) {
						var5 = var4;
						var4 = var3;
						var3 = var2;
						var6 = Tesselator::getInstance();
						var7 = var1->getBlockTextureFromSide(0);
						if(m_overrideBlockTexture >= 0) {
							var7 = m_overrideBlockTexture;
						}

						var8 = var1->getBlockBrightness(m_blockAccess, var2, var4, var5);
						var6.color3f(var8, var8, var8);
						var2 = (var7 & 15) << 4;
						var23 = var7 & 240;
						float var24 = (float)var2 / 256.0F;
						var10 = ((float)var2 + 15.99F) / 256.0F;
						var11 = (float)var23 / 256.0F;
						var22 = ((float)var23 + 15.99F) / 256.0F;
						float var16;
						float var19;
						if(!m_blockAccess->isBlockNormalCube(var3, var4 - 1, var5) && !Block::s_fire->canBlockCatchFire(m_blockAccess, var3, var4 - 1, var5)) {
							if((var3 + var4 + var5 & 1) == 1) {
								var24 = (float)var2 / 256.0F;
								var10 = ((float)var2 + 15.99F) / 256.0F;
								var11 = (float)(var23 + 16) / 256.0F;
								var22 = ((float)var23 + 15.99F + 16.0F) / 256.0F;
							}

							if((var3 / 2 + var4 / 2 + var5 / 2 & 1) == 1) {
								var14 = var10;
								var10 = var24;
								var24 = var14;
							}

							if(Block::s_fire->canBlockCatchFire(m_blockAccess, var3 - 1, var4, var5)) {
								var6.vertexUV((float)var3 + 0.2F, (float)var4 + 1.4F + 1.0F / 16.0F, (float)(var5 + 1), var10, var11);
								var6.vertexUV((float)var3, (float)var4 + 1.0F / 16.0F, (float)(var5 + 1), var10, var22);
								var6.vertexUV((float)var3, (float)var4 + 1.0F / 16.0F, (float)var5, var24, var22);
								var6.vertexUV((float)var3 + 0.2F, (float)var4 + 1.4F + 1.0F / 16.0F, (float)var5, var24, var11);
								var6.vertexUV((float)var3 + 0.2F, (float)var4 + 1.4F + 1.0F / 16.0F, (float)var5, var24, var11);
								var6.vertexUV((float)var3, (float)var4 + 1.0F / 16.0F, (float)var5, var24, var22);
								var6.vertexUV((float)var3, (float)var4 + 1.0F / 16.0F, (float)(var5 + 1), var10, var22);
								var6.vertexUV((float)var3 + 0.2F, (float)var4 + 1.4F + 1.0F / 16.0F, (float)(var5 + 1), var10, var11);
							}

							if(Block::s_fire->canBlockCatchFire(m_blockAccess, var3 + 1, var4, var5)) {
								var6.vertexUV((float)(var3 + 1) - 0.2F, (float)var4 + 1.4F + 1.0F / 16.0F, (float)var5, var24, var11);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.0F / 16.0F, (float)var5, var24, var22);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.0F / 16.0F, (float)(var5 + 1), var10, var22);
								var6.vertexUV((float)(var3 + 1) - 0.2F, (float)var4 + 1.4F + 1.0F / 16.0F, (float)(var5 + 1), var10, var11);
								var6.vertexUV((float)(var3 + 1) - 0.2F, (float)var4 + 1.4F + 1.0F / 16.0F, (float)(var5 + 1), var10, var11);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.0F / 16.0F, (float)(var5 + 1), var10, var22);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.0F / 16.0F, (float)var5, var24, var22);
								var6.vertexUV((float)(var3 + 1) - 0.2F, (float)var4 + 1.4F + 1.0F / 16.0F, (float)var5, var24, var11);
							}

							if(Block::s_fire->canBlockCatchFire(m_blockAccess, var3, var4, var5 - 1)) {
								var6.vertexUV((float)var3, (float)var4 + 1.4F + 1.0F / 16.0F, (float)var5 + 0.2F, var10, var11);
								var6.vertexUV((float)var3, (float)var4 + 1.0F / 16.0F, (float)var5, var10, var22);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.0F / 16.0F, (float)var5, var24, var22);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.4F + 1.0F / 16.0F, (float)var5 + 0.2F, var24, var11);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.4F + 1.0F / 16.0F, (float)var5 + 0.2F, var24, var11);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.0F / 16.0F, (float)var5, var24, var22);
								var6.vertexUV((float)var3, (float)var4 + 1.0F / 16.0F, (float)var5, var10, var22);
								var6.vertexUV((float)var3, (float)var4 + 1.4F + 1.0F / 16.0F, (float)var5 + 0.2F, var10, var11);
							}

							if(Block::s_fire->canBlockCatchFire(m_blockAccess, var3, var4, var5 + 1)) {
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.4F + 1.0F / 16.0F, (float)(var5 + 1) - 0.2F, var24, var11);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.0F / 16.0F, (float)(var5 + 1), var24, var22);
								var6.vertexUV((float)var3, (float)var4 + 1.0F / 16.0F, (float)(var5 + 1), var10, var22);
								var6.vertexUV((float)var3, (float)var4 + 1.4F + 1.0F / 16.0F, (float)(var5 + 1) - 0.2F, var10, var11);
								var6.vertexUV((float)var3, (float)var4 + 1.4F + 1.0F / 16.0F, (float)(var5 + 1) - 0.2F, var10, var11);
								var6.vertexUV((float)var3, (float)var4 + 1.0F / 16.0F, (float)(var5 + 1), var10, var22);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.0F / 16.0F, (float)(var5 + 1), var24, var22);
								var6.vertexUV((float)(var3 + 1), (float)var4 + 1.4F + 1.0F / 16.0F, (float)(var5 + 1) - 0.2F, var24, var11);
							}

							if(Block::s_fire->canBlockCatchFire(m_blockAccess, var3, var4 + 1, var5)) {
								var14 = (float)var3 + 0.5F + 0.5F;
								var15 = (float)var3 + 0.5F - 0.5F;
								var19 = (float)var5 + 0.5F + 0.5F;
								var16 = (float)var5 + 0.5F - 0.5F;
								var24 = (float)var2 / 256.0F;
								var10 = ((float)var2 + 15.99F) / 256.0F;
								var11 = (float)var23 / 256.0F;
								var22 = ((float)var23 + 15.99F) / 256.0F;
								++var4;
								if((var3 + var4 + var5 & 1) == 0) {
									var6.vertexUV(var15, (float)var4 + -0.2F, (float)var5, var10, var11);
									var6.vertexUV(var14, (float)var4, (float)var5, var10, var22);
									var6.vertexUV(var14, (float)var4, (float)(var5 + 1), var24, var22);
									var6.vertexUV(var15, (float)var4 + -0.2F, (float)(var5 + 1), var24, var11);
									var24 = (float)var2 / 256.0F;
									var10 = ((float)var2 + 15.99F) / 256.0F;
									var11 = (float)(var23 + 16) / 256.0F;
									var22 = ((float)var23 + 15.99F + 16.0F) / 256.0F;
									var6.vertexUV(var14, (float)var4 + -0.2F, (float)(var5 + 1), var10, var11);
									var6.vertexUV(var15, (float)var4, (float)(var5 + 1), var10, var22);
									var6.vertexUV(var15, (float)var4, (float)var5, var24, var22);
									var6.vertexUV(var14, (float)var4 + -0.2F, (float)var5, var24, var11);
								} else {
									var6.vertexUV((float)var3, (float)var4 + -0.2F, var19, var10, var11);
									var6.vertexUV((float)var3, (float)var4, var16, var10, var22);
									var6.vertexUV((float)(var3 + 1), (float)var4, var16, var24, var22);
									var6.vertexUV((float)(var3 + 1), (float)var4 + -0.2F, var19, var24, var11);
									var24 = (float)var2 / 256.0F;
									var10 = ((float)var2 + 15.99F) / 256.0F;
									var11 = (float)(var23 + 16) / 256.0F;
									var22 = ((float)var23 + 15.99F + 16.0F) / 256.0F;
									var6.vertexUV((float)(var3 + 1), (float)var4 + -0.2F, var16, var10, var11);
									var6.vertexUV((float)(var3 + 1), (float)var4, var19, var10, var22);
									var6.vertexUV((float)var3, (float)var4, var19, var24, var22);
									var6.vertexUV((float)var3, (float)var4 + -0.2F, var16, var24, var11);
								}
							}
						} else {
							var12 = (float)var3 + 0.5F + 0.2F;
							var13 = (float)var3 + 0.5F - 0.2F;
							var14 = (float)var5 + 0.5F + 0.2F;
							var15 = (float)var5 + 0.5F - 0.2F;
							var19 = (float)var3 + 0.5F - 0.3F;
							var16 = (float)var3 + 0.5F + 0.3F;
							float var17 = (float)var5 + 0.5F - 0.3F;
							float var18 = (float)var5 + 0.5F + 0.3F;
							var6.vertexUV(var19, (float)var4 + 1.4F, (float)(var5 + 1), var10, var11);
							var6.vertexUV(var12, (float)var4, (float)(var5 + 1), var10, var22);
							var6.vertexUV(var12, (float)var4, (float)var5, var24, var22);
							var6.vertexUV(var19, (float)var4 + 1.4F, (float)var5, var24, var11);
							var6.vertexUV(var16, (float)var4 + 1.4F, (float)var5, var10, var11);
							var6.vertexUV(var13, (float)var4, (float)var5, var10, var22);
							var6.vertexUV(var13, (float)var4, (float)(var5 + 1), var24, var22);
							var6.vertexUV(var16, (float)var4 + 1.4F, (float)(var5 + 1), var24, var11);
							var24 = (float)var2 / 256.0F;
							var10 = ((float)var2 + 15.99F) / 256.0F;
							var11 = (float)(var23 + 16) / 256.0F;
							var22 = ((float)var23 + 15.99F + 16.0F) / 256.0F;
							var6.vertexUV((float)(var3 + 1), (float)var4 + 1.4F, var18, var10, var11);
							var6.vertexUV((float)(var3 + 1), (float)var4, var15, var10, var22);
							var6.vertexUV((float)var3, (float)var4, var15, var24, var22);
							var6.vertexUV((float)var3, (float)var4 + 1.4F, var18, var24, var11);
							var6.vertexUV((float)var3, (float)var4 + 1.4F, var17, var10, var11);
							var6.vertexUV((float)var3, (float)var4, var14, var10, var22);
							var6.vertexUV((float)(var3 + 1), (float)var4, var14, var24, var22);
							var6.vertexUV((float)(var3 + 1), (float)var4 + 1.4F, var17, var24, var11);
							var12 = (float)var3 + 0.5F - 0.5F;
							var13 = (float)var3 + 0.5F + 0.5F;
							var14 = (float)var5 + 0.5F - 0.5F;
							var15 = (float)var5 + 0.5F + 0.5F;
							var19 = (float)var3 + 0.5F - 0.4F;
							var16 = (float)var3 + 0.5F + 0.4F;
							var17 = (float)var5 + 0.5F - 0.4F;
							var18 = (float)var5 + 0.5F + 0.4F;
							var6.vertexUV(var19, (float)var4 + 1.4F, (float)var5, var24, var11);
							var6.vertexUV(var12, (float)var4, (float)var5, var24, var22);
							var6.vertexUV(var12, (float)var4, (float)(var5 + 1), var10, var22);
							var6.vertexUV(var19, (float)var4 + 1.4F, (float)(var5 + 1), var10, var11);
							var6.vertexUV(var16, (float)var4 + 1.4F, (float)(var5 + 1), var24, var11);
							var6.vertexUV(var13, (float)var4, (float)(var5 + 1), var24, var22);
							var6.vertexUV(var13, (float)var4, (float)var5, var10, var22);
							var6.vertexUV(var16, (float)var4 + 1.4F, (float)var5, var10, var11);
							var24 = (float)var2 / 256.0F;
							var10 = ((float)var2 + 15.99F) / 256.0F;
							var11 = (float)var23 / 256.0F;
							var22 = ((float)var23 + 15.99F) / 256.0F;
							var6.vertexUV((float)var3, (float)var4 + 1.4F, var18, var24, var11);
							var6.vertexUV((float)var3, (float)var4, var15, var24, var22);
							var6.vertexUV((float)(var3 + 1), (float)var4, var15, var10, var22);
							var6.vertexUV((float)(var3 + 1), (float)var4 + 1.4F, var18, var10, var11);
							var6.vertexUV((float)(var3 + 1), (float)var4 + 1.4F, var17, var24, var11);
							var6.vertexUV((float)(var3 + 1), (float)var4, var14, var24, var22);
							var6.vertexUV((float)var3, (float)var4, var14, var10, var22);
							var6.vertexUV((float)var3, (float)var4 + 1.4F, var17, var10, var11);
						}

						return true;
					} else if(var5 == 5) {
						var5 = var4;
						var4 = var3;
						var3 = var2;
						var6 = Tesselator::getInstance();
						var7 = var1->getBlockTextureFromSide(0);
						if(m_overrideBlockTexture >= 0) {
							var7 = m_overrideBlockTexture;
						}

						var8 = var1->getBlockBrightness(m_blockAccess, var2, var4, var5);
						var6.color3f(var8, var8, var8);
						var2 = ((var7 & 15) << 4) + 16;
						var23 = (var7 & 15) << 4;
						int var9 = var7 & 240;
						if((var3 + var4 + var5 & 1) == 1) {
							var2 = (var7 & 15) << 4;
							var23 = ((var7 & 15) << 4) + 16;
						}

						var10 = (float)var2 / 256.0F;
						var11 = ((float)var2 + 15.99F) / 256.0F;
						var22 = (float)var9 / 256.0F;
						float var20 = ((float)var9 + 15.99F) / 256.0F;
						var12 = (float)var23 / 256.0F;
						var13 = ((float)var23 + 15.99F) / 256.0F;
						var14 = (float)var9 / 256.0F;
						var15 = ((float)var9 + 15.99F) / 256.0F;
						if(m_blockAccess->isBlockNormalCube(var3 - 1, var4, var5)) {
							var6.vertexUV((float)var3 + 0.05F, (float)(var4 + 1) + 2.0F / 16.0F, (float)(var5 + 1) + 2.0F / 16.0F, var10, var22);
							var6.vertexUV((float)var3 + 0.05F, (float)var4 - 2.0F / 16.0F, (float)(var5 + 1) + 2.0F / 16.0F, var10, var20);
							var6.vertexUV((float)var3 + 0.05F, (float)var4 - 2.0F / 16.0F, (float)var5 - 2.0F / 16.0F, var11, var20);
							var6.vertexUV((float)var3 + 0.05F, (float)(var4 + 1) + 2.0F / 16.0F, (float)var5 - 2.0F / 16.0F, var11, var22);
						}

						if(m_blockAccess->isBlockNormalCube(var3 + 1, var4, var5)) {
							var6.vertexUV((float)(var3 + 1) - 0.05F, (float)var4 - 2.0F / 16.0F, (float)(var5 + 1) + 2.0F / 16.0F, var11, var20);
							var6.vertexUV((float)(var3 + 1) - 0.05F, (float)(var4 + 1) + 2.0F / 16.0F, (float)(var5 + 1) + 2.0F / 16.0F, var11, var22);
							var6.vertexUV((float)(var3 + 1) - 0.05F, (float)(var4 + 1) + 2.0F / 16.0F, (float)var5 - 2.0F / 16.0F, var10, var22);
							var6.vertexUV((float)(var3 + 1) - 0.05F, (float)var4 - 2.0F / 16.0F, (float)var5 - 2.0F / 16.0F, var10, var20);
						}

						if(m_blockAccess->isBlockNormalCube(var3, var4, var5 - 1)) {
							var6.vertexUV((float)(var3 + 1) + 2.0F / 16.0F, (float)var4 - 2.0F / 16.0F, (float)var5 + 0.05F, var13, var15);
							var6.vertexUV((float)(var3 + 1) + 2.0F / 16.0F, (float)(var4 + 1) + 2.0F / 16.0F, (float)var5 + 0.05F, var13, var14);
							var6.vertexUV((float)var3 - 2.0F / 16.0F, (float)(var4 + 1) + 2.0F / 16.0F, (float)var5 + 0.05F, var12, var14);
							var6.vertexUV((float)var3 - 2.0F / 16.0F, (float)var4 - 2.0F / 16.0F, (float)var5 + 0.05F, var12, var15);
						}

						if(m_blockAccess->isBlockNormalCube(var3, var4, var5 + 1)) {
							var6.vertexUV((float)(var3 + 1) + 2.0F / 16.0F, (float)(var4 + 1) + 2.0F / 16.0F, (float)(var5 + 1) - 0.05F, var12, var14);
							var6.vertexUV((float)(var3 + 1) + 2.0F / 16.0F, (float)var4 - 2.0F / 16.0F, (float)(var5 + 1) - 0.05F, var12, var15);
							var6.vertexUV((float)var3 - 2.0F / 16.0F, (float)var4 - 2.0F / 16.0F, (float)(var5 + 1) - 0.05F, var13, var15);
							var6.vertexUV((float)var3 - 2.0F / 16.0F, (float)(var4 + 1) + 2.0F / 16.0F, (float)(var5 + 1) - 0.05F, var13, var14);
						}

						return true;
					} else {
						return false;
					}
				}
			}
		}
}

void RenderBlocks::renderBlockTorch(Block* var1, float var2, float var3, float var4, float var5, float var6) {
		Tesselator var7 = Tesselator::getInstance();
		int var19 = var1->getBlockTextureFromSide(0);
		if(m_overrideBlockTexture >= 0) {
			var19 = m_overrideBlockTexture;
		}

		int var8 = (var19 & 15) << 4;
		var19 &= 240;
		float var9 = (float)var8 / 256.0F;
		float var21 = ((float)var8 + 15.99F) / 256.0F;
		float var10 = (float)var19 / 256.0F;
		float var20 = ((float)var19 + 15.99F) / 256.0F;
		float var11 = var9 + 0.02734375F;
		float var12 = var10 + 0.0234375F;
		float var13 = var9 + 0.03515625F;
		float var14 = var10 + 0.03125F;
		var2 += 0.5F;
		var4 += 0.5F;
		float var15 = var2 - 0.5F;
		float var16 = var2 + 0.5F;
		float var17 = var4 - 0.5F;
		float var18 = var4 + 0.5F;
		var7.vertexUV(var2 + var5 * (6.0F / 16.0F) - 1.0F / 16.0F, var3 + 10.0F / 16.0F, var4 + var6 * (6.0F / 16.0F) - 1.0F / 16.0F, var11, var12);
		var7.vertexUV(var2 + var5 * (6.0F / 16.0F) - 1.0F / 16.0F, var3 + 10.0F / 16.0F, var4 + var6 * (6.0F / 16.0F) + 1.0F / 16.0F, var11, var14);
		var7.vertexUV(var2 + var5 * (6.0F / 16.0F) + 1.0F / 16.0F, var3 + 10.0F / 16.0F, var4 + var6 * (6.0F / 16.0F) + 1.0F / 16.0F, var13, var14);
		var7.vertexUV(var2 + var5 * (6.0F / 16.0F) + 1.0F / 16.0F, var3 + 10.0F / 16.0F, var4 + var6 * (6.0F / 16.0F) - 1.0F / 16.0F, var13, var12);
		var7.vertexUV(var2 - 1.0F / 16.0F, var3 + 1.0F, var17, var9, var10);
		var7.vertexUV(var2 - 1.0F / 16.0F + var5, var3, var17 + var6, var9, var20);
		var7.vertexUV(var2 - 1.0F / 16.0F + var5, var3, var18 + var6, var21, var20);
		var7.vertexUV(var2 - 1.0F / 16.0F, var3 + 1.0F, var18, var21, var10);
		var7.vertexUV(var2 + 1.0F / 16.0F, var3 + 1.0F, var18, var9, var10);
		var7.vertexUV(var2 + var5 + 1.0F / 16.0F, var3, var18 + var6, var9, var20);
		var7.vertexUV(var2 + var5 + 1.0F / 16.0F, var3, var17 + var6, var21, var20);
		var7.vertexUV(var2 + 1.0F / 16.0F, var3 + 1.0F, var17, var21, var10);
		var7.vertexUV(var15, var3 + 1.0F, var4 + 1.0F / 16.0F, var9, var10);
		var7.vertexUV(var15 + var5, var3, var4 + 1.0F / 16.0F + var6, var9, var20);
		var7.vertexUV(var16 + var5, var3, var4 + 1.0F / 16.0F + var6, var21, var20);
		var7.vertexUV(var16, var3 + 1.0F, var4 + 1.0F / 16.0F, var21, var10);
		var7.vertexUV(var16, var3 + 1.0F, var4 - 1.0F / 16.0F, var9, var10);
		var7.vertexUV(var16 + var5, var3, var4 - 1.0F / 16.0F + var6, var9, var20);
		var7.vertexUV(var15 + var5, var3, var4 - 1.0F / 16.0F + var6, var21, var20);
		var7.vertexUV(var15, var3 + 1.0F, var4 - 1.0F / 16.0F, var21, var10);
}

void RenderBlocks::renderBlockPlant(Block* var1, int var2, float var3, float var4, float var5) {
		Tesselator var6 = Tesselator::getInstance();
		int var11 = var1->getBlockTextureFromSideAndMetadata(0, var2);
		if(m_overrideBlockTexture >= 0) {
			var11 = m_overrideBlockTexture;
		}

		var2 = (var11 & 15) << 4;
		var11 &= 240;
		float var7 = (float)var2 / 256.0F;
		float var12 = ((float)var2 + 15.99F) / 256.0F;
		float var8 = (float)var11 / 256.0F;
		float var13 = ((float)var11 + 15.99F) / 256.0F;
		float var9 = var3 + 0.5F - 0.45F;
		var3 = var3 + 0.5F + 0.45F;
		float var10 = var5 + 0.5F - 0.45F;
		var5 = var5 + 0.5F + 0.45F;
		var6.vertexUV(var9, var4 + 1.0F, var10, var7, var8);
		var6.vertexUV(var9, var4, var10, var7, var13);
		var6.vertexUV(var3, var4, var5, var12, var13);
		var6.vertexUV(var3, var4 + 1.0F, var5, var12, var8);
		var6.vertexUV(var3, var4 + 1.0F, var5, var7, var8);
		var6.vertexUV(var3, var4, var5, var7, var13);
		var6.vertexUV(var9, var4, var10, var12, var13);
		var6.vertexUV(var9, var4 + 1.0F, var10, var12, var8);
		var6.vertexUV(var9, var4 + 1.0F, var5, var7, var8);
		var6.vertexUV(var9, var4, var5, var7, var13);
		var6.vertexUV(var3, var4, var10, var12, var13);
		var6.vertexUV(var3, var4 + 1.0F, var10, var12, var8);
		var6.vertexUV(var3, var4 + 1.0F, var10, var7, var8);
		var6.vertexUV(var3, var4, var10, var7, var13);
		var6.vertexUV(var9, var4, var5, var12, var13);
		var6.vertexUV(var9, var4 + 1.0F, var5, var12, var8);
}

void RenderBlocks::renderBlockCrops(Block* var1, int var2, float var3, float var4, float var5) {
		Tesselator var6 = Tesselator::getInstance();
		int var13 = var1->getBlockTextureFromSideAndMetadata(0, var2);
		if(m_overrideBlockTexture >= 0) {
			var13 = m_overrideBlockTexture;
		}

		var2 = (var13 & 15) << 4;
		var13 &= 240;
		float var7 = (float)var2 / 256.0F;
		float var14 = ((float)var2 + 15.99F) / 256.0F;
		float var8 = (float)var13 / 256.0F;
		float var15 = ((float)var13 + 15.99F) / 256.0F;
		float var9 = var3 + 0.5F - 0.25F;
		float var10 = var3 + 0.5F + 0.25F;
		float var11 = var5 + 0.5F - 0.5F;
		float var12 = var5 + 0.5F + 0.5F;
		var6.vertexUV(var9, var4 + 1.0F, var11, var7, var8);
		var6.vertexUV(var9, var4, var11, var7, var15);
		var6.vertexUV(var9, var4, var12, var14, var15);
		var6.vertexUV(var9, var4 + 1.0F, var12, var14, var8);
		var6.vertexUV(var9, var4 + 1.0F, var12, var7, var8);
		var6.vertexUV(var9, var4, var12, var7, var15);
		var6.vertexUV(var9, var4, var11, var14, var15);
		var6.vertexUV(var9, var4 + 1.0F, var11, var14, var8);
		var6.vertexUV(var10, var4 + 1.0F, var12, var7, var8);
		var6.vertexUV(var10, var4, var12, var7, var15);
		var6.vertexUV(var10, var4, var11, var14, var15);
		var6.vertexUV(var10, var4 + 1.0F, var11, var14, var8);
		var6.vertexUV(var10, var4 + 1.0F, var11, var7, var8);
		var6.vertexUV(var10, var4, var11, var7, var15);
		var6.vertexUV(var10, var4, var12, var14, var15);
		var6.vertexUV(var10, var4 + 1.0F, var12, var14, var8);
		var9 = var3 + 0.5F - 0.5F;
		var10 = var3 + 0.5F + 0.5F;
		var11 = var5 + 0.5F - 0.25F;
		var12 = var5 + 0.5F + 0.25F;
		var6.vertexUV(var9, var4 + 1.0F, var11, var7, var8);
		var6.vertexUV(var9, var4, var11, var7, var15);
		var6.vertexUV(var10, var4, var11, var14, var15);
		var6.vertexUV(var10, var4 + 1.0F, var11, var14, var8);
		var6.vertexUV(var10, var4 + 1.0F, var11, var7, var8);
		var6.vertexUV(var10, var4, var11, var7, var15);
		var6.vertexUV(var9, var4, var11, var14, var15);
		var6.vertexUV(var9, var4 + 1.0F, var11, var14, var8);
		var6.vertexUV(var10, var4 + 1.0F, var12, var7, var8);
		var6.vertexUV(var10, var4, var12, var7, var15);
		var6.vertexUV(var9, var4, var12, var14, var15);
		var6.vertexUV(var9, var4 + 1.0F, var12, var14, var8);
		var6.vertexUV(var9, var4 + 1.0F, var12, var7, var8);
		var6.vertexUV(var9, var4, var12, var7, var15);
		var6.vertexUV(var10, var4, var12, var14, var15);
		var6.vertexUV(var10, var4 + 1.0F, var12, var14, var8);
}

float RenderBlocks::materialNotWater(int var1, int var2, int var3) {
		return m_blockAccess->getBlockMaterial(var1, var2, var3) != Material::s_water ? 1.0F : (float)m_blockAccess->getBlockMetadata(var1, var2, var3) / 9.0F;
}

void RenderBlocks::renderBlockBottom(Block* var1, float var2, float var3, float var4, int var5) {
		Tesselator var6 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var5 = m_overrideBlockTexture;
		}

		int var7 = (var5 & 15) << 4;
		var5 &= 240;
		float var8 = (float)var7 / 256.0F;
		float var14 = ((float)var7 + 15.99F) / 256.0F;
		float var9 = (float)var5 / 256.0F;
		float var13 = ((float)var5 + 15.99F) / 256.0F;
		float var10 = var2 + var1->minX;
		var2 += var1->maxX;
		var3 += var1->minY;
		float var11 = var4 + var1->minZ;
		float var12 = var4 + var1->maxZ;
		var6.vertexUV(var10, var3, var12, var8, var13);
		var6.vertexUV(var10, var3, var11, var8, var9);
		var6.vertexUV(var2, var3, var11, var14, var9);
		var6.vertexUV(var2, var3, var12, var14, var13);
}

void RenderBlocks::renderBlockTop(Block* var1, float var2, float var3, float var4, int var5) {
		Tesselator var6 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var5 = m_overrideBlockTexture;
		}

		int var7 = (var5 & 15) << 4;
		var5 &= 240;
		float var8 = (float)var7 / 256.0F;
		float var14 = ((float)var7 + 15.99F) / 256.0F;
		float var9 = (float)var5 / 256.0F;
		float var13 = ((float)var5 + 15.99F) / 256.0F;
		float var10 = var2 + var1->minX;
		var2 += var1->maxX;
		var3 += var1->maxY;
		float var11 = var4 + var1->minZ;
		float var12 = var4 + var1->maxZ;
		var6.vertexUV(var2, var3, var12, var14, var13);
		var6.vertexUV(var2, var3, var11, var14, var9);
		var6.vertexUV(var10, var3, var11, var8, var9);
		var6.vertexUV(var10, var3, var12, var8, var13);
}

void RenderBlocks::renderBlockNorth(Block* var1, int var2, int var3, int var4, int var5) {
		Tesselator var6 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var5 = m_overrideBlockTexture;
		}

		int var7 = (var5 & 15) << 4;
		var5 &= 240;
		float var8 = (float)var7 / 256.0F;
		float var16 = ((float)var7 + 15.99F) / 256.0F;
		float var9;
		float var15;
		if(var1->minY >= 0.0F && var1->maxY <= 1.0F) {
			var9 = ((float)var5 + var1->minY * 15.99F) / 256.0F;
			var15 = ((float)var5 + var1->maxY * 15.99F) / 256.0F;
		} else {
			var9 = (float)var5 / 256.0F;
			var15 = ((float)var5 + 15.99F) / 256.0F;
		}

		float var10 = (float)var2 + var1->minX;
		float var13 = (float)var2 + var1->maxX;
		float var11 = (float)var3 + var1->minY;
		float var14 = (float)var3 + var1->maxY;
		float var12 = (float)var4 + var1->minZ;
		var6.vertexUV(var10, var14, var12, var16, var9);
		var6.vertexUV(var13, var14, var12, var8, var9);
		var6.vertexUV(var13, var11, var12, var8, var15);
		var6.vertexUV(var10, var11, var12, var16, var15);
}

void RenderBlocks::renderBlockSouth(Block* var1, int var2, int var3, int var4, int var5) {
		Tesselator var6 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var5 = m_overrideBlockTexture;
		}

		int var7 = (var5 & 15) << 4;
		var5 &= 240;
		float var8 = (float)var7 / 256.0F;
		float var16 = ((float)var7 + 15.99F) / 256.0F;
		float var9;
		float var15;
		if(var1->minY >= 0.0F && var1->maxY <= 1.0F) {
			var9 = ((float)var5 + var1->minY * 15.99F) / 256.0F;
			var15 = ((float)var5 + var1->maxY * 15.99F) / 256.0F;
		} else {
			var9 = (float)var5 / 256.0F;
			var15 = ((float)var5 + 15.99F) / 256.0F;
		}

		float var10 = (float)var2 + var1->minX;
		float var13 = (float)var2 + var1->maxX;
		float var11 = (float)var3 + var1->minY;
		float var14 = (float)var3 + var1->maxY;
		float var12 = (float)var4 + var1->maxZ;
		var6.vertexUV(var10, var14, var12, var8, var9);
		var6.vertexUV(var10, var11, var12, var8, var15);
		var6.vertexUV(var13, var11, var12, var16, var15);
		var6.vertexUV(var13, var14, var12, var16, var9);
}

void RenderBlocks::renderBlockWest(Block* var1, int var2, int var3, int var4, int var5) {
		Tesselator var6 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var5 = m_overrideBlockTexture;
		}

		int var7 = (var5 & 15) << 4;
		var5 &= 240;
		float var8 = (float)var7 / 256.0F;
		float var16 = ((float)var7 + 15.99F) / 256.0F;
		float var9;
		float var15;
		if(var1->minY >= 0.0F && var1->maxY <= 1.0F) {
			var9 = ((float)var5 + var1->minY * 15.99F) / 256.0F;
			var15 = ((float)var5 + var1->maxY * 15.99F) / 256.0F;
		} else {
			var9 = (float)var5 / 256.0F;
			var15 = ((float)var5 + 15.99F) / 256.0F;
		}

		float var13 = (float)var2 + var1->minX;
		float var10 = (float)var3 + var1->minY;
		float var14 = (float)var3 + var1->maxY;
		float var11 = (float)var4 + var1->minZ;
		float var12 = (float)var4 + var1->maxZ;
		var6.vertexUV(var13, var14, var12, var16, var9);
		var6.vertexUV(var13, var14, var11, var8, var9);
		var6.vertexUV(var13, var10, var11, var8, var15);
		var6.vertexUV(var13, var10, var12, var16, var15);
}

void RenderBlocks::renderBlockEast(Block* var1, int var2, int var3, int var4, int var5) {
		Tesselator var6 = Tesselator::getInstance();
		if(m_overrideBlockTexture >= 0) {
			var5 = m_overrideBlockTexture;
		}

		int var7 = (var5 & 15) << 4;
		var5 &= 240;
		float var8 = (float)var7 / 256.0F;
		float var16 = ((float)var7 + 15.99F) / 256.0F;
		float var9;
		float var15;
		if(var1->minY >= 0.0F && var1->maxY <= 1.0F) {
			var9 = ((float)var5 + var1->minY * 15.99F) / 256.0F;
			var15 = ((float)var5 + var1->maxY * 15.99F) / 256.0F;
		} else {
			var9 = (float)var5 / 256.0F;
			var15 = ((float)var5 + 15.99F) / 256.0F;
		}

		float var13 = (float)var2 + var1->maxX;
		float var10 = (float)var3 + var1->minY;
		float var14 = (float)var3 + var1->maxY;
		float var11 = (float)var4 + var1->minZ;
		float var12 = (float)var4 + var1->maxZ;
		var6.vertexUV(var13, var10, var12, var8, var15);
		var6.vertexUV(var13, var10, var11, var16, var15);
		var6.vertexUV(var13, var14, var11, var16, var9);
		var6.vertexUV(var13, var14, var12, var8, var9);
}

void RenderBlocks::renderBlockOnInventory(Block* var1) {
		Tesselator var2 = Tesselator::getInstance();
		int var3 = var1->getRenderType();
		if(var3 == 0) {
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -0.5F, -0.5F, -0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var2.init();
			Tesselator::normal(0.0F, -1.0F, 0.0F);
			renderBlockBottom(var1, 0.0F, 0.0F, 0.0F, var1->getBlockTextureFromSide(0));
			var2.flush();
			var2.init();
			Tesselator::normal(0.0F, 1.0F, 0.0F);
			renderBlockTop(var1, 0.0F, 0.0F, 0.0F, var1->getBlockTextureFromSide(1));
			var2.flush();
			var2.init();
			Tesselator::normal(0.0F, 0.0F, -1.0F);
			renderBlockNorth(var1, 0, 0, 0, var1->getBlockTextureFromSide(2));
			var2.flush();
			var2.init();
			Tesselator::normal(0.0F, 0.0F, 1.0F);
			renderBlockSouth(var1, 0, 0, 0, var1->getBlockTextureFromSide(3));
			var2.flush();
			var2.init();
			Tesselator::normal(-1.0F, 0.0F, 0.0F);
			renderBlockWest(var1, 0, 0, 0, var1->getBlockTextureFromSide(4));
			var2.flush();
			var2.init();
			Tesselator::normal(1.0F, 0.0F, 0.0F);
			renderBlockEast(var1, 0, 0, 0, var1->getBlockTextureFromSide(5));
			var2.flush();
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.5F, 0.5F, 0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		} else if(var3 == 1) {
			var2.init();
			Tesselator::normal(0.0F, -1.0F, 0.0F);
			renderBlockPlant(var1, -1, -0.5F, -0.5F, -0.5F);
			var2.flush();
		} else if(var3 == 6) {
			var2.init();
			Tesselator::normal(0.0F, -1.0F, 0.0F);
			renderBlockCrops(var1, -1, -0.5F, -0.5F, -0.5F);
			var2.flush();
		} else {
			if(var3 == 2) {
				var2.init();
				Tesselator::normal(0.0F, -1.0F, 0.0F);
				renderBlockTorch(var1, -0.5F, -0.5F, -0.5F, 0.0F, 0.0F);
				var2.flush();
			}

		}
}
