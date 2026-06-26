#include "stdafx.h"
// Auto-generated from RenderSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderSpider.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntitySpider.h"
#include "Tesselator.h"
#include "Textures.h"

RenderSpider::RenderSpider()
	: RenderLiving(new ModelSpider(), 1.0f)
{
		setRenderPassModel(new ModelSpider());
}

RenderSpider::~RenderSpider() {
	// No dynamic resources
}

float RenderSpider::getDeathMaxRotation(EntityLiving* var1) {
		return 180.0F;
}

bool RenderSpider::shouldRenderPass(EntityLiving* var1, int var2) {
		EntitySpider var10001 = (EntitySpider)var1;
		int var3 = var2;
		EntitySpider var5 = var10001;
		if(var3 != 0) {
			return false;
		} else if(var3 != 0) {
			return false;
		} else {
			Textures::load("game:\Content\mob\spider_eyes.png");
			float var4 = (1.0F - var5.getEntityBrightness(1.0F)) * 0.5F;
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, var4);
			return true;
		}
}
