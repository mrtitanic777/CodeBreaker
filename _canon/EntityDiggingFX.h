#ifndef ENTITYDIGGINGFX_H
#define ENTITYDIGGINGFX_H

// Auto-generated from EntityDiggingFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFX.h"

class Block;
class Tessellator;
class World;

class EntityDiggingFX : public EntityFX {
public:
	EntityDiggingFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12, Block* var14);
	virtual ~EntityDiggingFX();

	Block* block;

	virtual EntityDiggingFX* applyColorMultiplier(int var1, int var2, int var3);
	virtual int getFXLayer();
	virtual void renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7);

	static void initStatics();
};

#endif // ENTITYDIGGINGFX_H