#ifndef ENTITYBUBBLEFX_H
#define ENTITYBUBBLEFX_H

// Auto-generated from EntityBubbleFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFX.h"

class World;

class EntityBubbleFX : public EntityFX {
public:
	EntityBubbleFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual ~EntityBubbleFX();

	virtual void onUpdate();

	static void initStatics();
};

#endif // ENTITYBUBBLEFX_H