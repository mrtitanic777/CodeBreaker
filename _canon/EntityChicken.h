#ifndef ENTITYCHICKEN_H
#define ENTITYCHICKEN_H

// Auto-generated from EntityChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityAnimals.h"

class NBTTagCompound;
class World;

class EntityChicken : public EntityAnimals {
public:
	EntityChicken(World* var1);
	virtual ~EntityChicken();

	bool field_753_a;
	float wingRotation;
	float destPos;
	float prevDestPos;
	float prevWingRotation;
	float wingRotDelta;
	int timeUntilNextEgg;

	virtual void onLivingUpdate();
	virtual void fall(float var1);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual int getDropItemId();

	static void initStatics();
};

#endif // ENTITYCHICKEN_H