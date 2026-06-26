#include "stdafx.h"
// Auto-generated from Render.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "Entity.h"
#include "FontRenderer.h"
#include "ModelBase.h"
#include "RenderBlocks.h"
#include "RenderManager.h"
#include "World.h"
#include <string>
#include "ModelBiped.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "BlockFire.h"
#include "GameSettings.h"
#include "RenderEngine.h"

Render::Render()
	: renderManager(NULL)
	, m_unusedModelBiped(new ModelBiped())
	, m_unusedRenderBlocks(new RenderBlocks())
	, m_field_9246_c(0.0f)
	, m_field_194_c(1.0f)
{
}

Render::~Render() {
	renderManager = NULL;
	m_unusedModelBiped = NULL;
	m_unusedRenderBlocks = NULL;
}

void Render::loadTexture(const std::string& var1) {
		RenderEngine* var2 = renderManager->m_renderEngine;
		var2->bindTexture(var2->getTexture(var1));
}

void Render::func_140_a(const std::string& var1, const std::string& var2) {
		RenderEngine* var3 = renderManager->m_renderEngine;
		var3->bindTexture(var3->getTextureForDownloadableImage(var1, var2));
}

void Render::renderEntityOnFire(Entity* var1, double var2, double var4, double var6, float var8) {
  // Lighting is shader-based on Xbox 360 (disabled)
		int var9 = Block::s_fire->blockIndexInTexture;
		int var10 = (var9 & 15) << 4;
		int var11 = var9 & 240;
		float var12 = (float)var10 / 256.0F;
		float var13 = ((float)var10 + 15.99F) / 256.0F;
		float var14 = (float)var11 / 256.0F;
		float var15 = ((float)var11 + 15.99F) / 256.0F;
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var16 = var1->width * 1.4F;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var16, var16, var16);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		// PORT: bind terrain.png (was Textures::load(...) with the result discarded -> nothing bound)
		renderManager->m_renderEngine->bindTexture(Textures::load("game:\\Content\\terrain.png"));
		Tesselator& var17 = Tesselator::getInstance();
		float var18 = 1.0F;
		float var19 = 0.5F;
		float var20 = 0.0F;
		float var21 = var1->height / var1->width;
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-this->renderManager->m_field_1225_i)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, -0.4F + (float)((int)var21) * 0.02F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  var17.color4f(1.0F, 1.0F, 1.0F, 1.0F);
		var17.init();

		while(var21 > 0.0F) {
			var17.vertexUV((float)(var18 - var19), (float)(0.0F - var20), 0.0f, (float)var13, (float)var15);
			var17.vertexUV((float)(0.0F - var19), (float)(0.0F - var20), 0.0f, (float)var12, (float)var15);
			var17.vertexUV((float)(0.0F - var19), (float)(1.4F - var20), 0.0f, (float)var12, (float)var14);
			var17.vertexUV((float)(var18 - var19), (float)(1.4F - var20), 0.0f, (float)var13, (float)var14);
			--var21;
			--var20;
			var18 *= 0.9F;
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, -0.04F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		var17.flush();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
  // Lighting is shader-based on Xbox 360 (enabled)
}

void Render::renderShadow(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		RenderEngine* var10 = renderManager->m_renderEngine;
		var10->bindTexture(Textures::load("game:\\Content\\misc\\shadow.png"));
		World* var11 = getWorldFromRenderManager();
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		float var12 = m_field_9246_c;
		double var13 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var9;
		double var15 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var9 + (double)var1->func_392_h_();
		double var17 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var9;
		int var19 = (int)floor(var13 - (double)var12);
		int var20 = (int)floor(var13 + (double)var12);
		int var21 = (int)floor(var15 - (double)var12);
		int var22 = (int)floor(var15);
		int var23 = (int)floor(var17 - (double)var12);
		int var24 = (int)floor(var17 + (double)var12);
		double var25 = var2 - var13;
		double var27 = var4 - var15;
		double var29 = var6 - var17;
		Tesselator& var31 = Tesselator::getInstance();
		var31.init();

		for(int var32 = var19; var32 <= var20; ++var32) {
			for(int var33 = var21; var33 <= var22; ++var33) {
				for(int var34 = var23; var34 <= var24; ++var34) {
					int var35 = var11->getBlockId(var32, var33 - 1, var34);
					if(var35 > 0 && var11->getBlockLightValue(var32, var33, var34) > 3) {
						renderShadowOnBlock(Block::blocksList[var35], var2, var4 + (double)var1->func_392_h_(), var6, var32, var33, var34, var8, var12, var25, var27 + (double)var1->func_392_h_(), var29);
					}
				}
			}
		}

		var31.flush();
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

World* Render::getWorldFromRenderManager() {
		return renderManager->worldObj;
}

void Render::renderShadowOnBlock(Block* var1, double var2, double var4, double var6, int var8, int var9, int var10, float var11, float var12, double var13, double var15, double var17) {
		Tesselator& var19 = Tesselator::getInstance();
		if(var1->renderAsNormalBlock()) {
			double var20 = ((double)var11 - (var4 - ((double)var9 + var15)) / 2.0f) * 0.5f * (double)getWorldFromRenderManager()->getLightBrightness(var8, var9, var10);
			if(var20 >= 0.0f) {
				if(var20 > 1.0f) {
					var20 = 1.0f;
				}

				var19.color4f(1.0F, 1.0F, 1.0F, (float)var20);
				double var22 = (double)var8 + var1->m_field_370_bf + var13;
				double var24 = (double)var8 + var1->maxX + var13;
				double var26 = (double)var9 + var1->minY + var15 + 1.0f / 64.0f;
				double var28 = (double)var10 + var1->minZ + var17;
				double var30 = (double)var10 + var1->maxZ + var17;
				float var32 = (float)((var2 - var22) / 2.0f / (double)var12 + 0.5f);
				float var33 = (float)((var2 - var24) / 2.0f / (double)var12 + 0.5f);
				float var34 = (float)((var6 - var28) / 2.0f / (double)var12 + 0.5f);
				float var35 = (float)((var6 - var30) / 2.0f / (double)var12 + 0.5f);
				var19.vertexUV((float)var22, (float)var26, (float)var28, (float)var32, (float)var34);
				var19.vertexUV((float)var22, (float)var26, (float)var30, (float)var32, (float)var35);
				var19.vertexUV((float)var24, (float)var26, (float)var30, (float)var33, (float)var35);
				var19.vertexUV((float)var24, (float)var26, (float)var28, (float)var33, (float)var34);
			}
		}
}

void Render::renderOffsetAABB(AxisAlignedBB var0, double var1, double var3, double var5) {
  g_pd3dDevice->SetTexture(0, NULL);
		Tesselator& var7 = Tesselator::getInstance();
  var7.color4f(1.0F, 1.0F, 1.0F, 1.0F);
		var7.init();
		// PORT: Tesselator has no setTranslationD; fold the (var1,var3,var5) offset into every vertex coord.
		var7.normal(0.0F, 0.0F, -1.0F);
		var7.vertex((float)(var0.minX + var1), (float)(var0.maxY + var3), (float)(var0.minZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.maxY + var3), (float)(var0.minZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.minY + var3), (float)(var0.minZ + var5));
		var7.vertex((float)(var0.minX + var1), (float)(var0.minY + var3), (float)(var0.minZ + var5));
		var7.normal(0.0F, 0.0F, 1.0F);
		var7.vertex((float)(var0.minX + var1), (float)(var0.minY + var3), (float)(var0.maxZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.minY + var3), (float)(var0.maxZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.maxY + var3), (float)(var0.maxZ + var5));
		var7.vertex((float)(var0.minX + var1), (float)(var0.maxY + var3), (float)(var0.maxZ + var5));
		var7.normal(0.0F, -1.0F, 0.0F);
		var7.vertex((float)(var0.minX + var1), (float)(var0.minY + var3), (float)(var0.minZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.minY + var3), (float)(var0.minZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.minY + var3), (float)(var0.maxZ + var5));
		var7.vertex((float)(var0.minX + var1), (float)(var0.minY + var3), (float)(var0.maxZ + var5));
		var7.normal(0.0F, 1.0F, 0.0F);
		var7.vertex((float)(var0.minX + var1), (float)(var0.maxY + var3), (float)(var0.maxZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.maxY + var3), (float)(var0.maxZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.maxY + var3), (float)(var0.minZ + var5));
		var7.vertex((float)(var0.minX + var1), (float)(var0.maxY + var3), (float)(var0.minZ + var5));
		var7.normal(-1.0F, 0.0F, 0.0F);
		var7.vertex((float)(var0.minX + var1), (float)(var0.minY + var3), (float)(var0.maxZ + var5));
		var7.vertex((float)(var0.minX + var1), (float)(var0.maxY + var3), (float)(var0.maxZ + var5));
		var7.vertex((float)(var0.minX + var1), (float)(var0.maxY + var3), (float)(var0.minZ + var5));
		var7.vertex((float)(var0.minX + var1), (float)(var0.minY + var3), (float)(var0.minZ + var5));
		var7.normal(1.0F, 0.0F, 0.0F);
		var7.vertex((float)(var0.maxX + var1), (float)(var0.minY + var3), (float)(var0.minZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.maxY + var3), (float)(var0.minZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.maxY + var3), (float)(var0.maxZ + var5));
		var7.vertex((float)(var0.maxX + var1), (float)(var0.minY + var3), (float)(var0.maxZ + var5));
		var7.flush();
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
}

void Render::renderAABB(AxisAlignedBB var0) {
		Tesselator& var1 = Tesselator::getInstance();
		var1.init();
		var1.vertex((float)var0.minX, (float)var0.maxY, (float)var0.minZ);
		var1.vertex((float)var0.maxX, (float)var0.maxY, (float)var0.minZ);
		var1.vertex((float)var0.maxX, (float)var0.minY, (float)var0.minZ);
		var1.vertex((float)var0.minX, (float)var0.minY, (float)var0.minZ);
		var1.vertex((float)var0.minX, (float)var0.minY, (float)var0.maxZ);
		var1.vertex((float)var0.maxX, (float)var0.minY, (float)var0.maxZ);
		var1.vertex((float)var0.maxX, (float)var0.maxY, (float)var0.maxZ);
		var1.vertex((float)var0.minX, (float)var0.maxY, (float)var0.maxZ);
		var1.vertex((float)var0.minX, (float)var0.minY, (float)var0.minZ);
		var1.vertex((float)var0.maxX, (float)var0.minY, (float)var0.minZ);
		var1.vertex((float)var0.maxX, (float)var0.minY, (float)var0.maxZ);
		var1.vertex((float)var0.minX, (float)var0.minY, (float)var0.maxZ);
		var1.vertex((float)var0.minX, (float)var0.maxY, (float)var0.maxZ);
		var1.vertex((float)var0.maxX, (float)var0.maxY, (float)var0.maxZ);
		var1.vertex((float)var0.maxX, (float)var0.maxY, (float)var0.minZ);
		var1.vertex((float)var0.minX, (float)var0.maxY, (float)var0.minZ);
		var1.vertex((float)var0.minX, (float)var0.minY, (float)var0.maxZ);
		var1.vertex((float)var0.minX, (float)var0.maxY, (float)var0.maxZ);
		var1.vertex((float)var0.minX, (float)var0.maxY, (float)var0.minZ);
		var1.vertex((float)var0.minX, (float)var0.minY, (float)var0.minZ);
		var1.vertex((float)var0.maxX, (float)var0.minY, (float)var0.minZ);
		var1.vertex((float)var0.maxX, (float)var0.maxY, (float)var0.minZ);
		var1.vertex((float)var0.maxX, (float)var0.maxY, (float)var0.maxZ);
		var1.vertex((float)var0.maxX, (float)var0.minY, (float)var0.maxZ);
		var1.flush();
}

void Render::setRenderManager(RenderManager* var1) {
		renderManager = var1;
}

void Render::doRenderShadowAndFire(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		if(renderManager->m_options->m_fancyGraphics && m_field_9246_c > 0.0F) {
			double var10 = renderManager->func_851_a(var1->posX, var1->posY, var1->posZ);
			float var12 = (float)((1.0f - var10 / 256.0f) * (double)m_field_194_c);
			if(var12 > 0.0F) {
				renderShadow(var1, var2, var4, var6, var12, var9);
			}
		}

		if(var1->fire > 0 || var1->m_field_9299_bv) {
			renderEntityOnFire(var1, var2, var4, var6, var9);
		}

}

FontRenderer* Render::getFontRendererFromRenderManager() {
		return renderManager->getFontRenderer();
}
