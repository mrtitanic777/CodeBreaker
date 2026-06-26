#include "stdafx.h"
// Auto-generated from RenderChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderChicken.h"
#include "Common.h"
#include "Entity.h"
#include "EntityChicken.h"
#include "EntityLiving.h"
#include "ModelBase.h"
#include "RenderLiving.h"

RenderChicken::RenderChicken()
{
}

RenderChicken::RenderChicken(ModelBase* var1, float var2)
	: RenderLiving(var1, var2)
{
}

RenderChicken::~RenderChicken() {
	// No dynamic resources
}

void RenderChicken::a(EntityChicken* var1, double var2, double var4, double var6, float var8, float var9) {
		RenderLiving::a(var1, var2, var4, var6, var8, var9);
}

float RenderChicken::a(EntityChicken* var1, float var2) {
		float var3 = var1->m_field_756_e + (var1->m_field_752_b - var1->m_field_756_e) * var2;
		float var4 = var1->m_field_757_d + (var1->m_field_758_c - var1->m_field_757_d) * var2;
		return (sinf(var3) + 1.0F) * var4;
}

float RenderChicken::func_170_d(EntityLiving* var1, float var2) {
		return a((EntityChicken*)var1, var2);
}

void RenderChicken::a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityChicken*)var1, var2, var4, var6, var8, var9);
}

void RenderChicken::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
		a((EntityChicken*)var1, var2, var4, var6, var8, var9);
}
