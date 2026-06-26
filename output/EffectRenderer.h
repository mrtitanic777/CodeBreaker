#ifndef EFFECTRENDERER_H
#define EFFECTRENDERER_H

// Auto-generated from EffectRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>
#include <vector>

class Entity;
class EntityFX;
class Random;
class RenderEngine;
class World;

class EffectRenderer {
public:
	EffectRenderer();
	EffectRenderer(World* var1, RenderEngine* var2);
	virtual ~EffectRenderer();

	virtual void addEffect(EntityFX* var1);
	virtual void updateEffects();
	virtual void renderParticles(Entity* var1, float var2);
	virtual void renderLitParticles(float var1);
	virtual void clearEffects(World* var1);
	virtual void addBlockDestroyEffects(int var1, int var2, int var3);
	virtual void addBlockHitEffects(int var1, int var2, int var3, int var4);
	virtual const std::string& getStatistics();

public: // was private
	World* worldObj;
	std::vector<EntityFX*>* m_fxLayers;
	RenderEngine* m_renderEngine;
	Random* rand;

public:  // array-length companions
	int m_fxLayers_count;

};

#endif // EFFECTRENDERER_H