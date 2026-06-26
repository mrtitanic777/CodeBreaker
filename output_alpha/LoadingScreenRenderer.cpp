#include "stdafx.h"
// Auto-generated from LoadingScreenRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "LoadingScreenRenderer.h"
#include "Common.h"
#include "Minecraft.h"
#include <string>
#include "ScaledResolution.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"
#include "MinecraftError.h"

LoadingScreenRenderer::LoadingScreenRenderer()
	: m_field_1004_a("")
	, mc(NULL)
	, m_field_1007_c("")
	, m_field_1006_d(GetTickCount())
	, m_field_1005_e(false)
{
}

LoadingScreenRenderer::LoadingScreenRenderer(Minecraft* var1)
	: m_field_1004_a("")
	, mc(NULL)
	, m_field_1007_c("")
	, m_field_1006_d(GetTickCount())
	, m_field_1005_e(false)
{
		mc = var1;
}

LoadingScreenRenderer::~LoadingScreenRenderer() {
	mc = NULL;
}

void LoadingScreenRenderer::func_596_a(const std::string& var1) {
		m_field_1005_e = false;
		func_597_c(var1);
}

void LoadingScreenRenderer::func_594_b(const std::string& var1) {
		m_field_1005_e = true;
		func_597_c(m_field_1007_c);
}

void LoadingScreenRenderer::func_597_c(const std::string& var1) {
		if(!mc->running) {
			if(!m_field_1005_e) {
				// PORT: was 'throw new MinecraftError()' (EH off) -> abort loading
				return;
			}
		} else {
			m_field_1007_c = var1;
			ScaledResolution var2 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
			int var3 = var2.getScaledWidth();
			int var4 = var2.getScaledHeight();
   g_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
   { D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.5), (float)((double)var3 + 0.5), (float)((double)var4 + 0.5), (float)(0.5), (float)(100.0), (float)(300.0));
     Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, 200.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}
}

void LoadingScreenRenderer::displayLoadingString(const std::string& var1) {
		if(!mc->running) {
			if(!m_field_1005_e) {
				// PORT: was 'throw new MinecraftError()' (EH off) -> abort loading
				return;
			}
		} else {
			m_field_1006_d = 0L;
			m_field_1004_a = var1;
			setLoadingProgress(-1);
			m_field_1006_d = 0L;
		}
}

void LoadingScreenRenderer::setLoadingProgress(int var1) {
		if(!mc->running) {
			if(!m_field_1005_e) {
				// PORT: was 'throw new MinecraftError()' (EH off) -> abort loading
				return;
			}
		} else {
			long var2 = GetTickCount();
			if(var2 - m_field_1006_d >= 20L) {
				m_field_1006_d = var2;
				ScaledResolution var4 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
				int var5 = var4.getScaledWidth();
				int var6 = var4.getScaledHeight();
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
    // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
    { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
    { D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.5), (float)((double)var5 + 0.5), (float)((double)var6 + 0.5), (float)(0.5), (float)(100.0), (float)(300.0));
      Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }
    // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
    { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, 200.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
				Tesselator& t = Tesselator::getInstance();
				int var8 = Textures::load("game:\\Content\\gui\\background.png");
    g_pd3dDevice->SetTexture(0, Textures::getById(var8));
				float var9 = 32.0F;
				t.init();
				t.colorI(4210752);
				t.vertexUV(0.0f, (double)var6, 0.0f, 0.0f, (double)((float)var6 / var9));
				t.vertexUV((double)var5, (double)var6, 0.0f, (double)((float)var5 / var9), (double)((float)var6 / var9));
				t.vertexUV((double)var5, 0.0f, 0.0f, (double)((float)var5 / var9), 0.0f);
				t.vertexUV(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				t.flush();
				if(var1 >= 0) {
					unsigned char var10 = 100;
					unsigned char var11 = 2;
					int var12 = var5 / 2 - var10 / 2;
					int var13 = var6 / 2 + 16;
     g_pd3dDevice->SetTexture(0, NULL);
					t.init();
					t.colorI(8421504);
					t.vertex((double)var12, (double)var13, 0.0f);
					t.vertex((double)var12, (double)(var13 + var11), 0.0f);
					t.vertex((double)(var12 + var10), (double)(var13 + var11), 0.0f);
					t.vertex((double)(var12 + var10), (double)var13, 0.0f);
					t.colorI(8454016);
					t.vertex((double)var12, (double)var13, 0.0f);
					t.vertex((double)var12, (double)(var13 + var11), 0.0f);
					t.vertex((double)(var12 + var1), (double)(var13 + var11), 0.0f);
					t.vertex((double)(var12 + var1), (double)var13, 0.0f);
					t.flush();
     g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
				}

				mc->fontRenderer->drawStringWithShadow(m_field_1007_c, (var5 - mc->fontRenderer->getStringWidth(m_field_1007_c)) / 2, var6 / 2 - 4 - 16, 16777215);
				mc->fontRenderer->drawStringWithShadow(m_field_1004_a, (var5 - mc->fontRenderer->getStringWidth(m_field_1004_a)) / 2, var6 / 2 - 4 + 8, 16777215);
    g_pd3dDevice->EndScene(); g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    { // was try
					Sleep(0);
				} if(false) { // was catch(Exception var14)
				}

			}
		}
}
