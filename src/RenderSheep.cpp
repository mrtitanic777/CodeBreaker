#include "stdafx.h"
// Auto-generated from RenderSheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderSheep.h"
#include "Common.h"
#include "EntityLiving.h"
#include "ModelBase.h"
#include "EntitySheep.h"
#include "Tesselator.h"
#include "Textures.h"

RenderSheep::RenderSheep()
{
}

RenderSheep::RenderSheep(ModelBase* var1, ModelBase* var2, float var3)
	: RenderLiving(var1, 0.7f)
{
		setRenderPassModel(var2);
}

RenderSheep::~RenderSheep() {
	// No dynamic resources
}

bool RenderSheep::shouldRenderPass(EntityLiving* var1, int var2) {
		EntitySheep var10001 = (EntitySheep*)var1;
		int var3 = var2;
		EntitySheep var4 = var10001;
		Textures::load("game:\Content\mob\sheep_fur.png");
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		return var3 == 0 && !var4.m_sheared;
}
