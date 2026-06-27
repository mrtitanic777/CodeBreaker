#ifndef ENTITYPAINTING_H
#define ENTITYPAINTING_H

// Auto-generated from EntityPainting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"
#include "EnumArt.h"

class NBTTagCompound;
class World;

class EntityPainting : public Entity {
public:
	EntityPainting(World* var1);
	EntityPainting(World* var1, int var2, int var3, int var4, int var5);
	virtual ~EntityPainting();

	int tickCounter;
	int direction;
	int tileX;
	int tileY;
	int tileZ;
	EnumArt art;

	virtual void setDirection(int var1);
	virtual float getOffset(int var1);
	virtual void onUpdate();
	virtual bool onValidSurface();
	virtual bool canBeCollidedWith();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
};

#endif // ENTITYPAINTING_H