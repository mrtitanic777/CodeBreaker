#include "stdafx.h"
// Auto-generated from RenderFireball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderFireball.h"
#include "Common.h"
#include "Entity.h"
#include "EntityFireball.h"
#include "ItemStack.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "Item.h"
#include "RenderManager.h"

RenderFireball::RenderFireball()
{
}

RenderFireball::~RenderFireball() {
	// No dynamic resources
}

void RenderFireball::a(EntityFireball* var1, double var2, double var4, double var6, float var8, float var9) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
		float var10 = 2.0F;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var10 / 1.0F, var10 / 1.0F, var10 / 1.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		int var11 = Item::s_snowball->getIconIndex(NULL);
		Textures::load("game:\\Content\\gui\\items.png");
		Tesselator var12 = Tesselator::getInstance();
		float var13 = (float)(var11 % 16 * 16 + 0) / 256.0F;
		float var14 = (float)(var11 % 16 * 16 + 16) / 256.0F;
		float var15 = (float)(var11 / 16 * 16 + 0) / 256.0F;
		float var16 = (float)(var11 / 16 * 16 + 16) / 256.0F;
		float var17 = 1.0F;
		float var18 = 0.5F;
		float var19 = 0.25F;
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (180.0F - this->renderManager->m_field_1225_i)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (-this->renderManager->m_field_1224_j)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		var12.init();
		var12.normal(0.0F, 1.0F, 0.0F);
		var12.vertexUV((double)(0.0F - var18), (double)(0.0F - var19), 0.0f, (double)var13, (double)var16);
		var12.vertexUV((double)(var17 - var18), (double)(0.0F - var19), 0.0f, (double)var14, (double)var16);
		var12.vertexUV((double)(var17 - var18), (double)(1.0F - var19), 0.0f, (double)var14, (double)var15);
		var12.vertexUV((double)(0.0F - var18), (double)(1.0F - var19), 0.0f, (double)var13, (double)var15);
		var12.flush();
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void RenderFireball::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityFireball*)var1, var2, var4, var6, var8, var9);
}
