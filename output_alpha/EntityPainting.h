#ifndef ENTITYPAINTING_H
#define ENTITYPAINTING_H

// Auto-generated from EntityPainting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

struct EnumArt;
class NBTTagCompound;
class World;

class EntityPainting : public Entity {
public:
	EntityPainting();
	EntityPainting(World* var1);
	EntityPainting(World* var1, int var2, int var3, int var4, int var5);
	virtual ~EntityPainting();

	int m_field_691_a;
	EnumArt* m_field_690_b;

	virtual void func_412_b(int var1);
	virtual void onUpdate();
	virtual bool func_410_i();
	virtual bool canBeCollidedWith();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

public: // was private
	int m_field_695_c;
	int m_field_9322_d;
	int m_field_9321_e;
	int m_field_9320_f;

	float func_411_c(int var1);

};

#endif // ENTITYPAINTING_H