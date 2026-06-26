#include "stdafx.h"
// Auto-generated from RenderPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderPlayer.h"
#include "Common.h"
#include "EntityLiving.h"
#include "ModelBiped.h"
#include "Player.h"

RenderPlayer::RenderPlayer()
	: RenderLiving(new ModelBiped(), 0.5f)
{
		new ModelBiped(1.0F);
		new ModelBiped(0.5F);
}

RenderPlayer::~RenderPlayer() {
	// No dynamic resources
}

bool RenderPlayer::shouldRenderPass(EntityLiving* var1, int var2) {
		return false;
}
