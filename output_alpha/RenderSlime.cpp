#include "stdafx.h"
// Auto-generated from RenderSlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderSlime.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntitySlime.h"
#include "ModelBase.h"
#include "Shaders.h"
#include "Tesselator.h"

RenderSlime::RenderSlime()
	: m_field_205_f(NULL)
{
}

RenderSlime::RenderSlime(ModelBase* var1, ModelBase* var2, float var3)
	: RenderLiving(var1, var3)
	, m_field_205_f(NULL)
{
		m_field_205_f = var2;
}

RenderSlime::~RenderSlime() {
	m_field_205_f = NULL;
}

bool RenderSlime::a(EntitySlime* var1, int var2) {
		if(var2 == 0) {
			func_4013_a(m_field_205_f);
   // Normalize is shader-based on Xbox 360 (enabled)
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			return true;
		} else {
			if(var2 == 1) {
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			}

			return false;
		}
}

void RenderSlime::a(EntitySlime* var1, float var2) {
		float var3 = (var1->m_field_767_b + (var1->m_field_768_a - var1->m_field_767_b) * var2) / ((float)var1->m_field_770_c * 0.5F + 1.0F);
		float var4 = 1.0F / (var3 + 1.0F);
		float var5 = (float)var1->m_field_770_c;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var4 * var5, 1.0F / var4 * var5, var4 * var5);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}

void RenderSlime::func_6330_a(EntityLiving* var1, float var2) {
		a((EntitySlime*)var1, var2);
}

bool RenderSlime::func_166_a(EntityLiving* var1, int var2) {
		return a((EntitySlime*)var1, var2);
}
