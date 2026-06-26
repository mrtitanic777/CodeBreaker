#ifndef ENTITYBUBBLEFX_H
#define ENTITYBUBBLEFX_H

// Auto-generated from EntityBubbleFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class World;

class EntityBubbleFX : public EntityFX {
public:
	EntityBubbleFX();
	EntityBubbleFX(World* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual ~EntityBubbleFX();

	virtual void onEntityUpdate();

};

#endif // ENTITYBUBBLEFX_H