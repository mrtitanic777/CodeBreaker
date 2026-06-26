#include "stdafx.h"
// Auto-generated from RenderMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderMinecart.h"
#include "Common.h"
#include "Entity.h"
#include "EntityMinecart.h"
#include "ModelBase.h"
#include "Block.h"
#include "MathHelper.h"
#include "RenderBlocks.h"
#include "Shaders.h"
#include "Textures.h"
#include "Vec3D.h"
#include "ModelMinecart.h"

RenderMinecart::RenderMinecart()
	: m_a(NULL)
{
		m_field_9246_c = 0.5F;
		m_a = new ModelMinecart();
}

RenderMinecart::~RenderMinecart() {
	m_a = NULL;
}

void RenderMinecart::a(EntityMinecart* var1, double var2, double var4, double var6, float var8, float var9) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		double var10 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var9;
		double var12 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var9;
		double var14 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var9;
		double var16 = (double)0.3F;
		Vec3D* var18 = var1->func_514_g(var10, var12, var14);
		float var19 = var1->prevRotationPitch + (var1->rotationPitch - var1->prevRotationPitch) * var9;
		if(var18 != NULL) {
			Vec3D* var20 = var1->func_515_a(var10, var12, var14, var16);
			Vec3D* var21 = var1->func_515_a(var10, var12, var14, -var16);
			if(var20 == NULL) {
				var20 = var18;
			}

			if(var21 == NULL) {
				var21 = var18;
			}

			var2 += var18->xCoord - var10;
			var4 += (var20->yCoord + var21->yCoord) / 2.0f - var12;
			var6 += var18->zCoord - var14;
			Vec3D* var22 = var21->addVector(-var20->xCoord, -var20->yCoord, -var20->zCoord);
			if(var22->lengthVector() != 0.0f) {
				var22 = var22->normalize();
				var8 = (float)(atan2f(var22->zCoord, var22->xCoord) * 180.0f / 3.14159265358979323846f);
				var19 = (float)(atanf(var22->yCoord) * 73.0f);
			}
		}

  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (180.0F - var8)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (-var19)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var23 = (float)var1->m_b - var9;
		float var24 = (float)var1->m_a - var9;
		if(var24 < 0.0F) {
			var24 = 0.0F;
		}

		if(var23 > 0.0F) {
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (MathHelper::sin(var23) * var23 * var24 / 10.0F * (float)var1->m_c)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		if(var1->m_d != 0) {
			Textures::load("game:\\Content\\terrain.png");
			float var25 = 12.0F / 16.0F;
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var25, var25, var25);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 5.0F / 16.0F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (90.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			if(var1->m_d == 1) {
				(new RenderBlocks())->func_1227_a(Block::s_crate);
			} else if(var1->m_d == 2) {
				(new RenderBlocks())->func_1227_a(Block::s_stoneOvenIdle);
			}

   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-90.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, -(5.0F / 16.0F), 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F / var25, 1.0F / var25, 1.0F / var25);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		Textures::load("game:\\Content\\item\\cart.png");
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, -1.0F, -1.0F, 1.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		m_a->render(0.0F, 0.0F, -0.1F, 0.0F, 0.0F, 1.0F / 16.0F);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void RenderMinecart::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityMinecart*)var1, var2, var4, var6, var8, var9);
}
