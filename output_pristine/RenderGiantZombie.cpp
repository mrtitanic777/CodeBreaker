#include "stdafx.h"
// Auto-generated from RenderGiantZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderGiantZombie.h"
#include "Common.h"
#include "EntityLiving.h"
#include "ModelBase.h"
#include "Shaders.h"
#include "Zombie.h"

RenderGiantZombie::RenderGiantZombie()
	: m_scale(6.0f)
{
}

RenderGiantZombie::RenderGiantZombie(ModelBase* var1, float var2, float var3)
	: RenderLiving(var1, 3.0f)
	, m_scale(6.0f)
{
}

RenderGiantZombie::~RenderGiantZombie() {
	// No dynamic resources
}

void RenderGiantZombie::preRenderCallback(EntityLiving* var1, float var2) {
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, this.m_scale, this.m_scale, this.m_scale);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}
