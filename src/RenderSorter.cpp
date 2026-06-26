#include "stdafx.h"
// Auto-generated from RenderSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderSorter.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "WorldRenderer.h"

RenderSorter::RenderSorter()
	: m_baseEntity(NULL)
{
}

RenderSorter::RenderSorter(EntityPlayer* var1)
	: m_baseEntity(NULL)
{
		m_baseEntity = var1;
}

RenderSorter::~RenderSorter() {
	m_baseEntity = NULL;
}

int RenderSorter::compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2) {
		WorldRenderer var10001 = (WorldRenderer*)var1;
		WorldRenderer var3 = (WorldRenderer*)var2;
		WorldRenderer var6 = var10001;
		bool var4 = var6.isInFrustrum;
		bool var5 = var3.isInFrustrum;
		return var4 && !var5 ? 1 : ((!var5 || var4) && var6.distanceToEntitySquared(m_baseEntity) < var3.distanceToEntitySquared(m_baseEntity) ? 1 : -1);
}
