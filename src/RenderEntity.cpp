#include "stdafx.h"
// Auto-generated from RenderEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderEntity.h"
#include "Common.h"
#include "Entity.h"
#include "Shaders.h"

RenderEntity::RenderEntity()
{
}

RenderEntity::~RenderEntity() {
	// No dynamic resources
}

void RenderEntity::doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var2 - var1->lastTickPosX, var3 - var1->lastTickPosY, var4 - var1->lastTickPosZ);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		renderOffsetAABB(var1->boundingBox);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}
