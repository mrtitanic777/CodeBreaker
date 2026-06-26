#include "stdafx.h"
// Auto-generated from RenderSheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderSheep.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntitySheep.h"
#include "ModelBase.h"
#include "Textures.h"

RenderSheep::RenderSheep()
{
}

RenderSheep::RenderSheep(ModelBase* var1, ModelBase* var2, float var3)
	: RenderLiving(var1, var3)
{
		func_4013_a(var2);
}

RenderSheep::~RenderSheep() {
	// No dynamic resources
}

bool RenderSheep::a(EntitySheep* var1, int var2) {
		Textures::load("game:\\Content\\mob\\sheep_fur.png");
		return var2 == 0 && !var1->m_sheared;
}

bool RenderSheep::func_166_a(EntityLiving* var1, int var2) {
		return a((EntitySheep*)var1, var2);
}
