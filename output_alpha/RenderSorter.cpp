#include "stdafx.h"
// Auto-generated from RenderSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderSorter.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "WorldRenderer.h"

RenderSorter::RenderSorter()
	: m_field_4274_a(NULL)
{
}

RenderSorter::RenderSorter(EntityPlayer* var1)
	: m_field_4274_a(NULL)
{
		m_field_4274_a = var1;
}

RenderSorter::~RenderSorter() {
	m_field_4274_a = NULL;
}

int RenderSorter::a(WorldRenderer* var1, WorldRenderer* var2) {
		bool var3 = var1->m_field_1749_o;
		bool var4 = var2->m_field_1749_o;
		if(var3 && !var4) {
			return 1;
		} else if(var4 && !var3) {
			return -1;
		} else {
			double var5 = (double)var1->func_1202_a(m_field_4274_a);
			double var7 = (double)var2->func_1202_a(m_field_4274_a);
			return var5 < var7 ? 1 : (var5 > var7 ? -1 : (var1->m_field_1735_w < var2->m_field_1735_w ? 1 : -1));
		}
}

int RenderSorter::compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2) {
		return a((WorldRenderer*)var1, (WorldRenderer*)var2);
}
