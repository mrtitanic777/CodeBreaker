#include "stdafx.h"
// Auto-generated from RenderTNTPrimed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderTNTPrimed.h"
#include "Common.h"
#include "Entity.h"
#include "RenderBlocks.h"
#include "Block.h"
#include "EntityTNT.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

RenderTNTPrimed::RenderTNTPrimed()
	: m_blockRenderer(new RenderBlocks())
{
		shadowSize = 0.5F;
}

RenderTNTPrimed::~RenderTNTPrimed() {
	m_blockRenderer = NULL;
}

void RenderTNTPrimed::doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6) {
		EntityTNT var10001 = (EntityTNT)var1;
		var5 = var4;
		var4 = var3;
		var3 = var2;
		EntityTNT var7 = var10001;
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var3, var4, var5);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		if((float)var7.m_fuse - var6 + 1.0F < 10.0F) {
			var3 = 1.0F - ((float)var7.m_fuse - var6 + 1.0F) / 10.0F;
			if(var3 < 0.0F) {
				var3 = 0.0F;
			}

			if(var3 > 1.0F) {
				var3 = 1.0F;
			}

			var3 *= var3;
			var3 *= var3;
			var3 = 1.0F + var3 * 0.3F;
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var3, var3, var3);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		var3 = (1.0F - ((float)var7.m_fuse - var6 + 1.0F) / 100.0F) * 0.8F;
		Textures::load("game:\Content\terrain.png");
		m_blockRenderer->renderBlockOnInventory(Block::s_tnt);
		if(var7.m_fuse / 5 % 2 == 0) {
   g_pd3dDevice->SetTexture(0, NULL);
   // Lighting is shader-based on Xbox 360 (disabled)
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, var3);
			m_blockRenderer->renderBlockOnInventory(Block::s_tnt);
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
   // Lighting is shader-based on Xbox 360 (enabled)
   g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
		}

  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}
