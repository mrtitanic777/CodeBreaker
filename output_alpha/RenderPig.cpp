#include "stdafx.h"
// Auto-generated from RenderPig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderPig.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntityPig.h"
#include "ModelBase.h"
#include "Textures.h"

RenderPig::RenderPig()
{
}

RenderPig::RenderPig(ModelBase* var1, ModelBase* var2, float var3)
	: RenderLiving(var1, var3)
{
		func_4013_a(var2);
}

RenderPig::~RenderPig() {
	// No dynamic resources
}

bool RenderPig::a(EntityPig* var1, int var2) {
		Textures::load("game:\\Content\\mob\\saddle.png");
		return var2 == 0 && var1->m_rideable;
}

bool RenderPig::func_166_a(EntityLiving* var1, int var2) {
		return a((EntityPig*)var1, var2);
}
