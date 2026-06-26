#include "stdafx.h"
// Auto-generated from ItemRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemRenderer.h"
#include "Common.h"
#include "ItemStack.h"
#include "Minecraft.h"
#include "RenderBlocks.h"
#include "Block.h"
#include "EntityPlayerSP.h"
#include "MathHelper.h"
#include "RenderHelper.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

ItemRenderer::ItemRenderer()
	: mc(NULL)
	, m_itemToRender(NULL)
	, m_equippedProgress(0.0f)
	, m_prevEquippedProgress(0.0f)
	, swingProgress(0)
	, m_itemSwingState(false)
	, m_renderBlocksInstance(new RenderBlocks())
{
}

ItemRenderer::ItemRenderer(Minecraft* var1)
	: mc(NULL)
	, m_itemToRender(NULL)
	, m_equippedProgress(0.0f)
	, m_prevEquippedProgress(0.0f)
	, swingProgress(0)
	, m_itemSwingState(false)
	, m_renderBlocksInstance(new RenderBlocks())
{
		mc = var1;
}

ItemRenderer::~ItemRenderer() {
	mc = NULL;
	m_itemToRender = NULL;
	m_renderBlocksInstance = NULL;
}

void ItemRenderer::renderItemInFirstPerson(float var1) {
		float var2 = m_prevEquippedProgress + (m_equippedProgress - m_prevEquippedProgress) * var1;
		EntityPlayerSP var3 = mc->thePlayer;
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (var3.prevRotationPitch + (var3.rotationPitch - var3.prevRotationPitch) * var1)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var3.prevRotationYaw + (var3.rotationYaw - var3.prevRotationYaw) * var1)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		RenderHelper::enableStandardItemLighting();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		float var4;
		float var5;
		float var6;
		if(m_itemSwingState) {
			var4 = ((float)swingProgress + var1) / 8.0F;
			var5 = sinf(var4 * 3.14159265358979323846f);
			var6 = sinf(sqrtf(var4) * 3.14159265358979323846f);
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -var6 * 0.4F, MathHelper::sin(MathHelper::sqrt_float(var4) * (float)Math.PI * 2.0F) * 0.2F, -var5 * 0.2F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.56F, -0.52F - (1.0F - var2) * 0.6F, -0.71999997F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (45.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  // Normalize is shader-based on Xbox 360 (enabled)
		if(m_itemSwingState) {
			var4 = ((float)swingProgress + var1) / 8.0F;
			var5 = sinf(var4 * var4 * 3.14159265358979323846f);
			var6 = sinf(sqrtf(var4) * 3.14159265358979323846f);
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-var5 * 20.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (-var6 * 20.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (-var6 * 80.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		var4 = mc->theWorld->getLightBrightness((int)var3.posX, (int)var3.posY, (int)var3.posZ);
  Tesselator::getInstance().color4f(var4, var4, var4, 1.0F);
		if(m_itemToRender != NULL) {
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 0.4F, 0.4F, 0.4F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			if(m_itemToRender->itemID < 256 && Block::blocksList[m_itemToRender->itemID].getRenderType() == 0) {
    g_pd3dDevice->SetTexture(0, Textures::getById(this.mc->m_renderEngine->getTexture("/terrain.png")));
				m_renderBlocksInstance->renderBlockOnInventory(Block::blocksList[m_itemToRender->itemID]);
			} else {
				if(m_itemToRender->itemID < 256) {
     g_pd3dDevice->SetTexture(0, Textures::getById(this.mc->m_renderEngine->getTexture("/terrain.png")));
				} else {
     g_pd3dDevice->SetTexture(0, Textures::getById(this.mc->m_renderEngine->getTexture("/gui/items.png")));
				}

				Tesselator var12 = Tesselator::getInstance();
				ItemStack var10 = m_itemToRender;
				var1 = (float)(var10.getItem().getIconIndex() % 16 << 4) / 256.0F;
				var10 = m_itemToRender;
				var2 = (float)((var10.getItem().getIconIndex() % 16 << 4) + 16) / 256.0F;
				var10 = m_itemToRender;
				float var9 = (float)(var10.getItem().getIconIndex() / 16 << 4) / 256.0F;
				var10 = m_itemToRender;
				var4 = (float)((var10.getItem().getIconIndex() / 16 << 4) + 16) / 256.0F;
    // Normalize is shader-based on Xbox 360 (enabled)
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, -0.3F, 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.5F, 1.5F, 1.5F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (50.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (335.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -(15.0F / 16.0F), -(1.0F / 16.0F), 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				Tesselator::normal(0.0F, 0.0F, 1.0F);
				var12.init();
				var12.vertexUV(0.0F, 0.0F, 0.0F, var2, var4);
				var12.vertexUV(1.0F, 0.0F, 0.0F, var1, var4);
				var12.vertexUV(1.0F, 1.0F, 0.0F, var1, var9);
				var12.vertexUV(0.0F, 1.0F, 0.0F, var2, var9);
				var12.flush();
				Tesselator::normal(0.0F, 0.0F, -1.0F);
				var12.init();
				var12.vertexUV(0.0F, 1.0F, -(1.0F / 16.0F), var2, var9);
				var12.vertexUV(1.0F, 1.0F, -(1.0F / 16.0F), var1, var9);
				var12.vertexUV(1.0F, 0.0F, -(1.0F / 16.0F), var1, var4);
				var12.vertexUV(0.0F, 0.0F, -(1.0F / 16.0F), var2, var4);
				var12.flush();
				Tesselator::normal(-1.0F, 0.0F, 0.0F);
				var12.init();

				float var7;
				float var8;
				int var11;
				for(var11 = 0; var11 < 16; ++var11) {
					var7 = (float)var11 / 16.0F;
					var8 = var2 + (var1 - var2) * var7 - 0.001953125F;
					var7 *= 1.0F;
					var12.vertexUV(var7, 0.0F, -(1.0F / 16.0F), var8, var4);
					var12.vertexUV(var7, 0.0F, 0.0F, var8, var4);
					var12.vertexUV(var7, 1.0F, 0.0F, var8, var9);
					var12.vertexUV(var7, 1.0F, -(1.0F / 16.0F), var8, var9);
				}

				var12.flush();
				Tesselator::normal(1.0F, 0.0F, 0.0F);
				var12.init();

				for(var11 = 0; var11 < 16; ++var11) {
					var7 = (float)var11 / 16.0F;
					var8 = var2 + (var1 - var2) * var7 - 0.001953125F;
					var7 = var7 * 1.0F + 1.0F / 16.0F;
					var12.vertexUV(var7, 1.0F, -(1.0F / 16.0F), var8, var9);
					var12.vertexUV(var7, 1.0F, 0.0F, var8, var9);
					var12.vertexUV(var7, 0.0F, 0.0F, var8, var4);
					var12.vertexUV(var7, 0.0F, -(1.0F / 16.0F), var8, var4);
				}

				var12.flush();
				Tesselator::normal(0.0F, 1.0F, 0.0F);
				var12.init();

				for(var11 = 0; var11 < 16; ++var11) {
					var7 = (float)var11 / 16.0F;
					var8 = var4 + (var9 - var4) * var7 - 0.001953125F;
					var7 = var7 * 1.0F + 1.0F / 16.0F;
					var12.vertexUV(0.0F, var7, 0.0F, var2, var8);
					var12.vertexUV(1.0F, var7, 0.0F, var1, var8);
					var12.vertexUV(1.0F, var7, -(1.0F / 16.0F), var1, var8);
					var12.vertexUV(0.0F, var7, -(1.0F / 16.0F), var2, var8);
				}

				var12.flush();
				Tesselator::normal(0.0F, -1.0F, 0.0F);
				var12.init();

				for(var11 = 0; var11 < 16; ++var11) {
					var7 = (float)var11 / 16.0F;
					var8 = var4 + (var9 - var4) * var7 - 0.001953125F;
					var7 *= 1.0F;
					var12.vertexUV(1.0F, var7, 0.0F, var1, var8);
					var12.vertexUV(0.0F, var7, 0.0F, var2, var8);
					var12.vertexUV(0.0F, var7, -(1.0F / 16.0F), var2, var8);
					var12.vertexUV(1.0F, var7, -(1.0F / 16.0F), var1, var8);
				}

				var12.flush();
    // Normalize is shader-based on Xbox 360 (disabled)
			}
		} else {
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F, -1.0F, -1.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.2F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (-120.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F, 1.0F, 1.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

  // Normalize is shader-based on Xbox 360 (disabled)
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		RenderHelper::disableStandardItemLighting();
}

void ItemRenderer::renderOverlays(float var1) {
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		int var2;
		Tesselator var3;
		float var7;
		float var9;
		if(mc->thePlayer->fire > 0) {
			var2 = mc->Textures::load("game:\Content\terrain.png");
   g_pd3dDevice->SetTexture(0, Textures::getById(var2));
			var3 = Tesselator::getInstance();
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 0.9F);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			for(var2 = 0; var2 < 2; ++var2) {
    D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
				int var4 = Block::s_fire->blockIndexInTexture + (var2 << 4);
				int var5 = (var4 & 15) << 4;
				var4 &= 240;
				float var6 = (float)var5 / 256.0F;
				float var10 = ((float)var5 + 15.99F) / 256.0F;
				var7 = (float)var4 / 256.0F;
				var9 = ((float)var4 + 15.99F) / 256.0F;
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(-((var2 << 1) - 1)) * 0.24F, -0.3F, 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), ((float)((var2 << 1) - 1) * 10.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				var3.init();
				var3.vertexUV(-0.5F, -0.5F, -0.5F, var10, var9);
				var3.vertexUV(0.5F, -0.5F, -0.5F, var6, var9);
				var3.vertexUV(0.5F, 0.5F, -0.5F, var6, var7);
				var3.vertexUV(-0.5F, 0.5F, -0.5F, var10, var7);
				var3.flush();
    Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			}

   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}

		if(mc->thePlayer->isInsideOfMaterial()) {
			var2 = mc->Textures::load("game:\Content\water.png");
   g_pd3dDevice->SetTexture(0, Textures::getById(var2));
			var3 = Tesselator::getInstance();
			float var8 = mc->thePlayer->getEntityBrightness(var1);
   Tesselator::getInstance().color4f(var8, var8, var8, 0.5F);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			var7 = -mc->thePlayer->rotationYaw / 64.0F;
			var9 = mc->thePlayer->rotationPitch / 64.0F;
			var3.init();
			var3.vertexUV(-1.0F, -1.0F, -0.5F, var7 + 4.0F, var9 + 4.0F);
			var3.vertexUV(1.0F, -1.0F, -0.5F, var7 + 0.0F, var9 + 4.0F);
			var3.vertexUV(1.0F, 1.0F, -0.5F, var7 + 0.0F, var9 + 0.0F);
			var3.vertexUV(-1.0F, 1.0F, -0.5F, var7 + 4.0F, var9 + 0.0F);
			var3.flush();
   Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}

  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
}

void ItemRenderer::updateEquippedItem() {
		m_prevEquippedProgress = m_equippedProgress;
		if(m_itemSwingState) {
			++swingProgress;
			if(swingProgress == 8) {
				swingProgress = 0;
				m_itemSwingState = false;
			}
		}

		EntityPlayerSP var1 = mc->thePlayer;
		ItemStack var3 = var1.m_inventory.getCurrentItem();
		float var2 = var3 == m_itemToRender ? 1.0F : 0.0F;
		var2 -= m_equippedProgress;
		if(var2 < -0.4F) {
			var2 = -0.4F;
		}

		if(var2 > 0.4F) {
			var2 = 0.4F;
		}

		m_equippedProgress += var2;
		if(m_equippedProgress < 0.1F) {
			m_itemToRender = var3;
		}

}

void ItemRenderer::equipAnimationSpeed() {
		m_equippedProgress = 0.0F;
}

void ItemRenderer::equippedItemRender() {
		swingProgress = -1;
		m_itemSwingState = true;
}

void ItemRenderer::resetEquippedProgress() {
		m_equippedProgress = 0.0F;
}
