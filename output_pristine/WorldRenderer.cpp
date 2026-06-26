#include "stdafx.h"
// Auto-generated from WorldRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldRenderer.h"
#include "Common.h"
#include "EntityLiving.h"
#include "ICamera.h"
#include "RenderBlocks.h"
#include "Tesselator.h"
#include "World.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "Item.h"
#include "RenderItem.h"
#include "Tesselator.h"

Tesselator* WorldRenderer::s_tessellator = Tesselator::getInstance();
int WorldRenderer::chunksUpdated = 0;

WorldRenderer::WorldRenderer()
	: worldObj(NULL)
	, m_glRenderList(-1)
	, posX(0)
	, posY(0)
	, posZ(0)
	, m_sizeWidth(0)
	, m_sizeHeight(0)
	, m_sizeDepth(0)
	, isInFrustrum(false)
	, m_skipRenderPass(new bool[2])
	, m_posXPlus(0)
	, m_posYPlus(0)
	, m_posZPlus(0)
	, needsUpdate(false)
	, m_renderBlocks(NULL)
	, isVisible(true)
	, isWaitingOnOcclusionQuery(false)
	, glOcclusionQuery(0)
{
}

WorldRenderer::WorldRenderer(World* var1, int var2, int var3, int var4, int var5, int var6)
	: worldObj(NULL)
	, m_glRenderList(-1)
	, posX(0)
	, posY(0)
	, posZ(0)
	, m_sizeWidth(0)
	, m_sizeHeight(0)
	, m_sizeDepth(0)
	, isInFrustrum(false)
	, m_skipRenderPass(new bool[2])
	, m_posXPlus(0)
	, m_posYPlus(0)
	, m_posZPlus(0)
	, needsUpdate(false)
	, m_renderBlocks(NULL)
	, isVisible(true)
	, isWaitingOnOcclusionQuery(false)
	, glOcclusionQuery(0)
{
		m_renderBlocks = new RenderBlocks(var1);
		worldObj = var1;
		posX = var2;
		posY = var3;
		posZ = var4;
		m_sizeWidth = m_sizeHeight = m_sizeDepth = 16;
		m_posXPlus = var2 + m_sizeWidth / 2;
		m_posYPlus = var3 + m_sizeHeight / 2;
		m_posZPlus = var4 + m_sizeDepth / 2;
		sqrtf((float)(m_sizeWidth * m_sizeWidth + m_sizeHeight * m_sizeHeight + m_sizeDepth * m_sizeDepth));
		m_rendererBoundingBox = (AxisAlignedBB((float)var2, (float)var3, (float)var4, (float)(var2 + m_sizeWidth), (float)(var3 + m_sizeHeight), (float)(var4 + m_sizeDepth))).expand(2.0F, 2.0F, 2.0F);
		m_glRenderList = var6;
		setDontDraw();
  g_pd3dDevice->SetTexture(0, NULL);
  // BEGIN display list -> vertex buffer recording
		RenderItem::renderOffsetAABB(m_rendererBoundingBox);
  // END display list -> vertex buffer recording
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
}

WorldRenderer::~WorldRenderer() {
	worldObj = NULL;
	delete[] m_skipRenderPass;
	m_skipRenderPass = NULL;
	m_renderBlocks = NULL;
}

void WorldRenderer::updateRenderer() {
		if(needsUpdate) {
			++chunksUpdated;
			int var1 = posX;
			int var2 = posY;
			int var3 = posZ;
			int var4 = posX + m_sizeWidth;
			int var5 = posY + m_sizeHeight;
			int var6 = posZ + m_sizeDepth;

			int var7;
			for(var7 = 0; var7 < 2; ++var7) {
				m_skipRenderPass[var7] = true;
			}

			for(var7 = 0; var7 < 2; ++var7) {
				bool var8 = false;
				bool var9 = false;
				s_tessellator->init();
    // BEGIN display list -> vertex buffer recording

				for(int var10 = var2; var10 < var5; ++var10) {
					for(int var11 = var3; var11 < var6; ++var11) {
						int var12 = (var10 * worldObj /* PORT: array length - use tracked size */ + var11) * worldObj->width + var1;

						for(int var13 = var1; var13 < var4; ++var13) {
							int var14 = worldObj->m_d[var12++] & 255;
							if(var14 > 0) {
								Block var15 = Block::blocksList[var14];
								if(var15.getRenderBlockPass() != var7) {
									var8 = true;
								} else {
									var9 |= m_renderBlocks->renderBlockByRenderType(var15, var13, var10, var11);
								}
							}
						}
					}
				}

				s_tessellator->flush();
    // END display list -> vertex buffer recording
				if(var9) {
					m_skipRenderPass[var7] = false;
				}

				if(!var8) {
					break;
				}
			}

		}
}

float WorldRenderer::distanceToEntitySquared(EntityLiving* var1) {
		float var2 = var1->posX - (float)m_posXPlus;
		float var3 = var1->posY - (float)m_posYPlus;
		float var4 = var1->posZ - (float)m_posZPlus;
		return var2 * var2 + var3 * var3 + var4 * var4;
}

void WorldRenderer::setDontDraw() {
		for(int var1 = 0; var1 < 2; ++var1) {
			m_skipRenderPass[var1] = true;
		}

}

void WorldRenderer::stopRendering() {
		setDontDraw();
		worldObj = NULL;
}

int WorldRenderer::getGLCallListForPass(int* var1, int var2, int var3) {
		if(!isInFrustrum) {
			return var2;
		} else {
			if(!m_skipRenderPass[var3]) {
				var1[var2++] = m_glRenderList + var3;
			}

			return var2;
		}
}

void WorldRenderer::updateInFrustrum(ICamera* var1) {
		isInFrustrum = var1->isBoundingBoxInFrustrum(m_rendererBoundingBox);
}

void WorldRenderer::callOcclusionQueryList() {
  // Draw cached vertex buffer for list this.glRenderList + 2
}
