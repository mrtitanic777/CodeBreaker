#ifndef ENTITYTNTPRIMED_H
#define ENTITYTNTPRIMED_H

// Auto-generated from EntityTNTPrimed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

class NBTTagCompound;
class World;

class EntityTNTPrimed : public Entity {
public:
	EntityTNTPrimed();
	EntityTNTPrimed(World* var1);
	EntityTNTPrimed(World* var1, double var2, double var4, double var6);
	virtual ~EntityTNTPrimed();

	int m_fuse;

	virtual bool canBeCollidedWith();
	virtual void onUpdate();
	virtual float func_392_h_();

public: // was protected
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

public: // was private
	void explode();

};

#endif // ENTITYTNTPRIMED_H