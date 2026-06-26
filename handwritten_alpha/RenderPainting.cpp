#include "stdafx.h"
// Auto-generated from RenderPainting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderPainting.h"
#include "Common.h"
#include "Entity.h"
#include "EntityPainting.h"
#include "Random.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "EnumArt.h"
#include "RenderManager.h"
#include "World.h"

RenderPainting::RenderPainting()
	: rand(new Random())
{
}

RenderPainting::~RenderPainting() {
	rand = NULL;
}

void RenderPainting::a(EntityPainting* var1, double var2, double var4, double var6, float var8, float var9) {
		rand->setSeed(187L);
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var8)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
		g_pd3dDevice->SetTexture(0, Textures::getById(Textures::load("game:\\Content\\art\\kz.png")));
		EnumArt* var10 = var1->m_field_690_b;
		float var11 = 1.0F / 16.0F;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var11, var11, var11);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		func_159_a(var1, var10->field_1623_z, var10->field_1636_A, var10->field_1634_B, var10->field_1632_C);
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void RenderPainting::func_159_a(EntityPainting* var1, int var2, int var3, int var4, int var5) {
		float var6 = (float)(-var2) / 2.0F;
		float var7 = (float)(-var3) / 2.0F;
		float var8 = -0.5F;
		float var9 = 0.5F;

		for(int var10 = 0; var10 < var2 / 16; ++var10) {
			for(int var11 = 0; var11 < var3 / 16; ++var11) {
				float var12 = var6 + (float)((var10 + 1) * 16);
				float var13 = var6 + (float)(var10 * 16);
				float var14 = var7 + (float)((var11 + 1) * 16);
				float var15 = var7 + (float)(var11 * 16);
				func_160_a(var1, (var12 + var13) / 2.0F, (var14 + var15) / 2.0F);
				float var16 = (float)(var4 + var2 - var10 * 16) / 256.0F;
				float var17 = (float)(var4 + var2 - (var10 + 1) * 16) / 256.0F;
				float var18 = (float)(var5 + var3 - var11 * 16) / 256.0F;
				float var19 = (float)(var5 + var3 - (var11 + 1) * 16) / 256.0F;
				float var20 = 12.0F / 16.0F;
				float var21 = 13.0F / 16.0F;
				float var22 = 0.0F;
				float var23 = 1.0F / 16.0F;
				float var24 = 12.0F / 16.0F;
				float var25 = 13.0F / 16.0F;
				float var26 = 0.001953125F;
				float var27 = 0.001953125F;
				float var28 = 385.0F / 512.0F;
				float var29 = 385.0F / 512.0F;
				float var30 = 0.0F;
				float var31 = 1.0F / 16.0F;
				Tesselator& var32 = Tesselator::getInstance();
				var32.init();
				var32.normal(0.0F, 0.0F, -1.0F);
				var32.vertexUV((float)var12, (float)var15, (float)var8, (float)var17, (float)var18);
				var32.vertexUV((float)var13, (float)var15, (float)var8, (float)var16, (float)var18);
				var32.vertexUV((float)var13, (float)var14, (float)var8, (float)var16, (float)var19);
				var32.vertexUV((float)var12, (float)var14, (float)var8, (float)var17, (float)var19);
				var32.normal(0.0F, 0.0F, 1.0F);
				var32.vertexUV((float)var12, (float)var14, (float)var9, (float)var20, (float)var22);
				var32.vertexUV((float)var13, (float)var14, (float)var9, (float)var21, (float)var22);
				var32.vertexUV((float)var13, (float)var15, (float)var9, (float)var21, (float)var23);
				var32.vertexUV((float)var12, (float)var15, (float)var9, (float)var20, (float)var23);
				var32.normal(0.0F, -1.0F, 0.0F);
				var32.vertexUV((float)var12, (float)var14, (float)var8, (float)var24, (float)var26);
				var32.vertexUV((float)var13, (float)var14, (float)var8, (float)var25, (float)var26);
				var32.vertexUV((float)var13, (float)var14, (float)var9, (float)var25, (float)var27);
				var32.vertexUV((float)var12, (float)var14, (float)var9, (float)var24, (float)var27);
				var32.normal(0.0F, 1.0F, 0.0F);
				var32.vertexUV((float)var12, (float)var15, (float)var9, (float)var24, (float)var26);
				var32.vertexUV((float)var13, (float)var15, (float)var9, (float)var25, (float)var26);
				var32.vertexUV((float)var13, (float)var15, (float)var8, (float)var25, (float)var27);
				var32.vertexUV((float)var12, (float)var15, (float)var8, (float)var24, (float)var27);
				var32.normal(-1.0F, 0.0F, 0.0F);
				var32.vertexUV((float)var12, (float)var14, (float)var9, (float)var29, (float)var30);
				var32.vertexUV((float)var12, (float)var15, (float)var9, (float)var29, (float)var31);
				var32.vertexUV((float)var12, (float)var15, (float)var8, (float)var28, (float)var31);
				var32.vertexUV((float)var12, (float)var14, (float)var8, (float)var28, (float)var30);
				var32.normal(1.0F, 0.0F, 0.0F);
				var32.vertexUV((float)var13, (float)var14, (float)var8, (float)var29, (float)var30);
				var32.vertexUV((float)var13, (float)var15, (float)var8, (float)var29, (float)var31);
				var32.vertexUV((float)var13, (float)var15, (float)var9, (float)var28, (float)var31);
				var32.vertexUV((float)var13, (float)var14, (float)var9, (float)var28, (float)var30);
				var32.flush();
			}
		}

}

void RenderPainting::func_160_a(EntityPainting* var1, float var2, float var3) {
		int var4 = (int)floor(var1->posX);
		int var5 = (int)floor(var1->posY + (double)(var3 / 16.0F));
		int var6 = (int)floor(var1->posZ);
		if(var1->m_field_691_a == 0) {
			var4 = (int)floor(var1->posX + (double)(var2 / 16.0F));
		}

		if(var1->m_field_691_a == 1) {
			var6 = (int)floor(var1->posZ - (double)(var2 / 16.0F));
		}

		if(var1->m_field_691_a == 2) {
			var4 = (int)floor(var1->posX - (double)(var2 / 16.0F));
		}

		if(var1->m_field_691_a == 3) {
			var6 = (int)floor(var1->posZ + (double)(var2 / 16.0F));
		}

		float var7 = renderManager->worldObj->getLightBrightness(var4, var5, var6);
  Tesselator::getInstance().color3f(var7, var7, var7);
}

void RenderPainting::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityPainting*)var1, var2, var4, var6, var8, var9);
}
