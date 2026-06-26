#ifndef RENDERTNTPRIMED_H
#define RENDERTNTPRIMED_H

// Auto-generated from RenderTNTPrimed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;
class RenderBlocks;

class RenderTNTPrimed : public Render {
public:
	RenderTNTPrimed();
	virtual ~RenderTNTPrimed();

	virtual void doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6);

private:
	RenderBlocks* m_blockRenderer;

};

#endif // RENDERTNTPRIMED_H