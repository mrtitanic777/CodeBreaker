#include "stdafx.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// Auto-generated from ItemRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemRenderer.h"
#include "Common.h"
#include "ItemStack.h"
#include "Minecraft.h"
#include "RenderBlocks.h"
#include "Block.h"
#include "EntityPlayerSP.h"
#include "Item.h"
#include "Material.h"
#include "MathHelper.h"
#include "RenderHelper.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "BlockFire.h"
#include "InventoryPlayer.h"
#include "Render.h"
#include "RenderEngine.h"
#include "RenderManager.h"
#include "RenderPlayer.h"
#include "World.h"

ItemRenderer::ItemRenderer()
	: mc(NULL)
	, m_field_9451_b(NULL)
	, m_field_9453_c(0.0f)
	, m_field_9452_d(0.0f)
	, m_field_1357_e(new RenderBlocks())
{
}

ItemRenderer::ItemRenderer(Minecraft* var1)
	: mc(NULL)
	, m_field_9451_b(NULL)
	, m_field_9453_c(0.0f)
	, m_field_9452_d(0.0f)
	, m_field_1357_e(new RenderBlocks())
{
		mc = var1;
}

ItemRenderer::~ItemRenderer() {
	mc = NULL;
	m_field_9451_b = NULL;
	m_field_1357_e = NULL;
}

void ItemRenderer::renderItem(ItemStack* var1) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		if(var1->itemID < 256 && RenderBlocks::func_1219_a(Block::blocksList[var1->itemID]->getRenderType())) {
   g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/terrain.png")));
			m_field_1357_e->func_1227_a(Block::blocksList[var1->itemID]);
		} else {
			if(var1->itemID < 256) {
    g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/terrain.png")));
			} else {
    g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/gui/items.png")));
			}

			Tesselator var2 = Tesselator::getInstance();
			float var3 = ((float)(var1->getIconIndex() % 16 * 16) + 0.0F) / 256.0F;
			float var4 = ((float)(var1->getIconIndex() % 16 * 16) + 15.99F) / 256.0F;
			float var5 = ((float)(var1->getIconIndex() / 16 * 16) + 0.0F) / 256.0F;
			float var6 = ((float)(var1->getIconIndex() / 16 * 16) + 15.99F) / 256.0F;
			float var7 = 1.0F;
			float var8 = 0.0F;
			float var9 = 0.3F;
   // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -var8, -var9, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			float var10 = 1.5F;
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var10, var10, var10);
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
			float var11 = 1.0F / 16.0F;
			var2.init();
			var2.normal(0.0F, 0.0F, 1.0F);
			var2.vertexUV(0.0f, 0.0f, 0.0f, (double)var4, (double)var6);
			var2.vertexUV((double)var7, 0.0f, 0.0f, (double)var3, (double)var6);
			var2.vertexUV((double)var7, 1.0f, 0.0f, (double)var3, (double)var5);
			var2.vertexUV(0.0f, 1.0f, 0.0f, (double)var4, (double)var5);
			var2.flush();
			var2.init();
			var2.normal(0.0F, 0.0F, -1.0F);
			var2.vertexUV(0.0f, 1.0f, (double)(0.0F - var11), (double)var4, (double)var5);
			var2.vertexUV((double)var7, 1.0f, (double)(0.0F - var11), (double)var3, (double)var5);
			var2.vertexUV((double)var7, 0.0f, (double)(0.0F - var11), (double)var3, (double)var6);
			var2.vertexUV(0.0f, 0.0f, (double)(0.0F - var11), (double)var4, (double)var6);
			var2.flush();
			var2.init();
			var2.normal(-1.0F, 0.0F, 0.0F);

			int var12;
			float var13;
			float var14;
			float var15;
			for(var12 = 0; var12 < 16; ++var12) {
				var13 = (float)var12 / 16.0F;
				var14 = var4 + (var3 - var4) * var13 - 0.001953125F;
				var15 = var7 * var13;
				var2.vertexUV((double)var15, 0.0f, (double)(0.0F - var11), (double)var14, (double)var6);
				var2.vertexUV((double)var15, 0.0f, 0.0f, (double)var14, (double)var6);
				var2.vertexUV((double)var15, 1.0f, 0.0f, (double)var14, (double)var5);
				var2.vertexUV((double)var15, 1.0f, (double)(0.0F - var11), (double)var14, (double)var5);
			}

			var2.flush();
			var2.init();
			var2.normal(1.0F, 0.0F, 0.0F);

			for(var12 = 0; var12 < 16; ++var12) {
				var13 = (float)var12 / 16.0F;
				var14 = var4 + (var3 - var4) * var13 - 0.001953125F;
				var15 = var7 * var13 + 1.0F / 16.0F;
				var2.vertexUV((double)var15, 1.0f, (double)(0.0F - var11), (double)var14, (double)var5);
				var2.vertexUV((double)var15, 1.0f, 0.0f, (double)var14, (double)var5);
				var2.vertexUV((double)var15, 0.0f, 0.0f, (double)var14, (double)var6);
				var2.vertexUV((double)var15, 0.0f, (double)(0.0F - var11), (double)var14, (double)var6);
			}

			var2.flush();
			var2.init();
			var2.normal(0.0F, 1.0F, 0.0F);

			for(var12 = 0; var12 < 16; ++var12) {
				var13 = (float)var12 / 16.0F;
				var14 = var6 + (var5 - var6) * var13 - 0.001953125F;
				var15 = var7 * var13 + 1.0F / 16.0F;
				var2.vertexUV(0.0f, (double)var15, 0.0f, (double)var4, (double)var14);
				var2.vertexUV((double)var7, (double)var15, 0.0f, (double)var3, (double)var14);
				var2.vertexUV((double)var7, (double)var15, (double)(0.0F - var11), (double)var3, (double)var14);
				var2.vertexUV(0.0f, (double)var15, (double)(0.0F - var11), (double)var4, (double)var14);
			}

			var2.flush();
			var2.init();
			var2.normal(0.0F, -1.0F, 0.0F);

			for(var12 = 0; var12 < 16; ++var12) {
				var13 = (float)var12 / 16.0F;
				var14 = var6 + (var5 - var6) * var13 - 0.001953125F;
				var15 = var7 * var13;
				var2.vertexUV((double)var7, (double)var15, 0.0f, (double)var3, (double)var14);
				var2.vertexUV(0.0f, (double)var15, 0.0f, (double)var4, (double)var14);
				var2.vertexUV(0.0f, (double)var15, (double)(0.0F - var11), (double)var4, (double)var14);
				var2.vertexUV((double)var7, (double)var15, (double)(0.0F - var11), (double)var3, (double)var14);
			}

			var2.flush();
   // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
		}

  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void ItemRenderer::renderItemInFirstPerson(float var1) {
		float var2 = m_field_9452_d + (m_field_9453_c - m_field_9452_d) * var1;
		EntityPlayerSP* var3 = mc->thePlayer;
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (var3->prevRotationPitch + (var3->rotationPitch - var3->prevRotationPitch) * var1)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var3->prevRotationYaw + (var3->rotationYaw - var3->prevRotationYaw) * var1)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		RenderHelper::enableStandardItemLighting();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		float var4 = mc->theWorld->getLightBrightness((int)floor(var3->posX), (int)floor(var3->posY), (int)floor(var3->posZ));
  Tesselator::getInstance().color4f(var4, var4, var4, 1.0F);
		ItemStack* var5 = m_field_9451_b;
		if(var3->m_fishEntity != NULL) {
			var5 = new ItemStack(Item::s_stick->shiftedIndex);
		}

		float var6;
		float var7;
		float var8;
		float var9;
		if(var5 != NULL) {
   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			var6 = 0.8F;
			var7 = var3->getSwingProgress(var1);
			var8 = sinf(var7 * 3.14159265358979323846f);
			var9 = sinf(sqrtf(var7) * 3.14159265358979323846f);
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -var9 * 0.4F, MathHelper::sin(MathHelper::sqrt_float(var7) * (float)M_PI * 2.0F) * 0.2F, -var8 * 0.2F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.7F * var6, -0.65F * var6 - (1.0F - var2) * 0.6F, -0.9F * var6);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (45.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
			var7 = var3->getSwingProgress(var1);
			var8 = sinf(var7 * var7 * 3.14159265358979323846f);
			var9 = sinf(sqrtf(var7) * 3.14159265358979323846f);
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-var8 * 20.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (-var9 * 20.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (-var9 * 80.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var7 = 0.4F;
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var7, var7, var7);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			if(var5->getItem()->shouldRotateAroundWhenRendering()) {
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (180.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

			renderItem(var5);
   Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		} else {
   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			var6 = 0.8F;
			var7 = var3->getSwingProgress(var1);
			var8 = sinf(var7 * 3.14159265358979323846f);
			var9 = sinf(sqrtf(var7) * 3.14159265358979323846f);
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -var9 * 0.3F, MathHelper::sin(MathHelper::sqrt_float(var7) * (float)M_PI * 2.0F) * 0.4F, -var8 * 0.4F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.8F * var6, -(12.0F / 16.0F) * var6 - (1.0F - var2) * 0.6F, -0.9F * var6);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (45.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
			var7 = var3->getSwingProgress(var1);
			var8 = sinf(var7 * var7 * 3.14159265358979323846f);
			var9 = sinf(sqrtf(var7) * 3.14159265358979323846f);
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var9 * 70.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (-var8 * 20.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTextureForDownloadableImage(this->mc->thePlayer->skinUrl, this->mc->thePlayer->getEntityTexture())));
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -1.0F, 3.6F, 3.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (120.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (200.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-135.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F, 1.0F, 1.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 5.6F, 0.0F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			Render* var10 = RenderManager::instance->func_855_a(mc->thePlayer);
			RenderPlayer* var11 = (RenderPlayer*)var10;
			var9 = 1.0F;
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var9, var9, var9);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var11->drawFirstPersonHand();
   Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		}

  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
		RenderHelper::disableStandardItemLighting();
}

void ItemRenderer::renderOverlays(float var1) {
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		int var2;
		if(mc->thePlayer->fire > 0 || mc->thePlayer->m_field_9299_bv) {
			var2 = Textures::load("game:\\Content\\terrain.png");
   g_pd3dDevice->SetTexture(0, Textures::getById(var2));
			renderFireInFirstPerson(var1);
		}

		if(mc->thePlayer->func_345_I()) {
			var2 = (int)floor(mc->thePlayer->posX);
			int var3 = (int)floor(mc->thePlayer->posY);
			int var4 = (int)floor(mc->thePlayer->posZ);
			int var5 = Textures::load("game:\\Content\\terrain.png");
   g_pd3dDevice->SetTexture(0, Textures::getById(var5));
			int var6 = mc->theWorld->getBlockId(var2, var3, var4);
			if(Block::blocksList[var6] != NULL) {
				renderInsideOfBlock(var1, Block::blocksList[var6]->getBlockTextureFromSide(2));
			}
		}

		if(mc->thePlayer->isInsideOfMaterial(Material::s_water)) {
			var2 = Textures::load("game:\\Content\\misc\\water.png");
   g_pd3dDevice->SetTexture(0, Textures::getById(var2));
			renderWarpedTextureOverlay(var1);
		}

  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
}

void ItemRenderer::renderInsideOfBlock(float var1, int var2) {
		Tesselator var3 = Tesselator::getInstance();
		mc->thePlayer->getEntityBrightness(var1);
		float var4 = 0.1F;
  Tesselator::getInstance().color4f(var4, var4, var4, 0.5F);
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		float var5 = -1.0F;
		float var6 = 1.0F;
		float var7 = -1.0F;
		float var8 = 1.0F;
		float var9 = -0.5F;
		float var10 = 0.0078125F;
		float var11 = (float)(var2 % 16) / 256.0F - var10;
		float var12 = ((float)(var2 % 16) + 15.99F) / 256.0F + var10;
		float var13 = (float)(var2 / 16) / 256.0F - var10;
		float var14 = ((float)(var2 / 16) + 15.99F) / 256.0F + var10;
		var3.init();
		var3.vertexUV((double)var5, (double)var7, (double)var9, (double)var12, (double)var14);
		var3.vertexUV((double)var6, (double)var7, (double)var9, (double)var11, (double)var14);
		var3.vertexUV((double)var6, (double)var8, (double)var9, (double)var11, (double)var13);
		var3.vertexUV((double)var5, (double)var8, (double)var9, (double)var12, (double)var13);
		var3.flush();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
}

void ItemRenderer::renderWarpedTextureOverlay(float var1) {
		Tesselator var2 = Tesselator::getInstance();
		float var3 = mc->thePlayer->getEntityBrightness(var1);
  Tesselator::getInstance().color4f(var3, var3, var3, 0.5F);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		float var4 = 4.0F;
		float var5 = -1.0F;
		float var6 = 1.0F;
		float var7 = -1.0F;
		float var8 = 1.0F;
		float var9 = -0.5F;
		float var10 = -mc->thePlayer->rotationYaw / 64.0F;
		float var11 = mc->thePlayer->rotationPitch / 64.0F;
		var2.init();
		var2.vertexUV((double)var5, (double)var7, (double)var9, (double)(var4 + var10), (double)(var4 + var11));
		var2.vertexUV((double)var6, (double)var7, (double)var9, (double)(0.0F + var10), (double)(var4 + var11));
		var2.vertexUV((double)var6, (double)var8, (double)var9, (double)(0.0F + var10), (double)(0.0F + var11));
		var2.vertexUV((double)var5, (double)var8, (double)var9, (double)(var4 + var10), (double)(0.0F + var11));
		var2.flush();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void ItemRenderer::renderFireInFirstPerson(float var1) {
		Tesselator var2 = Tesselator::getInstance();
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 0.9F);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		float var3 = 1.0F;

		for(int var4 = 0; var4 < 2; ++var4) {
   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			int var5 = Block::s_fire->blockIndexInTexture + var4 * 16;
			int var6 = (var5 & 15) << 4;
			int var7 = var5 & 240;
			float var8 = (float)var6 / 256.0F;
			float var9 = ((float)var6 + 15.99F) / 256.0F;
			float var10 = (float)var7 / 256.0F;
			float var11 = ((float)var7 + 15.99F) / 256.0F;
			float var12 = (0.0F - var3) / 2.0F;
			float var13 = var12 + var3;
			float var14 = 0.0F - var3 / 2.0F;
			float var15 = var14 + var3;
			float var16 = -0.5F;
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(-(var4 * 2 - 1)) * 0.24F, -0.3F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), ((float)(var4 * 2 - 1) * 10.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var2.init();
			var2.vertexUV((double)var12, (double)var14, (double)var16, (double)var9, (double)var11);
			var2.vertexUV((double)var13, (double)var14, (double)var16, (double)var8, (double)var11);
			var2.vertexUV((double)var13, (double)var15, (double)var16, (double)var8, (double)var10);
			var2.vertexUV((double)var12, (double)var15, (double)var16, (double)var9, (double)var10);
			var2.flush();
   Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		}

  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void ItemRenderer::func_895_a() {
		m_field_9452_d = m_field_9453_c;
		EntityPlayerSP* var1 = mc->thePlayer;
		ItemStack* var2 = var1->m_inventory->getCurrentItem();
		float var4 = 0.4F;
		float var5 = var2 == m_field_9451_b ? 1.0F : 0.0F;
		float var6 = var5 - m_field_9453_c;
		if(var6 < -var4) {
			var6 = -var4;
		}

		if(var6 > var4) {
			var6 = var4;
		}

		m_field_9453_c += var6;
		if(m_field_9453_c < 0.1F) {
			m_field_9451_b = var2;
		}

}

void ItemRenderer::func_9449_b() {
		m_field_9453_c = 0.0F;
}

void ItemRenderer::func_9450_c() {
		m_field_9453_c = 0.0F;
}
