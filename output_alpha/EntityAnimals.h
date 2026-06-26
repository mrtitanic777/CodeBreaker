#ifndef ENTITYANIMALS_H
#define ENTITYANIMALS_H

// Auto-generated from EntityAnimals.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreature.h"

class NBTTagCompound;
class World;

class EntityAnimals : public EntityCreature {
public:
	EntityAnimals();
	EntityAnimals(World* var1);
	virtual ~EntityAnimals();

	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool getCanSpawnHere();
	virtual int func_421_b();

public: // was protected
	virtual float func_439_a(int var1, int var2, int var3);

};

#endif // ENTITYANIMALS_H