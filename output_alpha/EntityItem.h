#ifndef ENTITYITEM_H
#define ENTITYITEM_H

// Auto-generated from EntityItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

class EntityPlayer;
class ItemStack;
class NBTTagCompound;
class World;

class EntityItem : public Entity {
public:
	EntityItem();
	EntityItem(World* var1, double var2, double var4, double var6, ItemStack* var8);
	EntityItem(World* var1);
	virtual ~EntityItem();

	ItemStack* m_item;
	int m_age;
	int m_field_805_c;
	float m_field_804_d;

	virtual void onUpdate();
	virtual bool handleWaterMovement();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void onCollideWithPlayer(EntityPlayer* var1);

public: // was protected
	virtual void func_355_a(int var1);

public: // was private
	int m_field_803_e;
	int health;

	bool func_466_g(double var1, double var3, double var5);

};

#endif // ENTITYITEM_H