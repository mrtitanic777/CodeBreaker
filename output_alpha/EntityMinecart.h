#ifndef ENTITYMINECART_H
#define ENTITYMINECART_H

// Auto-generated from EntityMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include "IInventory.h"
#include <string>

struct AxisAlignedBB;
class EntityPlayer;
class ItemStack;
class NBTTagCompound;
class Vec3D;
class World;

class EntityMinecart : public Entity, public IInventory {
public:
	EntityMinecart();
	EntityMinecart(World* var1);
	EntityMinecart(World* var1, double var2, double var4, double var6, int var8);
	virtual ~EntityMinecart();

	int m_a;
	int m_b;
	int m_c;
	int m_d;
	int m_fuel;
	double m_pushX;
	double m_pushZ;

	virtual AxisAlignedBB func_383_b_(Entity* var1);
	virtual AxisAlignedBB func_372_f_();
	virtual bool canBePushed();
	virtual double func_402_h();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void func_9280_g();
	virtual bool canBeCollidedWith();
	virtual void setEntityDead();
	virtual void onUpdate();
	virtual Vec3D* func_515_a(double var1, double var3, double var5, double var7);
	virtual Vec3D* func_514_g(double var1, double var3, double var5);
	virtual float func_392_h_();
	virtual void applyEntityCollision(Entity* var1);
	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual const std::string& getInvName();
	virtual int getInventoryStackLimit();
	virtual void onInventoryChanged();
	virtual bool interact(EntityPlayer* var1);
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void setVelocity(double var1, double var3, double var5);

public: // was protected
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

public: // was private
	ItemStack** m_cargoItems;
	bool m_field_856_i;
	static int*** s_field_855_j;
	int m_field_9415_k;
	double m_field_9414_l;
	double m_field_9413_m;
	double m_field_9412_n;
	double m_field_9411_o;
	double m_field_9410_p;
	double m_field_9409_q;
	double m_field_9408_r;
	double m_field_9407_s;

public:  // array-length companions
	int m_cargoItems_count;

};

#endif // ENTITYMINECART_H