#include "stdafx.h"
// Auto-generated from EntityMap.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMap.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "EntityMapSlot.h"
#include <algorithm>
#include "Slot.h"

EntityMap::EntityMap()
	: width(0)
	, m_depth(0)
	, height(0)
	, m_slot(new EntityMapSlot(this))
	, m_slot2(new EntityMapSlot(this))
	, m_entityGrid(NULL)
{
}

EntityMap::EntityMap(int var1, int var2, int var3)
	: width(0)
	, m_depth(0)
	, height(0)
	, m_slot(new EntityMapSlot(this))
	, m_slot2(new EntityMapSlot(this))
	, m_entityGrid(NULL)
{
		width = var1 / 8;
		m_depth = var2 / 8;
		height = var3 / 8;
		if(width == 0) {
			width = 1;
		}

		if(m_depth == 0) {
			m_depth = 1;
		}

		if(height == 0) {
			height = 1;
		}

		m_entityGrid = new std::vector<Entity*> /* PORT: type */[width * m_depth * height];

		for(var1 = 0; var1 < width; ++var1) {
			for(var2 = 0; var2 < m_depth; ++var2) {
				for(var3 = 0; var3 < height; ++var3) {
					m_entityGrid[(var3 * m_depth + var2) * width + var1].clear();
				}
			}
		}

}

EntityMap::~EntityMap() {
	m_slot = NULL;
	m_slot2 = NULL;
	delete[] m_entityGrid;
	m_entityGrid = NULL;
}

void EntityMap::insert(Entity* var1) {
		m_entities.push_back(var1);
		m_slot->init(var1->posX, var1->posY, var1->posZ)->a(var1);
		var1->lastTickPosX = var1->posX;
		var1->lastTickPosY = var1->posY;
		var1->lastTickPosZ = var1->posZ;
}

void EntityMap::remove(Entity* var1) {
		m_slot->init(var1->lastTickPosX, var1->lastTickPosY, var1->lastTickPosZ)->b(var1);
		m_slot->init(var1->posX, var1->posY, var1->posZ)->b(var1);
  std::vector<Entity*>::iterator _removeIt = std::find(m_entities.begin(), m_entities.end(), var1);
  if (_removeIt != m_entities.end()) m_entities.erase(_removeIt);
}

std::vector<Entity*> /* PORT: determine element type */ EntityMap::a(Entity* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
  m_entitiesExcludingEntity.clear();
		return a(var1, var2, var3, var4, var5, var6, var7, m_entitiesExcludingEntity);
}

std::vector<Entity*> /* PORT: determine element type */ EntityMap::a(Entity* var1, float var2, float var3, float var4, float var5, float var6, float var7, std::vector<Entity*> /* PORT: determine element type */ var8) {
		EntityMapSlot* var9 = m_slot->init(var2, var3, var4);
		EntityMapSlot* var10 = m_slot2->init(var5, var6, var7);

		for(int var11 = var9->a() - 1; var11 <= var10->a() + 1; ++var11) {
			for(int var12 = var9->b() - 1; var12 <= var10->b() + 1; ++var12) {
				for(int var13 = var9->c() - 1; var13 <= var10->c() + 1; ++var13) {
					if(var11 >= 0 && var12 >= 0 && var13 >= 0 && var11 < width && var12 < m_depth && var13 < height) {
						std::vector<Entity*> var14 = m_entityGrid[(var13 * m_depth + var12) * width + var11];

						for(size_t var15 = 0; var15 < var14.size(); ++var15) {
							Entity* var16 = (Entity*)var14[var15];
							if(var16 != var1) {
								AxisAlignedBB var17 = var16->boundingBox;
								if(var5 > var17.minX && var2 < var17.maxX ? (var6 > var17.minY && var3 < var17.maxY ? var7 > var17.minZ && var4 < var17.maxZ : false) : false) {
									var8.push_back(var16);
								}
							}
						}
					}
				}
			}
		}

		return var8;
}

std::vector<Entity*> /* PORT: determine element type */ EntityMap::a(Entity* var1, AxisAlignedBB var2) {
  m_entitiesExcludingEntity.clear();
		return false ? m_entitiesExcludingEntity : a(var1, var2.minX, var2.minY, var2.minZ, var2.maxX, var2.maxY, var2.maxZ, m_entitiesExcludingEntity);
}

void EntityMap::updateEntities() {
		for(size_t var1 = 0; var1 < m_entities.size(); ++var1) {
			Entity* var2 = (Entity*)m_entities[var1];
			var2->lastTickPosX = var2->posX;
			var2->lastTickPosY = var2->posY;
			var2->lastTickPosZ = var2->posZ;
			var2->onEntityUpdate();
			if(var2->isDead) {
				_vecRemoveAt(m_entities, var1--);
				m_slot->init(var2->lastTickPosX, var2->lastTickPosY, var2->lastTickPosZ)->b(var2);
			} else {
				int var3 = (int)(var2->lastTickPosX / 8.0F);
				int var4 = (int)(var2->lastTickPosY / 8.0F);
				int var5 = (int)(var2->lastTickPosZ / 8.0F);
				int var6 = (int)(var2->posX / 8.0F);
				int var7 = (int)(var2->posY / 8.0F);
				int var8 = (int)(var2->posZ / 8.0F);
				if(var3 != var6 || var4 != var7 || var5 != var8) {
					EntityMapSlot* var11 = m_slot->init(var2->lastTickPosX, var2->lastTickPosY, var2->lastTickPosZ);
					EntityMapSlot* var9 = m_slot2->init(var2->posX, var2->posY, var2->posZ);
					if(var11 != var9) {
						var11->b(var2);
						var9->a(var2);
					}
				}
			}
		}

}
