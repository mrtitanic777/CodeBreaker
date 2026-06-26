#include "stdafx.h"
// Auto-generated from RenderItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderItem.h"
#include "Common.h"
#include "Entity.h"
#include "FontRenderer.h"
#include "ItemStack.h"
#include "Random.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "Tesselator.h"
#include <string>
#include "Block.h"
#include "EntityItem.h"
#include "Item.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

RenderItem::RenderItem()
	: m_renderBlocks(new RenderBlocks())
	, m_random(new Random())
{
		shadowSize = 0.15F;
		shadowOpaque = 12.0F / 16.0F;
}

RenderItem::~RenderItem() {
	m_renderBlocks = NULL;
	m_random = NULL;
}

void RenderItem::renderItemIntoGUI(RenderEngine* var1, ItemStack* var2, int var3, int var4) {
		if(var2 != NULL) {
			int var9;
			if(var2->itemID < 256 && Block::blocksList[var2->itemID].getRenderType() == 0) {
				var9 = var2->itemID;
				RenderEngine::bindTexture(var1.Textures::load("game:\Content\terrain.png"));
				Block var8 = Block::blocksList[var9];
    D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(var3 - 2), (float)(var4 + 3), 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 10.0F, 10.0F, 10.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 1.0F, 0.5F, 8.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (210.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (45.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
				m_renderBlocks->renderBlockOnInventory(var8);
    Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			} else {
				if(var2->getItem().getIconIndex() >= 0) {
     // Lighting is shader-based on Xbox 360 (disabled)
					if(var2->itemID < 256) {
						RenderEngine::bindTexture(var1.Textures::load("game:\Content\terrain.png"));
					} else {
						RenderEngine::bindTexture(var1.Textures::load("game:\Content\gui\items.png"));
					}

					int var10000 = var3;
					int var10001 = var4;
					int var10002 = var2->getItem().getIconIndex() % 16 << 4;
					int var10003 = var2->getItem().getIconIndex() / 16 << 4;
					bool var6 = true;
					var6 = true;
					var4 = var10003;
					var3 = var10002;
					var9 = var10001;
					int var7 = var10000;
					Tesselator var5 = Tesselator::getInstance();
					var5.init();
					var5.vertexUV((float)var7, (float)(var9 + 16), 0.0F, (float)var3 * 0.00390625F, (float)(var4 + 16) * 0.00390625F);
					var5.vertexUV((float)(var7 + 16), (float)(var9 + 16), 0.0F, (float)(var3 + 16) * 0.00390625F, (float)(var4 + 16) * 0.00390625F);
					var5.vertexUV((float)(var7 + 16), (float)var9, 0.0F, (float)(var3 + 16) * 0.00390625F, (float)var4 * 0.00390625F);
					var5.vertexUV((float)var7, (float)var9, 0.0F, (float)var3 * 0.00390625F, (float)var4 * 0.00390625F);
					var5.flush();
     // Lighting is shader-based on Xbox 360 (enabled)
				}

			}
		}
}

void RenderItem::renderItemOverlayIntoGUI(FontRenderer* var1, ItemStack* var2, int var3, int var4) {
		if(var2 != NULL) {
			if(var2->stackSize > 1) {
				std::string var5 = "" + var2->stackSize;
    // Lighting is shader-based on Xbox 360 (disabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				var1->drawStringWithShadow(var5, var3 + 19 - 2 - var1->getStringWidth(var5), var4 + 6 + 3, 16777215);
    // Lighting is shader-based on Xbox 360 (enabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			}

			if(var2->itemDamage > 0) {
				int var9 = 13 - var2->itemDamage * 13 / var2->isItemStackDamageable();
				int var7 = 255 - var2->itemDamage * 255 / var2->isItemStackDamageable();
    // Lighting is shader-based on Xbox 360 (disabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    g_pd3dDevice->SetTexture(0, NULL);
				Tesselator var8 = Tesselator::getInstance();
				int var6 = 255 - var7 << 16 | var7 << 8;
				var7 = (255 - var7) / 4 << 16 | 16128;
				renderQuad(var8, var3 + 2, var4 + 13, 13, 2, 0);
				renderQuad(var8, var3 + 2, var4 + 13, 12, 1, var7);
				renderQuad(var8, var3 + 2, var4 + 13, var9, 1, var6);
    g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
    // Lighting is shader-based on Xbox 360 (enabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			}

		}
}

void RenderItem::renderQuad(Tesselator* var0, int var1, int var2, int var3, int var4, int var5) {
		var0->init();
		var0->colorI(var5);
		var0->vertex((float)var1, (float)var2, 0.0F);
		var0->vertex((float)var1, (float)(var2 + var4), 0.0F);
		var0->vertex((float)(var1 + var3), (float)(var2 + var4), 0.0F);
		var0->vertex((float)(var1 + var3), (float)var2, 0.0F);
		var0->flush();
}

void RenderItem::doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6) {
		EntityItem var13 = (EntityItem)var1;
		RenderItem var12 = this;
		m_random->setSeed(187L);
		ItemStack var7 = var13.m_item;
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		float var8 = sinf(((float)var13.m_age + var6) / 10.0F + var13.m_hoverStart) * 0.1F + 0.1F;
		var6 = (((float)var13.m_age + var6) / 20.0F + var13.m_hoverStart) * (180.0F / 3.14159265358979323846f);
		unsigned char var9 = 1;
		if(var13.m_item.stackSize > 1) {
			var9 = 2;
		}

		if(var13.m_item.stackSize > 5) {
			var9 = 3;
		}

		if(var13.m_item.stackSize > 20) {
			var9 = 4;
		}

  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var2, var3 + var8, var4);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  // Normalize is shader-based on Xbox 360 (enabled)
		if(var7.itemID < 256 && Block::blocksList[var7.itemID].getRenderType() == 0) {
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var6)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			Textures::load("game:\Content\terrain.png");
			var2 = 0.25F;
			if(!Block::blocksList[var7.itemID].renderAsNormalBlock() && var7.itemID != Block::s_stairSingle->blockID) {
				var2 = 0.5F;
			}

   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var2, var2, var2);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

			for(int var16 = 0; var16 < var9; ++var16) {
    D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
				if(var16 > 0) {
					var4 = (var12.m_random->nextFloat() * 2.0F - 1.0F) * 0.2F / var2;
					var5 = (var12.m_random->nextFloat() * 2.0F - 1.0F) * 0.2F / var2;
					var6 = (var12.m_random->nextFloat() * 2.0F - 1.0F) * 0.2F / var2;
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var4, var5, var6);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}

				var12.m_renderBlocks.renderBlockOnInventory(Block::blocksList[var7.itemID]);
    Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			}
		} else {
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 0.5F, 0.5F, 0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			int var14 = var7.getItem().getIconIndex();
			if(var7.itemID < 256) {
				Textures::load("game:\Content\terrain.png");
			} else {
				Textures::load("game:\Content\gui\items.png");
			}

			Tesselator var15 = Tesselator::getInstance();
			var4 = (float)(var14 % 16 << 4) / 256.0F;
			var5 = (float)((var14 % 16 << 4) + 16) / 256.0F;
			var6 = (float)(var14 / 16 << 4) / 256.0F;
			var2 = (float)((var14 / 16 << 4) + 16) / 256.0F;

			for(int var17 = 0; var17 < var9; ++var17) {
    D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
				if(var17 > 0) {
					var8 = (var12.m_random->nextFloat() * 2.0F - 1.0F) * 0.3F;
					float var10 = (var12.m_random->nextFloat() * 2.0F - 1.0F) * 0.3F;
					float var11 = (var12.m_random->nextFloat() * 2.0F - 1.0F) * 0.3F;
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var8, var10, var11);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}

    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-var12.renderManager->m_playerViewY)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				var15.init();
				Tesselator::normal(0.0F, 1.0F, 0.0F);
				var15.vertexUV(-0.5F, -0.25F, 0.0F, var4, var2);
				var15.vertexUV(0.5F, -0.25F, 0.0F, var5, var2);
				var15.vertexUV(0.5F, 12.0F / 16.0F, 0.0F, var5, var6);
				var15.vertexUV(-0.5F, 12.0F / 16.0F, 0.0F, var4, var6);
				var15.flush();
    Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			}
		}

  // Normalize is shader-based on Xbox 360 (disabled)
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}
