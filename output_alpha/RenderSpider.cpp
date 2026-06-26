#include "stdafx.h"
// Auto-generated from RenderSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderSpider.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntitySpider.h"
#include "Tesselator.h"
#include "Textures.h"
#include "ModelSpider.h"

RenderSpider::RenderSpider()
	: RenderLiving(new ModelSpider(), 1.0f)
{
		func_4013_a(new ModelSpider());
}

RenderSpider::~RenderSpider() {
	// No dynamic resources
}

float RenderSpider::a(EntitySpider* var1) {
		return 180.0F;
}

bool RenderSpider::a(EntitySpider* var1, int var2) {
		if(var2 != 0) {
			return false;
		} else if(var2 != 0) {
			return false;
		} else {
			Textures::load("game:\\Content\\mob\\spider_eyes.png");
			float var3 = (1.0F - var1->getEntityBrightness(1.0F)) * 0.5F;
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, var3);
			return true;
		}
}

float RenderSpider::func_172_a(EntityLiving* var1) {
		return a((EntitySpider*)var1);
}

bool RenderSpider::func_166_a(EntityLiving* var1, int var2) {
		return a((EntitySpider*)var1, var2);
}
