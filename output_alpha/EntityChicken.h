#ifndef ENTITYCHICKEN_H
#define ENTITYCHICKEN_H

// Auto-generated from EntityChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityAnimals.h"
#include <string>

class NBTTagCompound;
class World;

class EntityChicken : public EntityAnimals {
public:
	EntityChicken();
	EntityChicken(World* var1);
	virtual ~EntityChicken();

	bool m_field_753_a;
	float m_field_752_b;
	float m_field_758_c;
	float m_field_757_d;
	float m_field_756_e;
	float m_field_755_h;
	int m_timeUntilNextEgg;

	virtual void onLivingUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

public: // was protected
	virtual void fall(float var1);
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual int getDropItemId();

};

#endif // ENTITYCHICKEN_H