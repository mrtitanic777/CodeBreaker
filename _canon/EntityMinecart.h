#ifndef ENTITYMINECART_H
#define ENTITYMINECART_H

// Auto-generated from EntityMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"
#include "IInventory.h"

class AxisAlignedBB;
class EntityPlayer;
class ItemStack;
class NBTTagCompound;
class Vec3D;
class World;

class EntityMinecart : public Entity, public IInventory {
public:
	EntityMinecart(World* var1);
	EntityMinecart(World* var1, double var2, double var4, double var6, int var8);
	virtual ~EntityMinecart();

	JArray<ItemStack* > cargoItems;
	int a;
	int b;
	int c;
	bool isInReverse;
	int d;
	int fuel;
	double pushX;
	double pushZ;
	int turnProgress;
	double minecartX;
	double minecartY;
	double minecartZ;
	double minecartYaw;
	double minecartPitch;
	double velocityX;
	double velocityY;
	double velocityZ;
	static JArray<JArray<JArray<int > > > s_railDirectionMatrix;

	virtual AxisAlignedBB* getBoundingBox(Entity* var1);
	virtual AxisAlignedBB* getCollisionBox();
	virtual bool canBePushed();
	virtual double getMountedYOffset();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void performHurtAnimation();
	virtual bool canBeCollidedWith();
	virtual void setEntityDead();
	virtual void onUpdate();
	virtual Vec3D* getPosOffsetFromRail(double var1, double var3, double var5, double var7);
	virtual Vec3D* getPosOnRail(double var1, double var3, double var5);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual float getYOffsetForRender();
	virtual void applyEntityCollision(Entity* var1);
	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual JavaString getInvName();
	virtual int getInventoryStackLimit();
	virtual void onInventoryChanged();
	virtual bool interact(EntityPlayer* var1);
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void setVelocity(double var1, double var3, double var5);
};

#endif // ENTITYMINECART_H