#ifndef ENTITYFALLINGSAND_H
#define ENTITYFALLINGSAND_H

// Auto-generated from EntityFallingSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class NBTTagCompound;
class World;

class EntityFallingSand : public Entity {
public:
	EntityFallingSand(World* var1);
	EntityFallingSand(World* var1, float var2, float var3, float var4, int var5);
	virtual ~EntityFallingSand();

	int blockID;
	int fallTime;

	virtual bool canBeCollidedWith();
	virtual void onUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual float getYOffsetForRender();
	virtual World* getWorld();

	static void initStatics();
};

#endif // ENTITYFALLINGSAND_H