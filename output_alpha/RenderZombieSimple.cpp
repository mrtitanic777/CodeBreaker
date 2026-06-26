#include "stdafx.h"
// Auto-generated from RenderZombieSimple.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderZombieSimple.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntityZombieSimple.h"
#include "ModelBase.h"
#include "Shaders.h"

RenderZombieSimple::RenderZombieSimple()
	: m_field_204_f(0)
{
}

RenderZombieSimple::RenderZombieSimple(ModelBase* var1, float var2, float var3)
	: RenderLiving(var1, var2 * var3)
	, m_field_204_f(0)
{
		m_field_204_f = var3;
}

RenderZombieSimple::~RenderZombieSimple() {
	// No dynamic resources
}

void RenderZombieSimple::a(EntityZombieSimple* var1, float var2) {
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, this->m_field_204_f, this->m_field_204_f, this->m_field_204_f);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}

void RenderZombieSimple::func_6330_a(EntityLiving* var1, float var2) {
		a((EntityZombieSimple*)var1, var2);
}
