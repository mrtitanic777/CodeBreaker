#ifndef ENTITYANIMALS_H
#define ENTITYANIMALS_H

// Auto-generated from EntityAnimals.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityCreature.h"

class NBTTagCompound;
class World;

class EntityAnimals : public EntityCreature {
public:
	EntityAnimals(World* var1);
	virtual ~EntityAnimals();

	virtual float getBlockPathWeight(int var1, int var2, int var3);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool getCanSpawnHere();
	virtual int getTalkInterval();

	static void initStatics();
};

#endif // ENTITYANIMALS_H