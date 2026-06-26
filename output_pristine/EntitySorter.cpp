#include "stdafx.h"
// Auto-generated from EntitySorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySorter.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "WorldRenderer.h"

EntitySorter::EntitySorter()
	: m_player(NULL)
{
}

EntitySorter::EntitySorter(EntityPlayer* var1)
	: m_player(NULL)
{
		m_player = var1;
}

EntitySorter::~EntitySorter() {
	m_player = NULL;
}

int EntitySorter::compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2) {
		WorldRenderer var10001 = (WorldRenderer)var1;
		WorldRenderer var3 = (WorldRenderer)var2;
		WorldRenderer var4 = var10001;
		return var4.distanceToEntitySquared(m_player) < var3.distanceToEntitySquared(m_player) ? -1 : 1;
}
