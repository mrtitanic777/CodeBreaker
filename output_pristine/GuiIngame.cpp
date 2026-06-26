#include "stdafx.h"
// Auto-generated from GuiIngame.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiIngame.h"
#include "Common.h"
#include "Minecraft.h"
#include "Random.h"
#include "RenderItem.h"
#include <string>
#include "FontRenderer.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "RenderHelper.h"
#include "ScaledResolution.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

RenderItem* GuiIngame::s_itemRenderer = new RenderItem();

GuiIngame::GuiIngame()
	: rand(new Random())
	, mc(NULL)
	, m_updateCounter(0)
{
}

GuiIngame::GuiIngame(Minecraft* var1)
	: rand(new Random())
	, mc(NULL)
	, m_updateCounter(0)
{
		mc = var1;
}

GuiIngame::~GuiIngame() {
	rand = NULL;
	mc = NULL;
}

void GuiIngame::renderGameOverlay(float var1) {
		ScaledResolution var2 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
		int var3 = var2.getScaledWidth();
		int var19 = var2.getScaledHeight();
		FontRenderer var4 = mc->fontRenderer;
		mc->m_entityRenderer.setupOverlayRendering();
  g_pd3dDevice->SetTexture(0, Textures::getById(this.mc->m_renderEngine->getTexture("/gui/gui.png")));
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		InventoryPlayer var5 = mc->thePlayer->m_inventory;
		zLevel = -90.0F;
		drawTexturedModalRect(var3 / 2 - 91, var19 - 22, 0, 0, 182, 22);
		drawTexturedModalRect(var3 / 2 - 91 - 1 + var5.m_currentItem * 20, var19 - 22 - 1, 0, 22, 24, 22);
  g_pd3dDevice->SetTexture(0, Textures::getById(this.mc->m_renderEngine->getTexture("/gui/icons.png")));
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		drawTexturedModalRect(var3 / 2 - 7, var19 / 2 - 7, 0, 0, 16, 16);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		bool var20 = mc->thePlayer->heartsLife / 3 % 2 == 1;
		if(mc->thePlayer->heartsLife < 10) {
			var20 = false;
		}

		int var6 = mc->thePlayer->health;
		int var7 = mc->thePlayer->prevHealth;
		rand->setSeed((long)(m_updateCounter * 312871));
		int var10;
		size_t var12;
		if(mc->m_playerController.shouldDrawHUD()) {
			for(var10 = 0; var10 < 10; ++var10) {
				unsigned char var11 = 0;
				if(var20) {
					var11 = 1;
				}

				var12 = var3 / 2 - 91 + (var10 << 3);
				int var13 = var19 - 32;
				if(var6 <= 4) {
					var13 += rand->nextInt(2);
				}

				drawTexturedModalRect(var12, var13, 16 + var11 * 9, 0, 9, 9);
				if(var20) {
					if((var10 << 1) + 1 < var7) {
						drawTexturedModalRect(var12, var13, 70, 0, 9, 9);
					}

					if((var10 << 1) + 1 == var7) {
						drawTexturedModalRect(var12, var13, 79, 0, 9, 9);
					}
				}

				if((var10 << 1) + 1 < var6) {
					drawTexturedModalRect(var12, var13, 52, 0, 9, 9);
				}

				if((var10 << 1) + 1 == var6) {
					drawTexturedModalRect(var12, var13, 61, 0, 9, 9);
				}
			}

			if(mc->thePlayer->isInsideOfMaterial()) {
				var10 = (int)ceilf((double)(mc->thePlayer->air - 2) * 10.0f / 300.0f);
				int var25 = (int)ceilf((double)mc->thePlayer->air * 10.0f / 300.0f) - var10;

				for(var12 = 0; var12 < var10 + var25; ++var12) {
					if(var12 < var10) {
						drawTexturedModalRect(var3 / 2 - 91 + (var12 << 3), var19 - 32 - 9, 16, 18, 9, 9);
					} else {
						drawTexturedModalRect(var3 / 2 - 91 + (var12 << 3), var19 - 32 - 9, 25, 18, 9, 9);
					}
				}
			}
		}

  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  // Normalize is shader-based on Xbox 360 (enabled)
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (180.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		RenderHelper::enableStandardItemLighting();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();

		for(var10 = 0; var10 < 9; ++var10) {
			int var26 = (var3 / 2 - 90 + var10 * 20 + 2);
			int var23 = var19 - 16 - 3;
			ItemStack var21 = mc->thePlayer->m_inventory.m_mainInventory[var10];
			if(var21 != NULL) {
				float var9 = (float)var21.m_animationsToGo - var1;
				if(var9 > 0.0F) {
     D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
					float var27 = 1.0F + var9 / 5.0F;
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(var26 + 8), (float)(var23 + 12), 0.0F);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
     { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F / var27, (var27 + 1.0F) / 2.0F, 1.0F);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(-(var26 + 8)), (float)(-(var23 + 12)), 0.0F);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}

				s_itemRenderer->renderItemIntoGUI(mc->m_renderEngine, var21, var26, var23);
				if(var9 > 0.0F) {
     Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
				}

				s_itemRenderer->renderItemOverlayIntoGUI(mc->fontRenderer, var21, var26, var23);
			}
		}

		RenderHelper::disableStandardItemLighting();
  // Normalize is shader-based on Xbox 360 (disabled)
		if(mc->m_options.m_showFPS) {
			var4.drawStringWithShadow("Minecraft Indev (" + mc->m_debug + ")", 2, 2, 16777215);
			Minecraft var22 = mc;
			var4.drawStringWithShadow(var22.m_renderGlobal.getDebugInfoRenders(), 2, 12, 16777215);
			var22 = mc;
			var4.drawStringWithShadow(var22.m_renderGlobal.getDebugInfoEntities(), 2, 22, 16777215);
			var22 = mc;
			var4.drawStringWithShadow("P: " + var22.m_effectRenderer.getStatistics() + ". T: " + var22.theWorld->debugSkylightUpdates(), 2, 32, 16777215);
			long var24 = 64 * 1024 * 1024 /* 64MB */;
			long var27 = 64 * 1024 * 1024 /* 64MB */;
			long var14 = 32 * 1024 * 1024 /* 32MB */;
			long var16 = var24 - var14;
			std::string var18 = "Free memory: " + var16 * 100L / var24 + "% of " + var24 / 1024L / 1024L + "MB";
			drawString(var4, var18, var3 - var4.getStringWidth(var18) - 2, 2, 14737632);
			var18 = "Allocated memory: " + var27 * 100L / var24 + "% (" + var27 / 1024L / 1024L + "MB)";
			drawString(var4, var18, var3 - var4.getStringWidth(var18) - 2, 12, 14737632);
		} else {
			var4.drawStringWithShadow("Minecraft Indev", 2, 2, 16777215);
		}

		for(var12 = 0; var12 < m_chatMessageList.size() && var12 < 10; ++var12) {
			if(m_chatMessageList[var12]->m_updateCounter < 200) {
				m_chatMessageList[var12];
				var4.drawStringWithShadow((std::string)NULL, 2, var19 - 8 - var12 * 9 - 20, 16777215);
			}
		}

}

void GuiIngame::addChatMessage() {
		++m_updateCounter;

		for(size_t var1 = 0; var1 < m_chatMessageList.size(); ++var1) {
			++m_chatMessageList[var1]->m_updateCounter;
		}

}
