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

void RenderEntity::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		renderOffsetAABB(var1->boundingBox, var2 - var1->lastTickPosX, var4 - var1->lastTickPosY, var6 - var1->lastTickPosZ);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}
