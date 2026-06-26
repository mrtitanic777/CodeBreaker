#include "stdafx.h"
// Auto-generated from RenderBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderBoat.h"
#include "Common.h"
#include "Entity.h"
#include "EntityBoat.h"
#include "ModelBase.h"
#include "MathHelper.h"
#include "Shaders.h"
#include "Textures.h"
#include "ModelBoat.h"

RenderBoat::RenderBoat()
	: m_field_198_d(NULL)
{
		m_field_9246_c = 0.5F;
		m_field_198_d = new ModelBoat();
}

RenderBoat::~RenderBoat() {
	m_field_198_d = NULL;
}

void RenderBoat::a(EntityBoat* var1, double var2, double var4, double var6, float var8, float var9) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (180.0F - var8)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var10 = (float)var1->m_field_806_b - var9;
		float var11 = (float)var1->m_field_807_a - var9;
		if(var11 < 0.0F) {
			var11 = 0.0F;
		}

		if(var10 > 0.0F) {
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (MathHelper::sin(var10) * var10 * var11 / 10.0F * (float)var1->m_field_808_c)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		Textures::load("game:\\Content\\terrain.png");
		float var12 = 12.0F / 16.0F;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var12, var12, var12);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F / var12, 1.0F / var12, 1.0F / var12);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		Textures::load("game:\\Content\\item\\boat.png");
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, -1.0F, -1.0F, 1.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		m_field_198_d->render(0.0F, 0.0F, -0.1F, 0.0F, 0.0F, 1.0F / 16.0F);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void RenderBoat::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityBoat*)var1, var2, var4, var6, var8, var9);
}
