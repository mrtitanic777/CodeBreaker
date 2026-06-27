#ifndef TILEENTITYMOBSPAWNER_H
#define TILEENTITYMOBSPAWNER_H

// Auto-generated from TileEntityMobSpawner.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TileEntity.h"

class NBTTagCompound;

class TileEntityMobSpawner : public TileEntity {
public:
	TileEntityMobSpawner();
	virtual ~TileEntityMobSpawner();

	int delay;
	JavaString entityID;
	double spinAngle;
	double prevSpinAngle;

	virtual bool anyPlayerInRange();
	virtual void updateEntity();
	virtual void updateDelay();
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);

	static void initStatics();
};

#endif // TILEENTITYMOBSPAWNER_H