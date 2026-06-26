#ifndef RENDERTNTPRIMED_H
#define RENDERTNTPRIMED_H

// Auto-generated from RenderTNTPrimed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;
class EntityTNTPrimed;
class RenderBlocks;

class RenderTNTPrimed : public Render {
public:
	RenderTNTPrimed();
	virtual ~RenderTNTPrimed();

	virtual void a(EntityTNTPrimed* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

public: // was private
	RenderBlocks* m_field_196_d;

};

#endif // RENDERTNTPRIMED_H