#include "stdafx.h"
// Auto-generated from RenderTNTPrimed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderTNTPrimed.h"
#include "Common.h"
#include "Entity.h"
#include "EntityTNTPrimed.h"
#include "RenderBlocks.h"
#include "Block.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

RenderTNTPrimed::RenderTNTPrimed()
	: m_field_196_d(new RenderBlocks())
{
		m_field_9246_c = 0.5F;
}

RenderTNTPrimed::~RenderTNTPrimed() {
	m_field_196_d = NULL;
}

void RenderTNTPrimed::a(EntityTNTPrimed* var1, double var2, double var4, double var6, float var8, float var9) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var10;
		if((float)var1->m_fuse - var9 + 1.0F < 10.0F) {
			var10 = 1.0F - ((float)var1->m_fuse - var9 + 1.0F) / 10.0F;
			if(var10 < 0.0F) {
				var10 = 0.0F;
			}

			if(var10 > 1.0F) {
				var10 = 1.0F;
			}

			var10 *= var10;
			var10 *= var10;
			float var11 = 1.0F + var10 * 0.3F;
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var11, var11, var11);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		var10 = (1.0F - ((float)var1->m_fuse - var9 + 1.0F) / 100.0F) * 0.8F;
		Textures::load("game:\\Content\\terrain.png");
		m_field_196_d->func_1227_a(Block::s_tnt);
		if(var1->m_fuse / 5 % 2 == 0) {
   g_pd3dDevice->SetTexture(0, NULL);
   // Lighting is shader-based on Xbox 360 (disabled)
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, var10);
			m_field_196_d->func_1227_a(Block::s_tnt);
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
   // Lighting is shader-based on Xbox 360 (enabled)
   g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
		}

  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void RenderTNTPrimed::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityTNTPrimed*)var1, var2, var4, var6, var8, var9);
}
