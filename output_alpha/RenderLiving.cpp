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
	: m_e(NULL)
	, m_field_6332_f(NULL)
{
}

RenderLiving::RenderLiving(ModelBase* var1, float var2)
	: m_e(NULL)
	, m_field_6332_f(NULL)
{
		m_e = var1;
		m_field_9246_c = var2;
}

RenderLiving::~RenderLiving() {
	m_e = NULL;
	m_field_6332_f = NULL;
}

void RenderLiving::func_4013_a(ModelBase* var1) {
		m_field_6332_f = var1;
}

void RenderLiving::a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_e->m_field_1244_k = func_167_c(var1, var9);
		m_e->m_field_1243_l = var1->m_ridingEntity != NULL || var1->m_field_9300_bu;
		if(m_field_6332_f != NULL) {
			m_field_6332_f->m_field_1243_l = m_e->m_field_1243_l;
		}

  { // was try
			float var10 = var1->m_field_734_o + (var1->m_field_735_n - var1->m_field_734_o) * var9;
			float var11 = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var9;
			float var12 = var1->prevRotationPitch + (var1->rotationPitch - var1->prevRotationPitch) * var9;
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			float var13 = func_170_d(var1, var9);
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (180.0F - var10)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			float var14;
			if(var1->deathTime > 0) {
				var14 = ((float)var1->deathTime + var9 - 1.0F) / 20.0F * 1.6F;
				var14 = sqrtf(var14);
				if(var14 > 1.0F) {
					var14 = 1.0F;
				}

    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (var14 * this->func_172_a(var1))*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

			var14 = 1.0F / 16.0F;
   // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, -1.0F, -1.0F, 1.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			func_6330_a(var1, var9);
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, -24.0F * var14 - 0.0078125F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			float var15 = var1->m_field_705_Q + (var1->m_field_704_R - var1->m_field_705_Q) * var9;
			float var16 = var1->m_field_703_S - var1->m_field_704_R * (1.0F - var9);
			if(var15 > 1.0F) {
				var15 = 1.0F;
			}

			func_140_a(var1->skinUrl, var1->getEntityTexture());
   g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			m_e->render(var16, var15, var13, var11 - var10, var12, var14);

			for(int var17 = 0; var17 < 4; ++var17) {
				if(func_166_a(var1, var17)) {
					m_field_6332_f->render(var16, var15, var13, var11 - var10, var12, var14);
     g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
     g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				}
			}

			func_6331_b(var1, var9);
			float var25 = var1->getEntityBrightness(var9);
			int var18 = func_173_a(var1, var25, var9);
			if((var18 >> 24 & 255) > 0 || var1->hurtTime > 0 || var1->deathTime > 0) {
    g_pd3dDevice->SetTexture(0, NULL);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
				if(var1->hurtTime > 0 || var1->deathTime > 0) {
     Tesselator::getInstance().color4f(var25, 0.0F, 0.0F, 0.4F);
					m_e->render(var16, var15, var13, var11 - var10, var12, var14);

					for(int var19 = 0; var19 < 4; ++var19) {
						if(func_166_a(var1, var19)) {
       Tesselator::getInstance().color4f(var25, 0.0F, 0.0F, 0.4F);
							m_field_6332_f->render(var16, var15, var13, var11 - var10, var12, var14);
						}
					}
				}

				if((var18 >> 24 & 255) > 0) {
					float var26 = (float)(var18 >> 16 & 255) / 255.0F;
					float var20 = (float)(var18 >> 8 & 255) / 255.0F;
					float var21 = (float)(var18 & 255) / 255.0F;
					float var22 = (float)(var18 >> 24 & 255) / 255.0F;
     Tesselator::getInstance().color4f(var26, var20, var21, var22);
					m_e->render(var16, var15, var13, var11 - var10, var12, var14);

					for(int var23 = 0; var23 < 4; ++var23) {
						if(func_166_a(var1, var23)) {
       Tesselator::getInstance().color4f(var26, var20, var21, var22);
							m_field_6332_f->render(var16, var15, var13, var11 - var10, var12, var14);
						}
					}
				}

    g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
			}

   // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
		} if(false) { // was catch(Exception var24)
   // Removed Java stream/exception call: var24.printStackTrace();
		}

  g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

float RenderLiving::func_167_c(EntityLiving* var1, float var2) {
		return var1->getSwingProgress(var2);
}

float RenderLiving::func_170_d(EntityLiving* var1, float var2) {
		return (float)var1->m_field_9311_be + var2;
}

void RenderLiving::func_6331_b(EntityLiving* var1, float var2) {
	// TODO: implement
}

bool RenderLiving::func_166_a(EntityLiving* var1, int var2) {
		return false;
}

float RenderLiving::func_172_a(EntityLiving* var1) {
		return 90.0F;
}

int RenderLiving::func_173_a(EntityLiving* var1, float var2, float var3) {
		return 0;
}

void RenderLiving::func_6330_a(EntityLiving* var1, float var2) {
	// TODO: implement
}

void RenderLiving::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityLiving*)var1, var2, var4, var6, var8, var9);
}
