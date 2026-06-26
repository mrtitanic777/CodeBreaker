#include "stdafx.h"
// Auto-generated from LoadingScreenRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "LoadingScreenRenderer.h"
#include "Common.h"
#include "InputStream.h"
#include "Minecraft.h"
#include "NBTTagCompound.h"
#include <string>
#include "NBTBase.h"
#include "ScaledResolution.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"

LoadingScreenRenderer::LoadingScreenRenderer(Minecraft* var1)
	: mc(NULL)
	, m_start(0)
{
		m_text = "";
		m_title = "";
		m_start = GetTickCount();
		mc = var1;
}

LoadingScreenRenderer::LoadingScreenRenderer()
	: mc(NULL)
	, m_start(0)
{
}

LoadingScreenRenderer::~LoadingScreenRenderer() {
	mc = NULL;
}

void LoadingScreenRenderer::displayProgressMessage(const std::string& var1) {
		if(!mc->running) {
			throw new MinecraftError();
		} else {
			m_title = var1;
			ScaledResolution var3 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
			int var2 = var3.getScaledWidth();
			int var4 = var3.getScaledHeight();
   g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
   { D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.0D), (float)((double)var2), (float)(0.0D), (float)((double)var4), (float)(100.0D), (float)(300.0D));
     Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, -200.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}
}

void LoadingScreenRenderer::displayLoadingString(const std::string& var1) {
		if(!mc->running) {
			throw new MinecraftError();
		} else {
			m_start = 0L;
			m_text = var1;
			setLoadingProgress(-1);
			m_start = 0L;
		}
}

void LoadingScreenRenderer::setLoadingProgress(int var1) {
		if(!mc->running) {
			throw new MinecraftError();
		} else {
			long var2 = GetTickCount();
			if(var2 - m_start >= 20L) {
				m_start = var2;
				ScaledResolution var8 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
				int var3 = var8.getScaledWidth();
				int var9 = var8.getScaledHeight();
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
    // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
    { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
    { D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.0D), (float)((double)var3), (float)(0.0D), (float)((double)var9), (float)(100.0D), (float)(300.0D));
      Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }
    // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
    { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, -200.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
				Tesselator var4 = Tesselator::getInstance();
				int var5 = mc->Textures::load("game:\Content\dirt.png");
    g_pd3dDevice->SetTexture(0, Textures::getById(var5));
				var4.init();
				var4.colorI(4210752);
				var4.vertexUV(0.0F, (float)var9, 0.0F, 0.0F, (float)var9 / 32.0F);
				var4.vertexUV((float)var3, (float)var9, 0.0F, (float)var3 / 32.0F, (float)var9 / 32.0F);
				var4.vertexUV((float)var3, 0.0F, 0.0F, (float)var3 / 32.0F, 0.0F);
				var4.vertexUV(0.0F, 0.0F, 0.0F, 0.0F, 0.0F);
				var4.flush();
				if(var1 >= 0) {
					var5 = var3 / 2 - 50;
					int var6 = var9 / 2 + 16;
     g_pd3dDevice->SetTexture(0, NULL);
					var4.init();
					var4.colorI(8421504);
					var4.vertex((float)var5, (float)var6, 0.0F);
					var4.vertex((float)var5, (float)(var6 + 2), 0.0F);
					var4.vertex((float)(var5 + 100), (float)(var6 + 2), 0.0F);
					var4.vertex((float)(var5 + 100), (float)var6, 0.0F);
					var4.colorI(8454016);
					var4.vertex((float)var5, (float)var6, 0.0F);
					var4.vertex((float)var5, (float)(var6 + 2), 0.0F);
					var4.vertex((float)(var5 + var1), (float)(var6 + 2), 0.0F);
					var4.vertex((float)(var5 + var1), (float)var6, 0.0F);
					var4.flush();
     g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
				}

				mc->fontRenderer->drawStringWithShadow(m_title, (var3 - mc->fontRenderer->getStringWidth(m_title)) / 2, var9 / 2 - 4 - 16, 16777215);
				mc->fontRenderer->drawStringWithShadow(m_text, (var3 - mc->fontRenderer->getStringWidth(m_text)) / 2, var9 / 2 - 4 + 8, 16777215);
    g_pd3dDevice->EndScene(); g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    { // was try
					Sleep(0);
				} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var7)
				}
			}
		}
}

NBTTagCompound* LoadingScreenRenderer::writeLevelTags(InputStream* var0) {
  // PORT: DataInputStream -> Xbox 360 file I/O  // was: DataInputStream* var4 = new DataInputStream(new GZIPInputStream(var0));

		NBTTagCompound var5;
  { // was try
			NBTBase* var1 = NBTBase::readTag(var4);
			if(!(dynamic_cast<NBTTagCompound*>(var1) != NULL)) {
				DbgPrintF("ERROR [IOException]: %s\n", "Root tag must be m_a named compound tag");
			}

			var5 = (NBTTagCompound*)var1;
  } { // was finally � cleanup code follows
   // Removed Java stream/exception call: var4.close();
		}

		return var5;
}
