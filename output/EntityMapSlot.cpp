#include "stdafx.h"
// Auto-generated from EntityMapSlot.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMapSlot.h"
#include "Common.h"
#include "Entity.h"
#include "EntityMap.h"
#include "Slot.h"

EntityMapSlot::EntityMapSlot()
	: m_xSlot(0)
	, m_ySlot(0)
	, m_zSlot(0)
	, m_entityMap(NULL)
{
}

EntityMapSlot::EntityMapSlot(EntityMap* var1, unsigned char var2)
	: m_xSlot(0)
	, m_ySlot(0)
	, m_zSlot(0)
	, m_entityMap(NULL)
{
		m_entityMap = var1;
}

EntityMapSlot::EntityMapSlot(EntityMap* var1)
	: m_xSlot(0)
	, m_ySlot(0)
	, m_zSlot(0)
	, m_entityMap(NULL)
{
  // PORT: constructor delegation  // was: this(var1, (unsigned char)0);
}

EntityMapSlot::~EntityMapSlot() {
	m_entityMap = NULL;
}

EntityMapSlot* EntityMapSlot::init(float var1, float var2, float var3) {
		m_xSlot = (int)(var1 / 8.0F);
		m_ySlot = (int)(var2 / 8.0F);
		m_zSlot = (int)(var3 / 8.0F);
		if(m_xSlot < 0) {
			m_xSlot = 0;
		}

		if(m_ySlot < 0) {
			m_ySlot = 0;
		}

		if(m_zSlot < 0) {
			m_zSlot = 0;
		}

		if(m_xSlot >= m_entityMap->width) {
			m_xSlot = m_entityMap->width - 1;
		}

		if(m_ySlot >= m_entityMap->m_depth) {
			m_ySlot = m_entityMap->m_depth - 1;
		}

		if(m_zSlot >= m_entityMap->height) {
			m_zSlot = m_entityMap->height - 1;
		}

		return this;
}

void EntityMapSlot::a(Entity* var1) {
		if(m_xSlot >= 0 && m_ySlot >= 0 && m_zSlot >= 0) {
			m_entityMap->m_entityGrid[(m_zSlot * m_entityMap->m_depth + m_ySlot) * m_entityMap->width + m_xSlot].push_back(var1);
		}

}

void EntityMapSlot::b(Entity* var1) {
		if(m_xSlot >= 0 && m_ySlot >= 0 && m_zSlot >= 0) {
			_vecRemoveObj(m_entityMap->m_entityGrid[(m_zSlot * m_entityMap->m_depth + m_ySlot) * m_entityMap->width + m_xSlot], var1);
		}

}

int EntityMapSlot::a() {
		return m_xSlot;
}

int EntityMapSlot::b() {
		return m_ySlot;
}

int EntityMapSlot::c() {
		return m_zSlot;
}
