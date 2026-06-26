#ifndef ENTITYDIGGINGFX_H
#define ENTITYDIGGINGFX_H

// Auto-generated from EntityDiggingFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Block;
class Tesselator;
class World;

class EntityDiggingFX : public EntityFX {
public:
	EntityDiggingFX();
	EntityDiggingFX(World* var1, float var2, float var3, float var4, float var5, float var6, float var7, Block* var8);
	virtual ~EntityDiggingFX();

	virtual int getFXLayer();
	virtual void renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);

};

#endif // ENTITYDIGGINGFX_H