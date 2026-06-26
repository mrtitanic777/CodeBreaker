#ifndef RENDERARROW_H
#define RENDERARROW_H

// Auto-generated from RenderArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;

class RenderArrow : public Render {
public:
	RenderArrow();
	virtual ~RenderArrow();

	virtual void doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6);

};

#endif // RENDERARROW_H