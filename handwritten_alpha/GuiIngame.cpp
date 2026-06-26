#include "stdafx.h"
// Hand-written PORT override for GuiIngame.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Started from the converter output (ConversionTool\output_alpha\GuiIngame.cpp, ~14 errors) and
// fixed to compile against Alpha's GuiIngame.h, using indev's working GuiIngame.cpp as the porting
// guide for the D3D9 matrix / string / item-render patterns. Alpha's HUD is richer than indev
// (armor bar, vignette/portal/pumpkin overlays, record toast, fading chat), so the body follows the
// converted Alpha source, not indev wholesale.
//
// Notable converter fixes:
//   * Color.HSBtoRGB(...) -> static HSBtoRGB() helper below (Java java.awt.Color algorithm, packed
//     0x00RRGGBB) -- there is no Color class in the port.
//   * bare 'currentScreen' -> 'mc->currentScreen' (it's a Minecraft member, not a local/static).
//   * Block::s_portal is a BlockPortal* (incomplete here) -> #include "BlockPortal.h" so
//     ->blockIndexInTexture (inherited from Block) resolves.
//   * func_554_a: '_savedMatrix' was declared inside the 'if(var6 > 0)' block but read again in a
//     second 'if(var6 > 0)' below -> hoisted to function scope.
//   * addChatMessage: 'var1' is a const std::string& but Java reassigned it in the wrap loop ->
//     work on a local copy. m_chatMessageList is std::vector<ChatLine*>; Java's list.add(0, line)
//     (insert-at-front) -> insert(begin(), new ChatLine(...)). Trim oldest from the BACK.
//   * Matrices keep the converter's post-multiply (current*new) here -- this HUD code was already
//     authored that way in the converted source and the indev reference uses the same orientation
//     for the rotate-180 item-lighting setup; the pop-animation is PRE-multiplied per indev.

#include "GuiIngame.h"
#include "Common.h"
#include "ChatLine.h"
#include "RenderGlobal.h"
#include "PlayerController.h"
#include "Minecraft.h"
#include "Random.h"
#include "RenderItem.h"
#include <string>
#include "FontRenderer.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "Material.h"
#include "RenderHelper.h"
#include "ScaledResolution.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "Block.h"
#include "BlockPortal.h"
#include "EffectRenderer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "GameSettings.h"
#include "GuiChat.h"
#include "RenderEngine.h"
#include "World.h"
#include <cmath>

RenderItem* GuiIngame::s_itemRenderer = new RenderItem();

// PORT: java.awt.Color.HSBtoRGB(h, s, b) -> packed 0x00RRGGBB. Used for the record-toast colour.
static int HSBtoRGB(float hue, float saturation, float brightness) {
	int r = 0, g = 0, b = 0;
	if(saturation == 0.0F) {
		r = g = b = (int)(brightness * 255.0F + 0.5F);
	} else {
		float h = (hue - (float)floorf(hue)) * 6.0F;
		float f = h - (float)floorf(h);
		float p = brightness * (1.0F - saturation);
		float q = brightness * (1.0F - saturation * f);
		float t = brightness * (1.0F - saturation * (1.0F - f));
		switch((int)h) {
		case 0:  r = (int)(brightness * 255.0F + 0.5F); g = (int)(t * 255.0F + 0.5F); b = (int)(p * 255.0F + 0.5F); break;
		case 1:  r = (int)(q * 255.0F + 0.5F); g = (int)(brightness * 255.0F + 0.5F); b = (int)(p * 255.0F + 0.5F); break;
		case 2:  r = (int)(p * 255.0F + 0.5F); g = (int)(brightness * 255.0F + 0.5F); b = (int)(t * 255.0F + 0.5F); break;
		case 3:  r = (int)(p * 255.0F + 0.5F); g = (int)(q * 255.0F + 0.5F); b = (int)(brightness * 255.0F + 0.5F); break;
		case 4:  r = (int)(t * 255.0F + 0.5F); g = (int)(p * 255.0F + 0.5F); b = (int)(brightness * 255.0F + 0.5F); break;
		default: r = (int)(brightness * 255.0F + 0.5F); g = (int)(p * 255.0F + 0.5F); b = (int)(q * 255.0F + 0.5F); break;
		}
	}
	return (r << 16) | (g << 8) | b;
}

GuiIngame::GuiIngame()
	: m_field_933_a("")
	, m_field_6446_b(0)
	, rand(new Random())
	, mc(NULL)
	, m_updateCounter(0)
	, m_field_9420_i("")
	, m_field_9419_j(0)
	, m_field_931_c(1.0f)
{
}

GuiIngame::GuiIngame(Minecraft* var1)
	: m_field_933_a("")
	, m_field_6446_b(0)
	, rand(new Random())
	, mc(NULL)
	, m_updateCounter(0)
	, m_field_9420_i("")
	, m_field_9419_j(0)
	, m_field_931_c(1.0f)
{
		mc = var1;
}

GuiIngame::~GuiIngame() {
	rand = NULL;
	mc = NULL;
}

void GuiIngame::renderGameOverlay(float var1, bool var2, int var3, int var4) {
		ScaledResolution var5 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
		int var6 = var5.getScaledWidth();
		int var7 = var5.getScaledHeight();
		FontRenderer* var8 = mc->fontRenderer;
		mc->m_field_9243_r->func_905_b();
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		if(mc->m_gameSettings->m_fancyGraphics) {
			func_4064_a(mc->thePlayer->getEntityBrightness(var1), var6, var7);
		}

		ItemStack* var9 = mc->thePlayer->m_inventory->armorItemInSlot(3);
		if(!mc->m_gameSettings->m_thirdPersonView && var9 != NULL && var9->itemID == Block::s_pumpkin->blockID) {
			func_4063_a(var6, var7);
		}

		float var10 = mc->thePlayer->m_field_4133_d + (mc->thePlayer->m_field_4134_c - mc->thePlayer->m_field_4133_d) * var1;
		if(var10 > 0.0F) {
			func_4065_b(var10, var6, var7);
		}

  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/gui/gui.png")));
		InventoryPlayer* var11 = mc->thePlayer->m_inventory;
		zLevel = -90.0F;
		drawTexturedModalRect(var6 / 2 - 91, var7 - 22, 0, 0, 182, 22);
		drawTexturedModalRect(var6 / 2 - 91 - 1 + var11->m_currentItem * 20, var7 - 22 - 1, 0, 22, 24, 22);
  g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/gui/icons.png")));
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		drawTexturedModalRect(var6 / 2 - 7, var7 / 2 - 7, 0, 0, 16, 16);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		bool var12 = mc->thePlayer->m_field_9306_bj / 3 % 2 == 1;
		if(mc->thePlayer->m_field_9306_bj < 10) {
			var12 = false;
		}

		int var13 = mc->thePlayer->health;
		int var14 = mc->thePlayer->m_field_9335_K;
		rand->setSeed((long)(m_updateCounter * 312871));
		int var15;
		int var16;
		int var17;
		if(mc->m_field_6327_b->func_6469_d()) {
			var15 = mc->thePlayer->getPlayerArmorValue();

			int var18;
			for(var16 = 0; var16 < 10; ++var16) {
				var17 = var7 - 32;
				if(var15 > 0) {
					var18 = var6 / 2 + 91 - var16 * 8 - 9;
					if(var16 * 2 + 1 < var15) {
						drawTexturedModalRect(var18, var17, 34, 9, 9, 9);
					}

					if(var16 * 2 + 1 == var15) {
						drawTexturedModalRect(var18, var17, 25, 9, 9, 9);
					}

					if(var16 * 2 + 1 > var15) {
						drawTexturedModalRect(var18, var17, 16, 9, 9, 9);
					}
				}

				unsigned char var27 = 0;
				if(var12) {
					var27 = 1;
				}

				int var19 = var6 / 2 - 91 + var16 * 8;
				if(var13 <= 4) {
					var17 += rand->nextInt(2);
				}

				drawTexturedModalRect(var19, var17, 16 + var27 * 9, 0, 9, 9);
				if(var12) {
					if(var16 * 2 + 1 < var14) {
						drawTexturedModalRect(var19, var17, 70, 0, 9, 9);
					}

					if(var16 * 2 + 1 == var14) {
						drawTexturedModalRect(var19, var17, 79, 0, 9, 9);
					}
				}

				if(var16 * 2 + 1 < var13) {
					drawTexturedModalRect(var19, var17, 52, 0, 9, 9);
				}

				if(var16 * 2 + 1 == var13) {
					drawTexturedModalRect(var19, var17, 61, 0, 9, 9);
				}
			}

			if(mc->thePlayer->isInsideOfMaterial(Material::s_water)) {
				var16 = (int)ceilf((float)((double)(mc->thePlayer->air - 2) * 10.0f / 300.0f));
				var17 = (int)ceilf((float)((double)mc->thePlayer->air * 10.0f / 300.0f)) - var16;

				for(var18 = 0; var18 < var16 + var17; ++var18) {
					if(var18 < var16) {
						drawTexturedModalRect(var6 / 2 - 91 + var18 * 8, var7 - 32 - 9, 16, 18, 9, 9);
					} else {
						drawTexturedModalRect(var6 / 2 - 91 + var18 * 8, var7 - 32 - 9, 25, 18, 9, 9);
					}
				}
			}
		}

  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  // PORT: GL_RESCALE_NORMAL -> normalize is shader-based on Xbox 360.
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _r; D3DXVECTOR3 _axis(1.0F,0.0F,0.0F); D3DXMatrixRotationAxis(&_r, &_axis, (180.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		RenderHelper::enableStandardItemLighting();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();

		for(var15 = 0; var15 < 9; ++var15) {
			var16 = var6 / 2 - 90 + var15 * 20 + 2;
			var17 = var7 - 16 - 3;
			func_554_a(var15, var16, var17, var1);
		}

		RenderHelper::disableStandardItemLighting();
  // PORT: GL_RESCALE_NORMAL -> normalize is shader-based on Xbox 360.
		std::string var23;
  // PORT: F3 debug overlay (XInput has no F3; Runtime memory APIs unportable) -> dropped.
  // Java drew the version string + position + memory stats here when KEY_F3 was held.

		if(m_field_9419_j > 0) {
			float var25 = (float)m_field_9419_j - var1;
			var16 = (int)(var25 * 256.0F / 20.0F);
			if(var16 > 255) {
				var16 = 255;
			}

			if(var16 > 0) {
    D3DXMATRIX _savedMatrix2 = Shaders::getWorldMatrix();
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(var6 / 2), (float)(var7 - 48), 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				var17 = HSBtoRGB(var25 / 50.0F, 0.7F, 0.6F) & 16777215;
				var8->drawString(m_field_9420_i, -var8->getStringWidth(m_field_9420_i) / 2, -4, var17 + (var16 << 24));
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    Shaders::setWorldMatrix(_savedMatrix2); Shaders::updateShaderConstants();
			}
		}

		int var26 = 10;
		bool var28 = false;
		if(dynamic_cast<GuiChat*>(mc->currentScreen) != NULL) {
			var26 = 20;
			var28 = true;
		}

  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
  _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, (float)(var7 - 48), 0.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

		for(size_t var17b = 0; var17b < m_chatMessageList.size() && var17b < (size_t)var26; ++var17b) {
			if(m_chatMessageList[var17b]->m_updateCounter < 200 || var28) {
				double var31 = (double)m_chatMessageList[var17b]->m_updateCounter / 200.0f;
				var31 = 1.0f - var31;
				var31 *= 10.0f;
				if(var31 < 0.0f) {
					var31 = 0.0f;
				}

				if(var31 > 1.0f) {
					var31 = 1.0f;
				}

				var31 *= var31;
				int var20 = (int)(255.0f * var31);
				if(var28) {
					var20 = 255;
				}

				if(var20 > 0) {
					int var32 = 2;
					int var22 = -(int)var17b * 9;
					var23 = m_chatMessageList[var17b]->m_message;
					drawRect(var32, var22 - 1, var32 + 320, var22 + 8, var20 / 2 << 24);
     g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					var8->drawStringWithShadow(var23, var32, var22, 16777215 + (var20 << 24));
				}
			}
		}

  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void GuiIngame::func_4063_a(int var1, int var2) {
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
  g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("%blur%/misc/pumpkinblur.png")));
		Tesselator& var3 = Tesselator::getInstance();
		var3.init();
		var3.vertexUV(0.0f, (double)var2, -90.0f, 0.0f, 1.0f);
		var3.vertexUV((double)var1, (double)var2, -90.0f, 1.0f, 1.0f);
		var3.vertexUV((double)var1, 0.0f, -90.0f, 1.0f, 0.0f);
		var3.vertexUV(0.0f, 0.0f, -90.0f, 0.0f, 0.0f);
		var3.flush();
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
}

void GuiIngame::func_4064_a(float var1, int var2, int var3) {
		var1 = 1.0F - var1;
		if(var1 < 0.0F) {
			var1 = 0.0F;
		}

		if(var1 > 1.0F) {
			var1 = 1.0F;
		}

		m_field_931_c = (float)((double)m_field_931_c + (double)(var1 - m_field_931_c) * 0.01f);
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  Tesselator::getInstance().color4f(this->m_field_931_c, this->m_field_931_c, this->m_field_931_c, 1.0F);
  g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("%blur%/misc/vignette.png")));
		Tesselator& var4 = Tesselator::getInstance();
		var4.init();
		var4.vertexUV(0.0f, (double)var3, -90.0f, 0.0f, 1.0f);
		var4.vertexUV((double)var2, (double)var3, -90.0f, 1.0f, 1.0f);
		var4.vertexUV((double)var2, 0.0f, -90.0f, 1.0f, 0.0f);
		var4.vertexUV(0.0f, 0.0f, -90.0f, 0.0f, 0.0f);
		var4.flush();
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void GuiIngame::func_4065_b(float var1, int var2, int var3) {
		var1 *= var1;
		var1 *= var1;
		var1 = var1 * 0.8F + 0.2F;
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, var1);
  g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/terrain.png")));
		float var4 = (float)(Block::s_portal->blockIndexInTexture % 16) / 16.0F;
		float var5 = (float)(Block::s_portal->blockIndexInTexture / 16) / 16.0F;
		float var6 = (float)(Block::s_portal->blockIndexInTexture % 16 + 1) / 16.0F;
		float var7 = (float)(Block::s_portal->blockIndexInTexture / 16 + 1) / 16.0F;
		Tesselator& var8 = Tesselator::getInstance();
		var8.init();
		var8.vertexUV(0.0f, (double)var3, -90.0f, (double)var4, (double)var7);
		var8.vertexUV((double)var2, (double)var3, -90.0f, (double)var6, (double)var7);
		var8.vertexUV((double)var2, 0.0f, -90.0f, (double)var6, (double)var5);
		var8.vertexUV(0.0f, 0.0f, -90.0f, (double)var4, (double)var5);
		var8.flush();
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
}

void GuiIngame::func_554_a(int var1, int var2, int var3, float var4) {
		ItemStack* var5 = mc->thePlayer->m_inventory->m_mainInventory[var1];
		if(var5 != NULL) {
			float var6 = (float)var5->m_animationsToGo - var4;
   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			if(var6 > 0.0F) {
				float var7 = 1.0F + var6 / 5.0F;
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(var2 + 8), (float)(var3 + 12), 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F / var7, (var7 + 1.0F) / 2.0F, 1.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(-(var2 + 8)), (float)(-(var3 + 12)), 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

			s_itemRenderer->renderItemIntoGUI(mc->fontRenderer, mc->m_renderEngine, var5, var2, var3);
			if(var6 > 0.0F) {
    Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			}

			s_itemRenderer->renderItemOverlayIntoGUI(mc->fontRenderer, mc->m_renderEngine, var5, var2, var3);
		}
}

void GuiIngame::func_555_a() {
		if(m_field_9419_j > 0) {
			--m_field_9419_j;
		}

		++m_updateCounter;

		for(size_t var1 = 0; var1 < m_chatMessageList.size(); ++var1) {
			++m_chatMessageList[var1]->m_updateCounter;
		}

}

void GuiIngame::addChatMessage(const std::string& var1) {
		std::string var1c = var1;
		while(mc->fontRenderer->getStringWidth(var1c) > 320) {
			size_t var2;
			for(var2 = 1; var2 < var1c.length() && mc->fontRenderer->getStringWidth(var1c.substr(0, var2 + 1)) <= 320; ++var2) {
			}

			addChatMessage(var1c.substr(0, var2));
			var1c = var1c.substr(var2);
		}

		m_chatMessageList.insert(m_chatMessageList.begin(), new ChatLine(var1c));

		while(m_chatMessageList.size() > 50) {
			delete m_chatMessageList.back();
			m_chatMessageList.pop_back();
		}

}

void GuiIngame::func_553_b(const std::string& var1) {
		m_field_9420_i = std::string("Now playing: ") + toStr(var1);
		m_field_9419_j = 60;
}
