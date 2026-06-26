#ifndef ENTITYRAINFX_H
#define ENTITYRAINFX_H

// Auto-generated from EntityRainFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Tesselator;
class World;

class EntityRainFX : public EntityFX {
public:
	EntityRainFX();
	EntityRainFX(World* var1, double var2, double var4, double var6);
	virtual ~EntityRainFX();

	virtual void func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onUpdate();

};

#endif // ENTITYRAINFX_H