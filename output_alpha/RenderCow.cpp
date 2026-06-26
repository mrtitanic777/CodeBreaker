#include "stdafx.h"
// Auto-generated from RenderCow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderCow.h"
#include "Common.h"
#include "Entity.h"
#include "EntityCow.h"
#include "EntityLiving.h"
#include "ModelBase.h"
#include "RenderLiving.h"

RenderCow::RenderCow()
{
}

RenderCow::RenderCow(ModelBase* var1, float var2)
	: RenderLiving(var1, var2)
{
}

RenderCow::~RenderCow() {
	// No dynamic resources
}

void RenderCow::a(EntityCow* var1, double var2, double var4, double var6, float var8, float var9) {
		RenderLiving::a(var1, var2, var4, var6, var8, var9);
}

void RenderCow::a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityCow*)var1, var2, var4, var6, var8, var9);
}

void RenderCow::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityCow*)var1, var2, var4, var6, var8, var9);
}
