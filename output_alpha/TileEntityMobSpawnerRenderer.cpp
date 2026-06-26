#include "stdafx.h"
// Auto-generated from TileEntityMobSpawnerRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntityMobSpawnerRenderer.h"
#include "Common.h"
#include "Map.h"
#include "TileEntity.h"
#include "TileEntityMobSpawner.h"
#include "Entity.h"
#include "Shaders.h"
#include "World.h"
#include "EntityList.h"
#include "RenderManager.h"

TileEntityMobSpawnerRenderer::TileEntityMobSpawnerRenderer()
	: m_field_1412_b(new std::map<std::string, void*>())  // PORT: Java init = new HashMap (was left NULL -> crash on first render)
{
}

TileEntityMobSpawnerRenderer::~TileEntityMobSpawnerRenderer() {
	delete m_field_1412_b;
	m_field_1412_b = NULL;
}

void TileEntityMobSpawnerRenderer::a(TileEntityMobSpawner* var1, double var2, double var4, double var6, float var8) {
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2 + 0.5F, (float)var4, (float)var6 + 0.5F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		Entity* var9 = NULL;
		{
			std::map<std::string, void*>::iterator _it = m_field_1412_b->find(var1->m_entityID);
			if(_it != m_field_1412_b->end()) {
				var9 = (Entity*)_it->second;
			}
		}
		if(var9 == NULL) {
			var9 = EntityList::createEntityInWorld(var1->m_entityID, NULL);
			(*m_field_1412_b)[var1->m_entityID] = var9;
		}

		if(var9 != NULL) {
			var9->setWorld(var1->worldObj);
			float var10 = 7.0F / 16.0F;
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.4F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), ((float)(var1->m_field_830_d + (var1->m_field_831_c - var1->m_field_830_d) * (double)var8) * 10.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (-30.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, -0.4F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var10, var10, var10);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var9->setLocationAndAngles(var2, var4, var6, 0.0F, 0.0F);
			RenderManager::instance->func_853_a(var9, 0.0f, 0.0f, 0.0f, 0.0F, var8);
		}

  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void TileEntityMobSpawnerRenderer::renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8) {
		a((TileEntityMobSpawner*)var1, var2, var4, var6, var8);
}
