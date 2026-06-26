#include "stdafx.h"
// Auto-generated from RenderItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderItem.h"
#include "Common.h"
#include "Entity.h"
#include "EntityItem.h"
#include "FontRenderer.h"
#include "ItemStack.h"
#include "Random.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "Tesselator.h"
#include <string>
#include "Block.h"
#include "Item.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "RenderManager.h"

RenderItem::RenderItem()
	: m_renderBlocks(new RenderBlocks())
	, m_random(new Random())
{
		m_field_9246_c = 0.15F;
		m_field_194_c = 12.0F / 16.0F;
}

RenderItem::~RenderItem() {
	m_renderBlocks = NULL;
	m_random = NULL;
}

void RenderItem::a(EntityItem* var1, double var2, double var4, double var6, float var8, float var9) {
		m_random->setSeed(187L);
		ItemStack* var10 = var1->m_item;
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		float var11 = sinf(((float)var1->m_age + var9) / 10.0F + var1->m_field_804_d) * 0.1F + 0.1F;
		float var12 = (((float)var1->m_age + var9) / 20.0F + var1->m_field_804_d) * (180.0F / 3.14159265358979323846f);
		unsigned char var13 = 1;
		if(var1->m_item->stackSize > 1) {
			var13 = 2;
		}

		if(var1->m_item->stackSize > 5) {
			var13 = 3;
		}

		if(var1->m_item->stackSize > 20) {
			var13 = 4;
		}

  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4 + var11, (float)var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
		float var16;
		float var17;
		float var18;
		if(var10->itemID < 256 && RenderBlocks::func_1219_a(Block::blocksList[var10->itemID]->getRenderType())) {
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var12)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			Textures::load("game:\\Content\\terrain.png");
			float var27 = 0.25F;
			if(!Block::blocksList[var10->itemID]->renderAsNormalBlock() && var10->itemID != Block::s_stairSingle->blockID) {
				var27 = 0.5F;
			}

   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var27, var27, var27);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

			for(int var28 = 0; var28 < var13; ++var28) {
    D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
				if(var28 > 0) {
					var16 = (m_random->nextFloat() * 2.0F - 1.0F) * 0.2F / var27;
					var17 = (m_random->nextFloat() * 2.0F - 1.0F) * 0.2F / var27;
					var18 = (m_random->nextFloat() * 2.0F - 1.0F) * 0.2F / var27;
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var16, var17, var18);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}

				m_renderBlocks->func_1227_a(Block::blocksList[var10->itemID]);
    Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			}
		} else {
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 0.5F, 0.5F, 0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			int var14 = var10->getIconIndex();
			if(var10->itemID < 256) {
				Textures::load("game:\\Content\\terrain.png");
			} else {
				Textures::load("game:\\Content\\gui\\items.png");
			}

			Tesselator var15 = Tesselator::getInstance();
			var16 = (float)(var14 % 16 * 16 + 0) / 256.0F;
			var17 = (float)(var14 % 16 * 16 + 16) / 256.0F;
			var18 = (float)(var14 / 16 * 16 + 0) / 256.0F;
			float var19 = (float)(var14 / 16 * 16 + 16) / 256.0F;
			float var20 = 1.0F;
			float var21 = 0.5F;
			float var22 = 0.25F;

			for(int var23 = 0; var23 < var13; ++var23) {
    D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
				if(var23 > 0) {
					float var24 = (m_random->nextFloat() * 2.0F - 1.0F) * 0.3F;
					float var25 = (m_random->nextFloat() * 2.0F - 1.0F) * 0.3F;
					float var26 = (m_random->nextFloat() * 2.0F - 1.0F) * 0.3F;
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var24, var25, var26);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}

    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (180.0F - this->renderManager->m_field_1225_i)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				var15.init();
				var15.normal(0.0F, 1.0F, 0.0F);
				var15.vertexUV((double)(0.0F - var21), (double)(0.0F - var22), 0.0f, (double)var16, (double)var19);
				var15.vertexUV((double)(var20 - var21), (double)(0.0F - var22), 0.0f, (double)var17, (double)var19);
				var15.vertexUV((double)(var20 - var21), (double)(1.0F - var22), 0.0f, (double)var17, (double)var18);
				var15.vertexUV((double)(0.0F - var21), (double)(1.0F - var22), 0.0f, (double)var16, (double)var18);
				var15.flush();
    Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			}
		}

  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void RenderItem::renderItemIntoGUI(FontRenderer* var1, RenderEngine* var2, ItemStack* var3, int var4, int var5) {
		if(var3 != NULL) {
			if(var3->itemID < 256 && RenderBlocks::func_1219_a(Block::blocksList[var3->itemID]->getRenderType())) {
				int var6 = var3->itemID;
				var2->bindTexture(Textures::load("game:\\Content\\terrain.png"));
				Block* var7 = Block::blocksList[var6];
    D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(var4 - 2), (float)(var5 + 3), 0.0F);
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
    { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F, 1.0F, 1.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				m_renderBlocks->func_1227_a(var7);
    Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			} else if(var3->getIconIndex() >= 0) {
    // Lighting is shader-based on Xbox 360 (disabled)
				if(var3->itemID < 256) {
					var2->bindTexture(Textures::load("game:\\Content\\terrain.png"));
				} else {
					var2->bindTexture(Textures::load("game:\\Content\\gui\\items.png"));
				}

				renderTexturedQuad(var4, var5, var3->getIconIndex() % 16 * 16, var3->getIconIndex() / 16 * 16, 16, 16);
    // Lighting is shader-based on Xbox 360 (enabled)
			}

   g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		}
}

void RenderItem::renderItemOverlayIntoGUI(FontRenderer* var1, RenderEngine* var2, ItemStack* var3, int var4, int var5) {
		if(var3 != NULL) {
			if(var3->stackSize > 1) {
				std::string var6 = toStr(var3->stackSize);
    // Lighting is shader-based on Xbox 360 (disabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				var1->drawStringWithShadow(var6, var4 + 19 - 2 - var1->getStringWidth(var6), var5 + 6 + 3, 16777215);
    // Lighting is shader-based on Xbox 360 (enabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			}

			if(var3->itemDamage > 0) {
				int var11 = 13 - var3->itemDamage * 13 / var3->getMaxDamage();
				int var7 = 255 - var3->itemDamage * 255 / var3->getMaxDamage();
    // Lighting is shader-based on Xbox 360 (disabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    g_pd3dDevice->SetTexture(0, NULL);
				Tesselator var8 = Tesselator::getInstance();
				int var9 = (255 - var7) << 16 | var7 << 8;
				int var10 = (255 - var7) / 4 << 16 | 16128;
				renderQuad(&var8, var4 + 2, var5 + 13, 13, 2, 0);
				renderQuad(&var8, var4 + 2, var5 + 13, 12, 1, var10);
				renderQuad(&var8, var4 + 2, var5 + 13, var11, 1, var9);
    g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
    // Lighting is shader-based on Xbox 360 (enabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			}

		}
}

void RenderItem::renderQuad(Tesselator* var1, int var2, int var3, int var4, int var5, int var6) {
		var1->init();
		var1->colorI(var6);
		var1->vertex((double)(var2 + 0), (double)(var3 + 0), 0.0f);
		var1->vertex((double)(var2 + 0), (double)(var3 + var5), 0.0f);
		var1->vertex((double)(var2 + var4), (double)(var3 + var5), 0.0f);
		var1->vertex((double)(var2 + var4), (double)(var3 + 0), 0.0f);
		var1->flush();
}

void RenderItem::renderTexturedQuad(int var1, int var2, int var3, int var4, int var5, int var6) {
		float var7 = 0.0F;
		float var8 = 0.00390625F;
		float var9 = 0.00390625F;
		Tesselator var10 = Tesselator::getInstance();
		var10.init();
		var10.vertexUV((double)(var1 + 0), (double)(var2 + var6), (double)var7, (double)((float)(var3 + 0) * var8), (double)((float)(var4 + var6) * var9));
		var10.vertexUV((double)(var1 + var5), (double)(var2 + var6), (double)var7, (double)((float)(var3 + var5) * var8), (double)((float)(var4 + var6) * var9));
		var10.vertexUV((double)(var1 + var5), (double)(var2 + 0), (double)var7, (double)((float)(var3 + var5) * var8), (double)((float)(var4 + 0) * var9));
		var10.vertexUV((double)(var1 + 0), (double)(var2 + 0), (double)var7, (double)((float)(var3 + 0) * var8), (double)((float)(var4 + 0) * var9));
		var10.flush();
}

void RenderItem::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityItem*)var1, var2, var4, var6, var8, var9);
}
