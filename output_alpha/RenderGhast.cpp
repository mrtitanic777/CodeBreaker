#include "stdafx.h"
// Auto-generated from RenderGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderGhast.h"
#include "Common.h"
#include "EntityGhast.h"
#include "EntityLiving.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "ModelGhast.h"

RenderGhast::RenderGhast()
	: RenderLiving(new ModelGhast(), 0.5f)
{
}

RenderGhast::~RenderGhast() {
	// No dynamic resources
}

void RenderGhast::a(EntityGhast* var1, float var2) {
		float var4 = ((float)var1->m_field_4125_e + (float)(var1->m_field_4124_f - var1->m_field_4125_e) * var2) / 20.0F;
		if(var4 < 0.0F) {
			var4 = 0.0F;
		}

		var4 = 1.0F / (var4 * var4 * var4 * var4 * var4 * 2.0F + 1.0F);
		float var5 = (8.0F + var4) / 2.0F;
		float var6 = (8.0F + 1.0F / var4) / 2.0F;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var6, var5, var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
}

void RenderGhast::func_6330_a(EntityLiving* var1, float var2) {
		a((EntityGhast*)var1, var2);
}
