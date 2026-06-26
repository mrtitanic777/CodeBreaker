#include "stdafx.h"
// Auto-generated from RenderGlobal.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderGlobal.h"
#include "Common.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "ICamera.h"
#include "ItemStack.h"
#include "Minecraft.h"
#include "MovingObjectPosition.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldRenderer.h"
#include <algorithm>
#include <string>
#include "AxisAlignedBB.h"
#include "Block.h"
#include "EntityMap.h"
#include "MathHelper.h"
#include "Random.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

RenderGlobal::RenderGlobal()
	: worldObj(NULL)
	, m_renderEngine(NULL)
	, m_glGenList(0)
	, m_renderIntBuffer(new int[65536])
	, m_sortedWorldRenderers(NULL)
	, m_worldRenderers(NULL)
	, m_renderChunksWide(0)
	, m_renderChunksTall(0)
	, m_renderChunksDeep(0)
	, m_glRenderListBase(0)
	, mc(NULL)
	, m_globalRenderBlocks(NULL)
	, m_occlusionEnabled(false)
	, m_cloudOffsetX(0)
	, m_glSkyList(0)
	, m_countEntitiesTotal(0)
	, m_countEntitiesRendered(0)
	, m_countEntitiesHidden(0)
	, m_dummyBuf50k(new int['\uc350'])
	, m_occlusionResult(new int[64])
	, m_renderersLoaded(0)
	, m_renderersBeingClipped(0)
	, m_renderersBeingOccluded(0)
	, m_renderersBeingRendered(0)
	, m_prevSortX(-9999.0f)
	, m_prevSortY(-9999.0f)
	, m_prevSortZ(-9999.0f)
	, m_damagePartialTime(0)
{
}

RenderGlobal::RenderGlobal(Minecraft* var1, RenderEngine* var2)
	: worldObj(NULL)
	, m_renderEngine(NULL)
	, m_glGenList(0)
	, m_renderIntBuffer(new int[65536])
	, m_sortedWorldRenderers(NULL)
	, m_worldRenderers(NULL)
	, m_renderChunksWide(0)
	, m_renderChunksTall(0)
	, m_renderChunksDeep(0)
	, m_glRenderListBase(0)
	, mc(NULL)
	, m_globalRenderBlocks(NULL)
	, m_occlusionEnabled(false)
	, m_cloudOffsetX(0)
	, m_glSkyList(0)
	, m_countEntitiesTotal(0)
	, m_countEntitiesRendered(0)
	, m_countEntitiesHidden(0)
	, m_dummyBuf50k(new int['\uc350'])
	, m_occlusionResult(new int[64])
	, m_renderersLoaded(0)
	, m_renderersBeingClipped(0)
	, m_renderersBeingOccluded(0)
	, m_renderersBeingRendered(0)
	, m_prevSortX(-9999.0f)
	, m_prevSortY(-9999.0f)
	, m_prevSortZ(-9999.0f)
	, m_damagePartialTime(0)
{
		mc = var1;
		m_renderEngine = var2;
  0 /* PORT: allocate 2 vertex buffer slots */
  0 /* PORT: allocate 786432 vertex buffer slots */
  // PORT: D3D9 caps check  // was: this.occlusionEnabled = GLContext::getCapabilities().GL_ARB_occlusion_query;
		if(m_occlusionEnabled) {
   memset(m_occlusionResult, 0, sizeof(m_occlusionResult));
   // PORT: use D3D9 GetTransform/GetViewport instead
			if(m_occlusionResult[0] == 0) {
				m_occlusionEnabled = false;
			} else {
				m_glOcclusionQueryBase = new int[262144];
    memset(m_glOcclusionQueryBase, 0, sizeof(m_glOcclusionQueryBase));
    // buffer m_glOcclusionQueryBase position = 0
    // NIO limit() — no-op for C array
    // PORT: D3D9 no occlusion queries on 360  // was: ARBOcclusionQuery::glGenQueriesARB(this.glOcclusionQueryBase);
			}
		}

  0 /* PORT: allocate 1 vertex buffer slots */
  // BEGIN display list -> vertex buffer recording
		Random* var5 = new Random(10842L);

		for(int var6 = 0; var6 < 500; ++var6) {
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (var5->nextFloat() * 360.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var5->nextFloat() * 360.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (var5->nextFloat() * 360.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			Tesselator var3 = Tesselator::getInstance();
			float var4 = 0.25F + var5->nextFloat() * 0.25F;
			var3.init();
			var3.vertexUV(-var4, -100.0F, var4, 1.0F, 1.0F);
			var3.vertexUV(var4, -100.0F, var4, 0.0F, 1.0F);
			var3.vertexUV(var4, -100.0F, -var4, 0.0F, 0.0F);
			var3.vertexUV(-var4, -100.0F, -var4, 1.0F, 0.0F);
			var3.flush();
		}

  // END display list -> vertex buffer recording
}

RenderGlobal::~RenderGlobal() {
	worldObj = NULL;
	m_renderEngine = NULL;
	delete[] m_sortedWorldRenderers;
	m_sortedWorldRenderers = NULL;
	delete[] m_worldRenderers;
	m_worldRenderers = NULL;
	mc = NULL;
	m_globalRenderBlocks = NULL;
	delete[] m_dummyBuf50k;
	m_dummyBuf50k = NULL;
}

void RenderGlobal::changeWorld(World* var1) {
		if(worldObj != NULL) {
			worldObj->removeWorldAccess(this);
		}

		m_prevSortX = -9999.0F;
		m_prevSortY = -9999.0F;
		m_prevSortZ = -9999.0F;
		RenderManager.instance.set(var1);
		worldObj = var1;
		m_globalRenderBlocks = new RenderBlocks(var1);
		if(var1 != NULL) {
			var1->addWorldAccess(this);
			loadRenderers();
		}

}

void RenderGlobal::loadRenderers() {
		int var1;
		if(m_worldRenderers != NULL) {
			for(var1 = 0; var1 < m_renderChunksWide * m_renderChunksTall * m_renderChunksDeep; ++var1) {
				m_worldRenderers[var1].stopRendering();
			}
		}

		m_renderChunksWide = worldObj->width / 16;
		m_renderChunksTall = worldObj->height / 16;
		m_renderChunksDeep = 4 /* PORT: quad vertex count */ / 16;
		m_worldRenderers = new WorldRenderer[m_renderChunksWide * m_renderChunksTall * m_renderChunksDeep];
		m_sortedWorldRenderers = new WorldRenderer[m_renderChunksWide * m_renderChunksTall * m_renderChunksDeep];
		var1 = 0;
		int var2 = 0;

		size_t var3;
		for(var3 = 0; var3 < m_renderChunksWide; ++var3) {
			for(int var4 = 0; var4 < m_renderChunksTall; ++var4) {
				for(int var5 = 0; var5 < m_renderChunksDeep; ++var5) {
					m_worldRenderers[(var5 * m_renderChunksTall + var4) * m_renderChunksWide + var3] = WorldRenderer(worldObj, var3 << 4, var4 << 4, var5 << 4, 16, m_glRenderListBase + var1);
					if(m_occlusionEnabled) {
						m_worldRenderers[(var5 * m_renderChunksTall + var4) * m_renderChunksWide + var3].glOcclusionQuery = m_glOcclusionQueryBase[var2];
					}

					++var2;
					m_sortedWorldRenderers[(var5 * m_renderChunksTall + var4) * m_renderChunksWide + var3] = m_worldRenderers[(var5 * m_renderChunksTall + var4) * m_renderChunksWide + var3];
					var1 += 3;
				}
			}
		}

		for(var3 = 0; var3 < m_worldRenderersToUpdate.size(); ++var3) {
			m_worldRenderersToUpdate[var3]->needsUpdate = false;
		}

  memset(m_worldRenderersToUpdate, 0, sizeof(m_worldRenderersToUpdate));
  // BEGIN display list -> vertex buffer recording
		oobGroundRenderHeight();
  // END display list -> vertex buffer recording
  // BEGIN display list -> vertex buffer recording
		oobWaterRenderHeight();
  // END display list -> vertex buffer recording
		markBlocksForUpdate(0, 0, 0, worldObj->width, worldObj->height, worldObj /* PORT: array length - use tracked size */);
}

void RenderGlobal::renderEntities(Vec3D* var1, ICamera* var2, float var3) {
		EntityMap var4 = worldObj->m_entityMap;
		RenderManager.instance.cacheActiveRenderInfo(worldObj, m_renderEngine, mc->thePlayer, var3);
		m_countEntitiesTotal = 0;
		m_countEntitiesRendered = 0;
		m_countEntitiesHidden = 0;

		for(int var5 = 0; var5 < var4.width; ++var5) {
			for(int var6 = 0; var6 < var4.m_depth; ++var6) {
				for(int var7 = 0; var7 < var4.height; ++var7) {
					std::vector<Entity> var8 = var4.m_entityGrid[(var7 * var4.m_depth + var6) * var4.width + var5];
					if(var8.size() != 0) {
						int var9 = (var5 << 3) + 4;
						int var10 = (var6 << 3) + 4;
						int var11 = (var7 << 3) + 4;
						m_countEntitiesTotal += var8.size();
						float var10001 = (float)var9;
						float var10002 = (float)var10;
						float var14 = (float)var11;
						float var13 = var10002;
						float var12 = var10001;
						bool var10000;
						if(var12 >= 0.0F && var13 >= 0.0F && var14 >= 0.0F && var12 < (float)worldObj->width && var13 < (float)worldObj->height && var14 < (float)worldObj /* PORT: array length - use tracked size */) {
							int var17 = (int)(var12 / 16.0F);
							int var19 = (int)(var13 / 16.0F);
							int var20 = (int)(var14 / 16.0F);
							var10000 = m_worldRenderers[(var20 * m_renderChunksTall + var19) * m_renderChunksWide + var17].isInFrustrum && m_worldRenderers[(var20 * m_renderChunksTall + var19) * m_renderChunksWide + var17].isVisible;
						} else {
							var10000 = true;
						}

						if(var10000) {
							for(var9 = 0; var9 < var8.size(); ++var9) {
								Entity var15 = (Entity)var8[var9];
								var13 = var15.posX - var1->m_xCoord;
								var14 = var15.posY - var1->m_yCoord;
								var12 = var15.posZ - var1->m_zCoord;
								var13 = var13 * var13 + var14 * var14 + var12 * var12;
								float var16 = var13;
								AxisAlignedBB var18 = var15.boundingBox;
								var13 = var18.maxX - var18.minX;
								var14 = var18.maxY - var18.minY;
								var12 = var18.maxZ - var18.minZ;
								var12 = (var13 + var14 + var12) / 3.0F;
								var12 *= 64.0F;
								if(var16 < var12 * var12 && var2->isBoundingBoxInFrustrum(var15.boundingBox) && var15 != worldObj->m_playerEntity) {
									++m_countEntitiesRendered;
									RenderManager.instance.renderEntity(var15, var3);
								}
							}
						} else {
							m_countEntitiesHidden += var8.size();
						}
					}
				}
			}
		}

}

const std::string& RenderGlobal::getDebugInfoRenders() {
		return "C: " + m_renderersBeingRendered + "/" + m_renderersLoaded + ". F: " + m_renderersBeingClipped + ", O: " + m_renderersBeingOccluded;
}

const std::string& RenderGlobal::getDebugInfoEntities() {
		return "E: " + m_countEntitiesRendered + "/" + m_countEntitiesTotal + ". B: " + m_countEntitiesHidden + ", I: " + (m_countEntitiesTotal - m_countEntitiesHidden - m_countEntitiesRendered);
}

int RenderGlobal::sortAndRender(EntityPlayer* var1, int var2) {
		if(var2 == 0) {
			m_renderersLoaded = 0;
			m_renderersBeingClipped = 0;
			m_renderersBeingOccluded = 0;
			m_renderersBeingRendered = 0;
		}

		float var3 = var1->posX - m_prevSortX;
		float var4 = var1->posY - m_prevSortY;
		float var5 = var1->posZ - m_prevSortZ;
		if(var3 * var3 + var4 * var4 + var5 * var5 > 16.0F) {
			m_prevSortX = var1->posX;
			m_prevSortY = var1->posY;
			m_prevSortZ = var1->posZ;
			Arrays::sort(m_sortedWorldRenderers, new EntitySorter(var1));
		}

		int var8;
		if(m_occlusionEnabled && var2 == 0) {
			int var10 = 8;
			checkOcclusionQueryResult(0, 8);

			int var6;
			for(var6 = 0; var6 < 8; ++var6) {
				m_sortedWorldRenderers[var6].isVisible = true;
			}

			var8 = 0 + renderSortedRenderers(0, 8, var2);

			do {
				int var9 = var10;
				var10 <<= 1;
				if(var10 > m_sortedWorldRenderers /* PORT: array length - use tracked size */) {
					var10 = m_renderChunksWide * m_renderChunksTall * m_renderChunksDeep;
				}

    g_pd3dDevice->SetTexture(0, NULL);
    // Lighting is shader-based on Xbox 360 (disabled)
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				checkOcclusionQueryResult(var9, var10);

				for(var6 = var9; var6 < var10; ++var6) {
					if(!m_sortedWorldRenderers[var6].isInFrustrum) {
						m_sortedWorldRenderers[var6].isVisible = true;
					}

					if(m_sortedWorldRenderers[var6].isInFrustrum && !m_sortedWorldRenderers[var6].isWaitingOnOcclusionQuery) {
						float var7 = sqrtf(m_sortedWorldRenderers[var6].distanceToEntitySquared(var1));
						int var11 = (int)(1.0F + var7 / 64.0F);
						if(m_cloudOffsetX % var11 == var6 % var11) {
       // PORT: D3D9 no occlusion queries on 360  // was: ARBOcclusionQuery::glBeginQueryARB(GL15.GL_SAMPLES_PASSED, this.sortedWorldRenderers[var6].glOcclusionQuery);
							m_sortedWorldRenderers[var6].callOcclusionQueryList();
       // PORT: D3D9 no occlusion queries on 360  // was: ARBOcclusionQuery::glEndQueryARB(GL15.GL_SAMPLES_PASSED);
							m_sortedWorldRenderers[var6].isWaitingOnOcclusionQuery = true;
						}
					}
				}

    g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				var8 += renderSortedRenderers(var9, var10, var2);
			} while(var10 < m_renderChunksWide * m_renderChunksTall * m_renderChunksDeep);
		} else {
			var8 = 0 + renderSortedRenderers(0, m_sortedWorldRenderers /* PORT: array length - use tracked size */, var2);
		}

		return var8;
}

void RenderGlobal::checkOcclusionQueryResult(int var1, int var2) {
		for(var1 = var1; var1 < var2; ++var1) {
			if(m_sortedWorldRenderers[var1].isWaitingOnOcclusionQuery) {
    memset(m_occlusionResult, 0, sizeof(m_occlusionResult));
    // Occlusion/VBO query — not used on Xbox 360
				if(m_occlusionResult[0] != 0) {
					m_sortedWorldRenderers[var1].isWaitingOnOcclusionQuery = false;
     memset(m_occlusionResult, 0, sizeof(m_occlusionResult));
     // Occlusion/VBO query — not used on Xbox 360
					m_sortedWorldRenderers[var1].isVisible = m_occlusionResult[0] != 0;
				}
			}
		}

}

int RenderGlobal::renderSortedRenderers(int var1, int var2, int var3) {
		int var4 = 0;

		for(var1 = var1; var1 < var2; ++var1) {
			if(var3 == 0) {
				++m_renderersLoaded;
				if(!m_sortedWorldRenderers[var1].isInFrustrum) {
					++m_renderersBeingClipped;
				}

				if(m_sortedWorldRenderers[var1].isInFrustrum && !m_sortedWorldRenderers[var1].isVisible) {
					++m_renderersBeingOccluded;
				}

				if(m_sortedWorldRenderers[var1].isInFrustrum && m_sortedWorldRenderers[var1].isVisible) {
					++m_renderersBeingRendered;
				}
			}

			if(m_sortedWorldRenderers[var1].isInFrustrum && m_sortedWorldRenderers[var1].isVisible) {
				var4 = m_sortedWorldRenderers[var1].getGLCallListForPass(m_dummyBuf50k, var4, var3);
			}
		}

  memset(m_renderIntBuffer, 0, sizeof(m_renderIntBuffer));
  m_renderIntBuffer[0] = m_dummyBuf50k, 0, var4;
  // NIO flip() — no-op for C array
  // NIO remaining() — no-op for C array
   // PORT: batch draw cached vertex buffers
		}

  // NIO remaining() — no-op for C array
}

void RenderGlobal::renderAllRenderLists() {
  g_pd3dDevice->SetTexture(0, Textures::getById(this.m_renderEngine->getTexture("/terrain.png")));
  // PORT: batch draw cached vertex buffers
}

void RenderGlobal::updateClouds() {
		++m_cloudOffsetX;
}

void RenderGlobal::renderSky(float var1) {
  g_pd3dDevice->SetTexture(0, NULL);
		Vec3.0f var2 = worldObj->getSkyColor(var1);
		float var3 = var2.m_xCoord;
		float var4 = var2.m_yCoord;
		float var9 = var2.m_zCoord;
		if(mc->m_options.m_anaglyph) {
			float var5 = (var3 * 30.0F + var4 * 59.0F + var9 * 11.0F) / 100.0F;
			var4 = (var3 * 30.0F + var4 * 70.0F) / 100.0F;
			var9 = (var3 * 30.0F + var9 * 70.0F) / 100.0F;
			var3 = var5;
			var4 = var4;
			var9 = var9;
		}

  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		Tesselator var12 = Tesselator::getInstance();
		var12.init();
		var12.color3f(var3, var4, var9);
		var9 = (float)(worldObj->height + 10);

		int var10;
		for(var10 = -2048; var10 < worldObj->width + 2048; var10 += 512) {
			for(int var13 = -2048; var13 < worldObj_count /* PORT: set this from allocation */ + 2048; var13 += 512) {
				var12.vertex((float)var10, var9, (float)var13);
				var12.vertex((float)(var10 + 512), var9, (float)var13);
				var12.vertex((float)(var10 + 512), var9, (float)(var13 + 512));
				var12.vertex((float)var10, var9, (float)(var13 + 512));
			}
		}

		var12.flush();
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  Shaders::setFogEnabled(false);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		var3 = worldObj->m_playerEntity.lastTickPosX + (worldObj->m_playerEntity.posX - worldObj->m_playerEntity.lastTickPosX) * var1;
		var4 = worldObj->m_playerEntity.lastTickPosY + (worldObj->m_playerEntity.posY - worldObj->m_playerEntity.lastTickPosY) * var1;
		float var6 = worldObj->m_playerEntity.lastTickPosZ + (worldObj->m_playerEntity.posZ - worldObj->m_playerEntity.lastTickPosZ) * var1;
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var3, var4, var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (0.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (this.worldObj->getCelestialAngle(var1) * 360.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  g_pd3dDevice->SetTexture(0, Textures::getById(this.m_renderEngine->getTexture("/terrain/sun.png")));
		var12.init();
		var12.vertexUV(-30.0F, 100.0F, -30.0F, 0.0F, 0.0F);
		var12.vertexUV(30.0F, 100.0F, -30.0F, 1.0F, 0.0F);
		var12.vertexUV(30.0F, 100.0F, 30.0F, 1.0F, 1.0F);
		var12.vertexUV(-30.0F, 100.0F, 30.0F, 0.0F, 1.0F);
		var12.flush();
  g_pd3dDevice->SetTexture(0, Textures::getById(this.m_renderEngine->getTexture("/terrain/moon.png")));
		var12.init();
		var12.vertexUV(-20.0F, -100.0F, 20.0F, 1.0F, 1.0F);
		var12.vertexUV(20.0F, -100.0F, 20.0F, 0.0F, 1.0F);
		var12.vertexUV(20.0F, -100.0F, -20.0F, 0.0F, 0.0F);
		var12.vertexUV(-20.0F, -100.0F, -20.0F, 1.0F, 0.0F);
		var12.flush();
  g_pd3dDevice->SetTexture(0, NULL);
		var9 = worldObj->getStarBrightness(var1);
  Tesselator::getInstance().color4f(var9, var9, var9, var9);
  // Draw cached vertex buffer for list this.glSkyList
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  Shaders::setFogEnabled(true);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  g_pd3dDevice->SetTexture(0, Textures::getById(this.m_renderEngine->getTexture("/clouds.png")));
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		Vec3.0f var11 = worldObj->getCloudColor(var1);
		var4 = var11.m_xCoord;
		var6 = var11.m_yCoord;
		var3 = var11.m_zCoord;
		float var7;
		if(mc->m_options.m_anaglyph) {
			var9 = (var4 * 30.0F + var6 * 59.0F + var3 * 11.0F) / 100.0F;
			var7 = (var4 * 30.0F + var6 * 70.0F) / 100.0F;
			var3 = (var4 * 30.0F + var3 * 70.0F) / 100.0F;
			var4 = var9;
			var6 = var7;
			var3 = var3;
		}

		var9 = (float)worldObj->m_cloudHeight;
		var7 = ((float)m_cloudOffsetX + var1) * (0.5F / 1024.0F) * 0.03F;
		var12.init();
		var12.color3f(var4, var6, var3);

		for(int var8 = -2048; var8 < worldObj->width + 2048; var8 += 512) {
			for(var10 = -2048; var10 < worldObj_count /* PORT: set this from allocation */ + 2048; var10 += 512) {
				var12.vertexUV((float)var8, var9, (float)(var10 + 512), (float)var8 * (0.5F / 1024.0F) + var7, (float)(var10 + 512) * (0.5F / 1024.0F));
				var12.vertexUV((float)(var8 + 512), var9, (float)(var10 + 512), (float)(var8 + 512) * (0.5F / 1024.0F) + var7, (float)(var10 + 512) * (0.5F / 1024.0F));
				var12.vertexUV((float)(var8 + 512), var9, (float)var10, (float)(var8 + 512) * (0.5F / 1024.0F) + var7, (float)var10 * (0.5F / 1024.0F));
				var12.vertexUV((float)var8, var9, (float)var10, (float)var8 * (0.5F / 1024.0F) + var7, (float)var10 * (0.5F / 1024.0F));
				var12.vertexUV((float)var8, var9, (float)var10, (float)var8 * (0.5F / 1024.0F) + var7, (float)var10 * (0.5F / 1024.0F));
				var12.vertexUV((float)(var8 + 512), var9, (float)var10, (float)(var8 + 512) * (0.5F / 1024.0F) + var7, (float)var10 * (0.5F / 1024.0F));
				var12.vertexUV((float)(var8 + 512), var9, (float)(var10 + 512), (float)(var8 + 512) * (0.5F / 1024.0F) + var7, (float)(var10 + 512) * (0.5F / 1024.0F));
				var12.vertexUV((float)var8, var9, (float)(var10 + 512), (float)var8 * (0.5F / 1024.0F) + var7, (float)(var10 + 512) * (0.5F / 1024.0F));
			}
		}

		var12.flush();
}

void RenderGlobal::oobGroundRenderer() {
		float var1 = worldObj->getLightBrightness(0, worldObj->getGroundLevel(), 0);
  g_pd3dDevice->SetTexture(0, Textures::getById(this.m_renderEngine->getTexture("/dirt.png")));
		if(worldObj->getGroundLevel() > worldObj->getWaterLevel() && worldObj->m_defaultFluid == Block::s_waterMoving->blockID) {
   g_pd3dDevice->SetTexture(0, Textures::getById(this.m_renderEngine->getTexture("/grass.png")));
		}

  Tesselator::getInstance().color4f(var1, var1, var1, 1.0F);
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  // Draw cached vertex buffer for list this.glGenList
}

void RenderGlobal::oobGroundRenderHeight() {
		Tesselator var1 = Tesselator::getInstance();
		float var2 = (float)worldObj->getGroundLevel();
		int var3 = 128;
		if(128 > worldObj->width) {
			var3 = worldObj->width;
		}

		if(var3 > worldObj /* PORT: array length - use tracked size */) {
			var3 = 4 /* PORT: quad vertex count */;
		}

		int var4 = 2048 / var3;
		var1.init();

		for(int var5 = -var3 * var4; var5 < worldObj->width + var3 * var4; var5 += var3) {
			for(int var6 = -var3 * var4; var6 < worldObj_count /* PORT: set this from allocation */ + var3 * var4; var6 += var3) {
				if(var2 < 0.0F || var5 < 0 || var6 < 0 || var5 >= worldObj->width || var6 >= 4 /* PORT: quad vertex count */) {
					var1.vertexUV((float)var5, var2, (float)(var6 + var3), 0.0F, (float)var3);
					var1.vertexUV((float)(var5 + var3), var2, (float)(var6 + var3), (float)var3, (float)var3);
					var1.vertexUV((float)(var5 + var3), var2, (float)var6, (float)var3, 0.0F);
					var1.vertexUV((float)var5, var2, (float)var6, 0.0F, 0.0F);
				}
			}
		}

		var1.flush();
}

void RenderGlobal::oobWaterRenderer() {
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetTexture(0, Textures::getById(this.m_renderEngine->getTexture("/water.png")));
		float var1 = worldObj->getLightBrightness(0, worldObj->getWaterLevel(), 0);
  Tesselator::getInstance().color4f(var1, var1, var1, 1.0F);
  // Draw cached vertex buffer for list this.glGenList + 1
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void RenderGlobal::oobWaterRenderHeight() {
		float var1 = (float)worldObj->getWaterLevel();
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		Tesselator var2 = Tesselator::getInstance();
		int var3 = 128;
		if(128 > worldObj->width) {
			var3 = worldObj->width;
		}

		if(var3 > worldObj /* PORT: array length - use tracked size */) {
			var3 = 4 /* PORT: quad vertex count */;
		}

		int var4 = 2048 / var3;
		var2.init();
		float var5 = Block::s_waterMoving->minX;
		float var6 = Block::s_waterMoving->minZ;

		for(int var7 = -var3 * var4; var7 < worldObj->width + var3 * var4; var7 += var3) {
			for(int var8 = -var3 * var4; var8 < worldObj_count /* PORT: set this from allocation */ + var3 * var4; var8 += var3) {
				float var9 = var1 + Block::s_waterMoving->minY;
				if(var1 < 0.0F || var7 < 0 || var8 < 0 || var7 >= worldObj->width || var8 >= 4 /* PORT: quad vertex count */) {
					var2.vertexUV((float)var7 + var5, var9, (float)(var8 + var3) + var6, 0.0F, (float)var3);
					var2.vertexUV((float)(var7 + var3) + var5, var9, (float)(var8 + var3) + var6, (float)var3, (float)var3);
					var2.vertexUV((float)(var7 + var3) + var5, var9, (float)var8 + var6, (float)var3, 0.0F);
					var2.vertexUV((float)var7 + var5, var9, (float)var8 + var6, 0.0F, 0.0F);
					var2.vertexUV((float)var7 + var5, var9, (float)var8 + var6, 0.0F, 0.0F);
					var2.vertexUV((float)(var7 + var3) + var5, var9, (float)var8 + var6, (float)var3, 0.0F);
					var2.vertexUV((float)(var7 + var3) + var5, var9, (float)(var8 + var3) + var6, (float)var3, (float)var3);
					var2.vertexUV((float)var7 + var5, var9, (float)(var8 + var3) + var6, 0.0F, (float)var3);
				}
			}
		}

		var2.flush();
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void RenderGlobal::updateRenderers(EntityPlayer* var1) {
		Collections::sort(m_worldRenderersToUpdate, new RenderSorter(var1));
		int var2 = m_worldRenderersToUpdate.size() - 1;
		int var3 = m_worldRenderersToUpdate.size();

		for(int var4 = 0; var4 < var3; ++var4) {
			WorldRenderer var5 = (WorldRenderer)m_worldRenderersToUpdate.get(var2 - var4);
			if(var5.distanceToEntitySquared(var1) > 2500.0F && var4 > 4) {
				return;
			}

   std::vector<void*>::iterator _removeIt = std::find(m_worldRenderersToUpdate.begin(), m_worldRenderersToUpdate.end(), var5);
   if (_removeIt != m_worldRenderersToUpdate.end()) m_worldRenderersToUpdate.erase(_removeIt);
			var5.updateRenderer();
			var5.needsUpdate = false;
		}

}

void RenderGlobal::drawBlockBreaking(MovingObjectPosition* var1, int var2, ItemStack* var3) {
		Tesselator var4 = Tesselator::getInstance();
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, (MathHelper::sin((float)System.currentTimeMillis() / 100.0F) * 0.2F + 0.4F) * 0.5F);
		if(m_damagePartialTime > 0.0F) {
   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			int var5 = m_Textures::load("game:\Content\terrain.png");
   g_pd3dDevice->SetTexture(0, Textures::getById(var5));
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 0.5F);
   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			var5 = worldObj->getBlockId(var1->m_blockX, var1->m_blockY, var1->m_blockZ);
			Block var6 = var5 > 0 ? Block::blocksList[var5] : NULL;
   g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			var4.init();
			var4.disableColor();
			if(var6 == NULL) {
				var6 = Block::s_stone;
			}

			m_globalRenderBlocks->renderBlockUsingTexture(var6, var1->m_blockX, var1->m_blockY, var1->m_blockZ, 240 + (int)(m_damagePartialTime * 10.0F));
			var4.flush();
   g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
   Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		}

  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void RenderGlobal::drawSelectionBox(MovingObjectPosition* var1, int var2) {
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  Tesselator::getInstance().color4f(0.0F, 0.0F, 0.0F, 0.4F);
  // Line width not supported on Xbox 360 GPU
  g_pd3dDevice->SetTexture(0, NULL);
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		var2 = worldObj->getBlockId(var1->m_blockX, var1->m_blockY, var1->m_blockZ);
		if(var2 > 0) {
			AxisAlignedBB var3 = Block::blocksList[var2].getSelectedBoundingBoxFromPool(var1->m_blockX, var1->m_blockY, var1->m_blockZ).expand(0.002F, 0.002F, 0.002F);
			Tesselator var4 = Tesselator::getInstance();
			var4.init( /* PORT: draw mode */ 3);
			var4.vertex(var3.minX, var3.minY, var3.minZ);
			var4.vertex(var3.maxX, var3.minY, var3.minZ);
			var4.vertex(var3.maxX, var3.minY, var3.maxZ);
			var4.vertex(var3.minX, var3.minY, var3.maxZ);
			var4.vertex(var3.minX, var3.minY, var3.minZ);
			var4.flush();
			var4.init( /* PORT: draw mode */ 3);
			var4.vertex(var3.minX, var3.maxY, var3.minZ);
			var4.vertex(var3.maxX, var3.maxY, var3.minZ);
			var4.vertex(var3.maxX, var3.maxY, var3.maxZ);
			var4.vertex(var3.minX, var3.maxY, var3.maxZ);
			var4.vertex(var3.minX, var3.maxY, var3.minZ);
			var4.flush();
			var4.init( /* PORT: draw mode */ 1);
			var4.vertex(var3.minX, var3.minY, var3.minZ);
			var4.vertex(var3.minX, var3.maxY, var3.minZ);
			var4.vertex(var3.maxX, var3.minY, var3.minZ);
			var4.vertex(var3.maxX, var3.maxY, var3.minZ);
			var4.vertex(var3.maxX, var3.minY, var3.maxZ);
			var4.vertex(var3.maxX, var3.maxY, var3.maxZ);
			var4.vertex(var3.minX, var3.minY, var3.maxZ);
			var4.vertex(var3.minX, var3.maxY, var3.maxZ);
			var4.flush();
		}

  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void RenderGlobal::markBlocksForUpdate(int var1, int var2, int var3, int var4, int var5, int var6) {
		var1 /= 16;
		var2 /= 16;
		var3 /= 16;
		var4 /= 16;
		var5 /= 16;
		var6 /= 16;
		if(var1 < 0) {
			var1 = 0;
		}

		if(var2 < 0) {
			var2 = 0;
		}

		if(var3 < 0) {
			var3 = 0;
		}

		if(var4 > m_renderChunksWide - 1) {
			var4 = m_renderChunksWide - 1;
		}

		if(var5 > m_renderChunksTall - 1) {
			var5 = m_renderChunksTall - 1;
		}

		if(var6 > m_renderChunksDeep - 1) {
			var6 = m_renderChunksDeep - 1;
		}

		for(var1 = var1; var1 <= var4; ++var1) {
			for(int var7 = var2; var7 <= var5; ++var7) {
				for(int var8 = var3; var8 <= var6; ++var8) {
					WorldRenderer var9 = m_worldRenderers[(var8 * m_renderChunksTall + var7) * m_renderChunksWide + var1];
					if(!var9.needsUpdate) {
						var9.needsUpdate = true;
						m_worldRenderersToUpdate.push_back(m_worldRenderers[(var8 * m_renderChunksTall + var7) * m_renderChunksWide + var1]);
					}
				}
			}
		}

}

void RenderGlobal::markBlockAndNeighborsNeedsUpdate(int var1, int var2, int var3) {
		markBlocksForUpdate(var1 - 1, var2 - 1, var3 - 1, var1 + 1, var2 + 1, var3 + 1);
}

void RenderGlobal::markBlockRangeNeedsUpdate(int var1, int var2, int var3, int var4, int var5, int var6) {
		markBlocksForUpdate(var1 - 1, var2 - 1, var3 - 1, var4 + 1, var5 + 1, var6 + 1);
}

void RenderGlobal::clipRenderersByFrustrum(ICamera* var1) {
		for(int var2 = 0; var2 < m_renderChunksWide * m_renderChunksTall * m_renderChunksDeep; ++var2) {
			m_worldRenderers[var2].updateInFrustrum(var1);
		}

}

void RenderGlobal::playSound(const std::string& var1, float var2, float var3, float var4, float var5, float var6) {
		mc->m_sndManager.playSound(var1, var2, var3, var4, var5, var6);
}

void RenderGlobal::spawnParticle(const std::string& var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = worldObj->m_playerEntity.posX - var2;
		float var9 = worldObj->m_playerEntity.posY - var3;
		float var10 = worldObj->m_playerEntity.posZ - var4;
		if(var8 * var8 + var9 * var9 + var10 * var10 <= 256.0F) {
			if(var1 == "bubble") {
				mc->m_effectRenderer.addEffect(new EntityBubbleFX(worldObj, var2, var3, var4, var5, var6, var7));
			} else if(var1 == "smoke") {
				mc->m_effectRenderer.addEffect(new EntitySmokeFX(worldObj, var2, var3, var4));
			} else if(var1 == "explode") {
				mc->m_effectRenderer.addEffect(new EntityExplodeFX(worldObj, var2, var3, var4, var5, var6, var7));
			} else if(var1 == "flame") {
				mc->m_effectRenderer.addEffect(new EntityFlameFX(worldObj, var2, var3, var4));
			} else if(var1 == "lava") {
				mc->m_effectRenderer.addEffect(new EntityLavaFX(worldObj, var2, var3, var4));
			} else if(var1 == "splash") {
				mc->m_effectRenderer.addEffect(new EntitySplashFX(worldObj, var2, var3, var4));
			} else {
				if(var1 == "largesmoke") {
					mc->m_effectRenderer.addEffect(new EntitySmokeFX(worldObj, var2, var3, var4, 2.5F));
				}

			}
		}
}

void RenderGlobal::playMusic(const std::string& var1, float var2, float var3, float var4, float var5) {
		mc->m_sndManager.playRandomMusicIfReady(var2, var3, var4);
}

void RenderGlobal::obtainEntitySkin(Entity* var1) {
		if(var1->skinUrl != NULL) {
			m_renderEngine->obtainImageData(var1->skinUrl, new ImageBufferDownload());
		}

}

void RenderGlobal::releaseEntitySkin(Entity* var1) {
		if(var1->skinUrl != NULL) {
			m_renderEngine->releaseImageData(var1->skinUrl);
		}

}

void RenderGlobal::updateAllRenderers() {
  // BEGIN display list -> vertex buffer recording
		oobGroundRenderHeight();
  // END display list -> vertex buffer recording
  // BEGIN display list -> vertex buffer recording
		oobWaterRenderHeight();
  // END display list -> vertex buffer recording
}
