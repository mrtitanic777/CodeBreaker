#ifndef RENDERBOAT_H
#define RENDERBOAT_H

// Auto-generated from RenderBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Render.h"

class Entity;
class EntityBoat;
class ModelBase;

class RenderBoat : public Render {
public:
	RenderBoat();
	virtual ~RenderBoat();

	ModelBase* modelBoat;

	virtual void a(EntityBoat* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	RenderBoat(const RenderBoat&);
	RenderBoat& operator=(const RenderBoat&);
};

#endif // RENDERBOAT_H