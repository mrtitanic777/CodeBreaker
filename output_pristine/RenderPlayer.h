#ifndef RENDERPLAYER_H
#define RENDERPLAYER_H

// Auto-generated from RenderPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;

class RenderPlayer : public RenderLiving {
public:
	RenderPlayer();
	virtual ~RenderPlayer();


protected:
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);

};

#endif // RENDERPLAYER_H