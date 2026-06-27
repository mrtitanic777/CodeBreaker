#ifndef EFFECTRENDERER_H
#define EFFECTRENDERER_H

// Auto-generated from EffectRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Entity;
class EntityFX;
class Random;
class RenderEngine;
class World;

class EffectRenderer {
public:
	EffectRenderer(World* var1, RenderEngine* var2);
	virtual ~EffectRenderer();

	World* worldObj;
	JArray<JList<void*> > fxLayers;
	RenderEngine* renderer;
	Random* rand;

	virtual void addEffect(EntityFX* var1);
	virtual void updateEffects();
	virtual void renderParticles(Entity* var1, float var2);
	virtual void renderLitParticles(Entity* var1, float var2);
	virtual void clearEffects(World* var1);
	virtual void addBlockDestroyEffects(int var1, int var2, int var3);
	virtual void addBlockHitEffects(int var1, int var2, int var3, int var4);
	virtual JavaString getStatistics();

	static void initStatics();
};

#endif // EFFECTRENDERER_H