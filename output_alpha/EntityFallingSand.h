#ifndef ENTITYFALLINGSAND_H
#define ENTITYFALLINGSAND_H

// Auto-generated from EntityFallingSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

class NBTTagCompound;
class World;

class EntityFallingSand : public Entity {
public:
	EntityFallingSand();
	EntityFallingSand(World* var1);
	EntityFallingSand(World* var1, float var2, float var3, float var4, int var5);
	virtual ~EntityFallingSand();

	int m_field_799_a;
	int m_field_798_b;

	virtual bool canBeCollidedWith();
	virtual void onUpdate();
	virtual float func_392_h_();
	virtual World* func_465_i();

public: // was protected
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

};

#endif // ENTITYFALLINGSAND_H