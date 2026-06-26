#include "stdafx.h"
// Auto-generated from EntitySorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySorter.h"
#include "Common.h"
#include "Entity.h"
#include "WorldRenderer.h"

EntitySorter::EntitySorter()
	: m_field_1594_a(NULL)
{
}

EntitySorter::EntitySorter(Entity* var1)
	: m_field_1594_a(NULL)
{
		m_field_1594_a = var1;
}

EntitySorter::~EntitySorter() {
	m_field_1594_a = NULL;
}

int EntitySorter::a(WorldRenderer* var1, WorldRenderer* var2) {
		return var1->func_1202_a(m_field_1594_a) < var2->func_1202_a(m_field_1594_a) ? -1 : 1;
}

int EntitySorter::compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2) {
		return a((WorldRenderer*)var1, (WorldRenderer*)var2);
}
