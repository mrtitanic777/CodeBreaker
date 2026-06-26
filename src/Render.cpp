#include "stdafx.h"
// Auto-generated from Render.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "RenderManager.h"
#include <string>
#include "Block.h"
#include "ModelBiped.h"
#include "RenderBlocks.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "World.h"
#include "BlockFire.h"

Render::Render()
	: renderManager(NULL)
	, shadowSize(0)
	, shadowOpaque(0)
{
		new ModelBiped();
		new RenderBlocks();
		shadowSize = 0.0F;
		shadowOpaque = 1.0F;
}

Render::~Render() {
	renderManager = NULL;
}

void Render::loadTexture(const std::string& var1) {
		RenderEngine* var2 = renderManager->m_renderEngine;
		RenderEngine::bindTexture(var2->getTexture(var1));
}

void Render::loadDownloadableImageTexture(const std::string& var1, const std::string& var2) {
		RenderEngine* var3 = renderManager->m_renderEngine;
		RenderEngine::bindTexture(var3->getTextureForDownloadableImage(var1, var2));
}

void Render::renderOffsetAABB(const AxisAlignedBB& var0) {
  g_pd3dDevice->SetTexture(0, NULL);
		Tesselator var1 = Tesselator::getInstance();
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		var1.init();
		Tesselator::normal(0.0F, 0.0F, -1.0F);
		var1.vertex(var0.minX, var0.maxY, var0.minZ);
		var1.vertex(var0.maxX, var0.maxY, var0.minZ);
		var1.vertex(var0.maxX, var0.minY, var0.minZ);
		var1.vertex(var0.minX, var0.minY, var0.minZ);
		Tesselator::normal(0.0F, 0.0F, 1.0F);
		var1.vertex(var0.minX, var0.minY, var0.maxZ);
		var1.vertex(var0.maxX, var0.minY, var0.maxZ);
		var1.vertex(var0.maxX, var0.maxY, var0.maxZ);
		var1.vertex(var0.minX, var0.maxY, var0.maxZ);
		Tesselator::normal(0.0F, -1.0F, 0.0F);
		var1.vertex(var0.minX, var0.minY, var0.minZ);
		var1.vertex(var0.maxX, var0.minY, var0.minZ);
		var1.vertex(var0.maxX, var0.minY, var0.maxZ);
		var1.vertex(var0.minX, var0.minY, var0.maxZ);
		Tesselator::normal(0.0F, 1.0F, 0.0F);
		var1.vertex(var0.minX, var0.maxY, var0.maxZ);
		var1.vertex(var0.maxX, var0.maxY, var0.maxZ);
		var1.vertex(var0.maxX, var0.maxY, var0.minZ);
		var1.vertex(var0.minX, var0.maxY, var0.minZ);
		Tesselator::normal(-1.0F, 0.0F, 0.0F);
		var1.vertex(var0.minX, var0.minY, var0.maxZ);
		var1.vertex(var0.minX, var0.maxY, var0.maxZ);
		var1.vertex(var0.minX, var0.maxY, var0.minZ);
		var1.vertex(var0.minX, var0.minY, var0.minZ);
		Tesselator::normal(1.0F, 0.0F, 0.0F);
		var1.vertex(var0.maxX, var0.minY, var0.minZ);
		var1.vertex(var0.maxX, var0.maxY, var0.minZ);
		var1.vertex(var0.maxX, var0.maxY, var0.maxZ);
		var1.vertex(var0.maxX, var0.minY, var0.maxZ);
		var1.flush();
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
}

void Render::setRenderManager(RenderManager* var1) {
		renderManager = var1;
}

void Render::renderShadow(Entity* var1, float var2, float var3, float var4, float var5) {
		float var12;
		float var18;
		float var19;
		int var29;
		float var34;
		float var36;
		if(shadowSize > 0.0F) {
			var5 = renderManager->getDistanceToCamera(var2, var3, var4);
			var5 = (1.0F - var5 / 256.0F) * shadowOpaque;
			if(var5 > 0.0F) {
				float var9 = var5;
				float var8 = var4;
				float var7 = var3;
				float var6 = var2;
				Render var27 = this;
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				RenderEngine* var10 = renderManager->m_renderEngine;
				RenderEngine::bindTexture(var10->Textures::load("game:\Content\shadow.png"));
				World* var11 = renderManager->worldObj;
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				var12 = shadowSize;

				for(var29 = (int)(var2 - var12); var29 <= (int)(var6 + var12); ++var29) {
					for(int var13 = (int)(var7 - 2.0F); var13 <= (int)var7; ++var13) {
						for(int var14 = (int)(var8 - var12); var14 <= (int)(var8 + var12); ++var14) {
							int var15 = var11->getBlockId(var29, var13 - 1, var14);
							if(var15 > 0 && var11->getBlockLightValue(var29, var13, var14) > 3) {
								Block var16 = Block::blocksList[var15];
								Tesselator var25 = Tesselator::getInstance();
								var34 = (var9 - (var7 - (float)var13) / 2.0F) * 0.5F * var27.renderManager->worldObj->getLightBrightness(var29, var13, var14);
								if(var34 >= 0.0F) {
         Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, var34);
									var25.init();
									var34 = (float)var29 + var16.minX;
									var18 = (float)var29 + var16.maxX;
									float var20 = (float)var13 + var16.minY;
									float var21 = (float)var14 + var16.minZ;
									var36 = (float)var14 + var16.maxZ;
									float var22 = (var6 - var34) / 2.0F / var12 + 0.5F;
									float var17 = (var6 - var18) / 2.0F / var12 + 0.5F;
									float var23 = (var8 - var21) / 2.0F / var12 + 0.5F;
									var19 = (var8 - var36) / 2.0F / var12 + 0.5F;
									var25.vertexUV(var34, var20, var21, var22, var23);
									var25.vertexUV(var34, var20, var36, var22, var19);
									var25.vertexUV(var18, var20, var36, var17, var19);
									var25.vertexUV(var18, var20, var21, var17, var23);
									var25.flush();
								}
							}
						}
					}
				}

    Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
		}

		if(var1->fire > 0) {
   // Lighting is shader-based on Xbox 360 (disabled)
			int var26 = Block::s_fire->blockIndexInTexture;
			var29 = (var26 & 15) << 4;
			int var30 = var26 & 240;
			var12 = (float)var29 / 256.0F;
			float var31 = ((float)var29 + 15.99F) / 256.0F;
			float var32 = (float)var30 / 256.0F;
			float var33 = ((float)var30 + 15.99F) / 256.0F;
   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var2, var3, var4);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var34 = var1->width * 1.4F;
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var34, var34, var34);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			Textures::load("game:\Content\terrain.png");
			Tesselator var35 = Tesselator::getInstance();
			var36 = 1.0F;
			var18 = 0.0F;
			var19 = var1->height / var1->width;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-this->renderManager->m_playerViewY)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, 0.4F + (float);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			var35.init();

			while(var19 > 0.0F) {
				var35.vertexUV(-0.5F, 0.0F - var18, 0.0F, var12, var33);
				var35.vertexUV(var36 - 0.5F, 0.0F - var18, 0.0F, var31, var33);
				var35.vertexUV(var36 - 0.5F, 1.4F - var18, 0.0F, var31, var32);
				var35.vertexUV(-0.5F, 1.4F - var18, 0.0F, var12, var32);
				--var19;
				--var18;
				var36 *= 0.9F;
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, -0.04F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

			var35.flush();
   Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
   // Lighting is shader-based on Xbox 360 (enabled)
		}

}
