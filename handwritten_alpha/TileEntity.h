#ifndef TILEENTITY_H
#define TILEENTITY_H

// Auto-generated from TileEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class Block;
class NBTTagCompound;
class World;

class TileEntity {
public:
	TileEntity();
	virtual ~TileEntity();
	static void initStatics();  // run once at startup (was Java static {})

	World* worldObj;
	int xCoord;
	int yCoord;
	int zCoord;

	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual void updateEntity();
	static TileEntity* createAndLoadEntity(NBTTagCompound* var0);
	virtual int getBlockMetadata();
	virtual void onInventoryChanged();
	virtual double getDistanceFrom(double var1, double var3, double var5);
	virtual Block* getBlockType();

	// PORT: Java used reflection (getClass()) for the NBT "id" string. C++03 on
	// Xbox 360 has no RTTI reflection, so each TileEntity subclass returns its
	// saved id here. Default "" means "not persisted".
	virtual const std::string& getEntityName();

public: // was private
	void addMapping(const std::string& var1);

};

#endif // TILEENTITY_H
