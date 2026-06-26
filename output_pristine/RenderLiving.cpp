#include "stdafx.h"
// Auto-generated from RenderLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"
#include "Common.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "ModelBase.h"
#include "Shaders.h"
#include "Tesselator.h"

RenderLiving::RenderLiving()
	: m_mainModel(NULL)
	, m_e(NULL)
{
}

RenderLiving::RenderLiving(ModelBase* var1, float var2)
	: m_mainModel(NULL)
	, m_e(NULL)
{
		m_mainModel = var1;
		shadowSize = var2;
}

RenderLiving::~RenderLiving() {
	m_mainModel = NULL;
	m_e = NULL;
}

void RenderLiving::setRenderPassModel(ModelBase* var1) {
		m_e = var1;
}

bool RenderLiving::shouldRenderPass(EntityLiving* var1, int var2) {
		return false;
}

float RenderLiving::getDeathMaxRotation(EntityLiving* var1) {
		return 90.0F;
}

int RenderLiving::getColorMultiplier(EntityLiving* var1, float var2, float var3) {
		return 0;
}

void RenderLiving::preRenderCallback(EntityLiving* var1, float var2) {
	// TODO: implement
}

void RenderLiving::doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6) {
		EntityLiving var10001 = (EntityLiving)var1;
		var6 = var6;
		var5 = var4;
		var4 = var3;
		var3 = var2;
		EntityLiving var16 = var10001;
		RenderLiving var15 = this;
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  { // was try
			float var7 = var16.prevRenderYawOffset + (var16.renderYawOffset - var16.prevRenderYawOffset) * var6;
			float var8 = var16.prevRotationYaw + (var16.rotationYaw - var16.prevRotationYaw) * var6;
			float var9 = var16.prevRotationPitch + (var16.rotationPitch - var16.prevRotationPitch) * var6;
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var3, var4, var5);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var3 = (float)var16.ticksExisted + var6;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (180.0F - var7)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			if(var16.deathTime > 0) {
				var4 = ((float)var16.deathTime + var6 - 1.0F) / 20.0F * 1.6F;
				var4 = sqrtf(var4);
				if(var4 > 1.0F) {
					var4 = 1.0F;
				}

    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (var4 * var15.getDeathMaxRotation(var16))*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, -(1.0F / 16.0F), -(1.0F / 16.0F), 1.0F / 16.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var15.preRenderCallback(var16, var6);
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, -24.0F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   // Normalize is shader-based on Xbox 360 (enabled)
			var4 = var16.m_ae + (var16.m_limbYaw - var16.m_ae) * var6;
			var5 = var16.m_limbSwing - var16.m_limbYaw * (1.0F - var6);
			if(var4 > 1.0F) {
				var4 = 1.0F;
			}

			var15.loadDownloadableImageTexture(var16.skinUrl, var16.getTexture());
   g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			var15.m_mainModel.render(var5, var4, var3, var8 - var7, var9, 1.0F);

			for(int var10 = 0; var10 < 4; ++var10) {
				if(var15.shouldRenderPass(var16, var10)) {
					var15.m_e.render(var5, var4, var3, var8 - var7, var9, 1.0F);
     g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
     g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				}
			}

			float var18 = var16.getEntityBrightness(var6);
			int var17 = var15.getColorMultiplier(var16, var18, var6);
			if(var17 >>> 24 > 0 || var16.hurtTime > 0 || var16.deathTime > 0) {
    g_pd3dDevice->SetTexture(0, NULL);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
				if(var16.hurtTime > 0 || var16.deathTime > 0) {
     Tesselator::getInstance().color4f(var18, 0.0F, 0.0F, 0.4F);
					var15.m_mainModel.render(var5, var4, var3, var8 - var7, var9, 1.0F);

					for(int var11 = 0; var11 < 4; ++var11) {
						if(var15.shouldRenderPass(var16, var11)) {
       Tesselator::getInstance().color4f(var18, 0.0F, 0.0F, 0.4F);
							var15.m_e.render(var5, var4, var3, var8 - var7, var9, 1.0F);
						}
					}
				}

				if(var17 >>> 24 > 0) {
					float var19 = (float)(var17 >> 16 & 255) / 255.0F;
					var18 = (float)(var17 >> 8 & 255) / 255.0F;
					float var12 = (float)(var17 & 255) / 255.0F;
					var6 = (float)(var17 >>> 24) / 255.0F;
     Tesselator::getInstance().color4f(var19, var18, var12, var6);
					var15.m_mainModel.render(var5, var4, var3, var8 - var7, var9, 1.0F);

					for(int var13 = 0; var13 < 4; ++var13) {
						if(var15.shouldRenderPass(var16, var13)) {
       Tesselator::getInstance().color4f(var19, var18, var12, var6);
							var15.m_e.render(var5, var4, var3, var8 - var7, var9, 1.0F);
						}
					}
				}

    g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
			}

   // Normalize is shader-based on Xbox 360 (disabled)
		} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var14)
   // Removed Java stream/exception call: var14.printStackTrace();
		}

  g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}
