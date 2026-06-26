#include "stdafx.h"
// Auto-generated from RenderFallingSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderFallingSand.h"
#include "Common.h"
#include "Entity.h"
#include "EntityFallingSand.h"
#include "RenderBlocks.h"
#include "Block.h"
#include "Shaders.h"
#include "Textures.h"
#include "World.h"

RenderFallingSand::RenderFallingSand()
	: m_field_197_d(new RenderBlocks())
{
		m_field_9246_c = 0.5F;
}

RenderFallingSand::~RenderFallingSand() {
	m_field_197_d = NULL;
}

void RenderFallingSand::a(EntityFallingSand* var1, double var2, double var4, double var6, float var8, float var9) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		Textures::load("game:\\Content\\terrain.png");
		Block* var10 = Block::blocksList[var1->m_field_799_a];
		World* var11 = var1->func_465_i();
  // Lighting is shader-based on Xbox 360 (disabled)
		m_field_197_d->func_1243_a(var10, var11, (int)floor(var1->posX), (int)floor(var1->posY), (int)floor(var1->posZ));
  // Lighting is shader-based on Xbox 360 (enabled)
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void RenderFallingSand::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityFallingSand*)var1, var2, var4, var6, var8, var9);
}
