#include "stdafx.h"
// Auto-generated from RenderBiped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderBiped.h"
#include "Common.h"
#include "EntityLiving.h"
#include "ModelBiped.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"
#include "RenderBlocks.h"
#include "Shaders.h"
#include "ModelRenderer.h"
#include "RenderManager.h"
#include "ItemRenderer.h"

RenderBiped::RenderBiped()
	: m_field_4013_a(NULL)
{
}

RenderBiped::RenderBiped(ModelBiped* var1, float var2)
	: RenderLiving(var1, var2)
	, m_field_4013_a(NULL)
{
		m_field_4013_a = var1;
}

RenderBiped::~RenderBiped() {
	m_field_4013_a = NULL;
}

void RenderBiped::func_6331_b(EntityLiving* var1, float var2) {
		ItemStack* var3 = var1->getHeldItem();
		if(var3 != NULL) {
   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			m_field_4013_a->m_bipedRightArm->func_926_b(1.0F / 16.0F);
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -(1.0F / 16.0F), 7.0F / 16.0F, 1.0F / 16.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			float var4;
			if(var3->itemID < 256 && RenderBlocks::func_1219_a(Block::blocksList[var3->itemID]->getRenderType())) {
				var4 = 0.5F;
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 3.0F / 16.0F, -(5.0F / 16.0F));
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				var4 *= 12.0F / 16.0F;
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (20.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (45.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var4, -var4, var4);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			} else if(Item::itemsList[var3->itemID]->isFull3D()) {
				var4 = 10.0F / 16.0F;
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 3.0F / 16.0F, 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var4, -var4, var4);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (-100.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (45.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			} else {
				var4 = 6.0F / 16.0F;
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.25F, 3.0F / 16.0F, -(3.0F / 16.0F));
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var4, var4, var4);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (60.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (-90.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
    { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (20.0F)*DEG_TO_RAD);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

			renderManager->m_field_4236_f->renderItem(var3);
   Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		}

}
