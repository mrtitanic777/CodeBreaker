#ifndef ENTITYTNTPRIMED_H
#define ENTITYTNTPRIMED_H

// Auto-generated from EntityTNTPrimed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class NBTTagCompound;
class World;

class EntityTNTPrimed : public Entity {
public:
	EntityTNTPrimed(World* var1);
	EntityTNTPrimed(World* var1, double var2, double var4, double var6);
	virtual ~EntityTNTPrimed();

	int fuse;

	virtual bool canBeCollidedWith();
	virtual void onUpdate();
	virtual void explode();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual float getYOffsetForRender();

	static void initStatics();
};

#endif // ENTITYTNTPRIMED_H