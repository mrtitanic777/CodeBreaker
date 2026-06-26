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

void RenderArrow::doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6) {
		EntityArrow var8 = (EntityArrow)var1;
		Textures::load("game:\Content\item\arrows.png");
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var2, var3, var4);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var8.prevRotationYaw + (var8.rotationYaw - var8.prevRotationYaw) * var6 - 90.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (var8.prevRotationPitch + (var8.rotationPitch - var8.prevRotationPitch) * var6)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		Tesselator var7 = Tesselator::getInstance();
  // Normalize is shader-based on Xbox 360 (enabled)
		var2 = (float)var8.m_arrowShake - var6;
		if(var2 > 0.0F) {
			var2 = -sinf(var2 * 3.0F) * var2;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (var2)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (45.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 0.05625F, 0.05625F, 0.05625F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -4.0F, 0.0F, 0.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  Tesselator::getInstance().normal(0.05625F, 0.0F, 0.0F);
		var7.init();
		var7.vertexUV(-7.0F, -2.0F, -2.0F, 0.0F, 0.15625F);
		var7.vertexUV(-7.0F, -2.0F, 2.0F, 0.15625F, 0.15625F);
		var7.vertexUV(-7.0F, 2.0F, 2.0F, 0.15625F, 5.0F / 16.0F);
		var7.vertexUV(-7.0F, 2.0F, -2.0F, 0.0F, 5.0F / 16.0F);
		var7.flush();
  Tesselator::getInstance().normal(-0.05625F, 0.0F, 0.0F);
		var7.init();
		var7.vertexUV(-7.0F, 2.0F, -2.0F, 0.0F, 0.15625F);
		var7.vertexUV(-7.0F, 2.0F, 2.0F, 0.15625F, 0.15625F);
		var7.vertexUV(-7.0F, -2.0F, 2.0F, 0.15625F, 5.0F / 16.0F);
		var7.vertexUV(-7.0F, -2.0F, -2.0F, 0.0F, 5.0F / 16.0F);
		var7.flush();

		for(int var9 = 0; var9 < 4; ++var9) {
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (90.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   Tesselator::getInstance().normal(0.0F, 0.0F, 0.05625F);
			var7.init();
			var7.vertexUV(-8.0F, -2.0F, 0.0F, 0.0F, 0.0F);
			var7.vertexUV(8.0F, -2.0F, 0.0F, 0.5F, 0.0F);
			var7.vertexUV(8.0F, 2.0F, 0.0F, 0.5F, 0.15625F);
			var7.vertexUV(-8.0F, 2.0F, 0.0F, 0.0F, 0.15625F);
			var7.flush();
		}

  // Normalize is shader-based on Xbox 360 (disabled)
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}
