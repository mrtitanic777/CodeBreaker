#ifndef ENTITYANIMAL_H
#define ENTITYANIMAL_H

// Auto-generated from EntityAnimal.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreature.h"

class NBTTagCompound;
class World;

class EntityAnimal : public EntityCreature {
public:
	EntityAnimal();
	EntityAnimal(World* var1);
	virtual ~EntityAnimal();

	virtual bool getCanSpawnHere(float var1, float var2, float var3);

protected:
	virtual float getBlockPathWeight(int var1, int var2, int var3);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

};

#endif // ENTITYANIMAL_H