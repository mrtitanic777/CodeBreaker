#include "stdafx.h"
// Auto-generated from TileEntitySignRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntitySignRenderer.h"
#include "Common.h"
#include "SignModel.h"
#include "TileEntity.h"
#include "TileEntitySign.h"
#include <string>
#include "Block.h"
#include "FontRenderer.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "ModelRenderer.h"

TileEntitySignRenderer::TileEntitySignRenderer()
	: m_signModel(new SignModel())
{
}

TileEntitySignRenderer::~TileEntitySignRenderer() {
	m_signModel = NULL;
}

void TileEntitySignRenderer::a(TileEntitySign* var1, double var2, double var4, double var6, float var8) {
		Block* var9 = var1->getBlockType();
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		float var10 = 2.0F / 3.0F;
		float var12;
		if(var9 == Block::s_signPost) {
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2 + 0.5F, (float)var4 + 12.0F / 16.0F * var10, (float)var6 + 0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			float var11 = (float)(var1->getBlockMetadata() * 360) / 16.0F;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-var11)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			m_signModel->m_field_1345_b->m_field_1403_h = true;
		} else {
			int var16 = var1->getBlockMetadata();
			var12 = 0.0F;
			if(var16 == 2) {
				var12 = 180.0F;
			}

			if(var16 == 4) {
				var12 = 90.0F;
			}

			if(var16 == 5) {
				var12 = -90.0F;
			}

   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2 + 0.5F, (float)var4 + 12.0F / 16.0F * var10, (float)var6 + 0.5F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-var12)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, -(5.0F / 16.0F), -(7.0F / 16.0F));
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			m_signModel->m_field_1345_b->m_field_1403_h = false;
		}

		bindTextureByName("/item/sign.png");
  _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var10, -var10, -var10);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		m_signModel->func_887_a();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		FontRenderer* var17 = getFontRenderer();
		var12 = (float)(1.0f / 60.0f) * var10;
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.5F * var10, 0.07F * var10);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var12, -var12, var12);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  Tesselator::getInstance().normal(0.0F, 0.0F, -1.0F * var12);
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		unsigned char var13 = 0;

		for(int var14 = 0; var14 < var1->m_signText_count; ++var14) {
			std::string var15 = var1->m_signText[var14];
			if(var14 == var1->m_lineBeingEdited) {
				var15 = "> " + toStr(var15) + " <";
				var17->drawString(var15, -var17->getStringWidth(var15) / 2, var14 * 10 - var1->m_signText_count * 5, var13);
			} else {
				var17->drawString(var15, -var17->getStringWidth(var15) / 2, var14 * 10 - var1->m_signText_count * 5, var13);
			}
		}

  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void TileEntitySignRenderer::renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8) {
		a((TileEntitySign*)var1, var2, var4, var6, var8);
}
