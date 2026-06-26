#include "stdafx.h"
// Auto-generated from RenderArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderArrow.h"
#include "Common.h"
#include "Entity.h"
#include "EntityArrow.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

RenderArrow::RenderArrow()
{
}

RenderArrow::~RenderArrow() {
	// No dynamic resources
}

void RenderArrow::a(EntityArrow* var1, double var2, double var4, double var6, float var8, float var9) {
		Textures::load("game:\\Content\\item\\arrows.png");
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var9 - 90.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (var1->prevRotationPitch + (var1->rotationPitch - var1->prevRotationPitch) * var9)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		Tesselator var10 = Tesselator::getInstance();
		unsigned char var11 = 0;
		float var12 = 0.0F;
		float var13 = 0.5F;
		float var14 = (float)(0 + var11 * 10) / 32.0F;
		float var15 = (float)(5 + var11 * 10) / 32.0F;
		float var16 = 0.0F;
		float var17 = 0.15625F;
		float var18 = (float)(5 + var11 * 10) / 32.0F;
		float var19 = (float)(10 + var11 * 10) / 32.0F;
		float var20 = 0.05625F;
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
		float var21 = (float)var1->m_arrowShake - var9;
		if(var21 > 0.0F) {
			float var22 = -sinf(var21 * 3.0F) * var21;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (var22)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (45.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var20, var20, var20);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -4.0F, 0.0F, 0.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  Tesselator::getInstance().normal(var20, 0.0F, 0.0F);
		var10.init();
		var10.vertexUV(-7.0f, -2.0f, -2.0f, (double)var16, (double)var18);
		var10.vertexUV(-7.0f, -2.0f, 2.0f, (double)var17, (double)var18);
		var10.vertexUV(-7.0f, 2.0f, 2.0f, (double)var17, (double)var19);
		var10.vertexUV(-7.0f, 2.0f, -2.0f, (double)var16, (double)var19);
		var10.flush();
  Tesselator::getInstance().normal(-var20, 0.0F, 0.0F);
		var10.init();
		var10.vertexUV(-7.0f, 2.0f, -2.0f, (double)var16, (double)var18);
		var10.vertexUV(-7.0f, 2.0f, 2.0f, (double)var17, (double)var18);
		var10.vertexUV(-7.0f, -2.0f, 2.0f, (double)var17, (double)var19);
		var10.vertexUV(-7.0f, -2.0f, -2.0f, (double)var16, (double)var19);
		var10.flush();

		for(int var23 = 0; var23 < 4; ++var23) {
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (90.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   Tesselator::getInstance().normal(0.0F, 0.0F, var20);
			var10.init();
			var10.vertexUV(-8.0f, -2.0f, 0.0f, (double)var12, (double)var14);
			var10.vertexUV(8.0f, -2.0f, 0.0f, (double)var13, (double)var14);
			var10.vertexUV(8.0f, 2.0f, 0.0f, (double)var13, (double)var15);
			var10.vertexUV(-8.0f, 2.0f, 0.0f, (double)var12, (double)var15);
			var10.flush();
		}

  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void RenderArrow::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityArrow*)var1, var2, var4, var6, var8, var9);
}
