#ifndef TILEENTITYMOBSPAWNER_H
#define TILEENTITYMOBSPAWNER_H

// Auto-generated from TileEntityMobSpawner.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntity.h"
#include <string>

class NBTTagCompound;

class TileEntityMobSpawner : public TileEntity {
public:
	TileEntityMobSpawner();
	virtual ~TileEntityMobSpawner();

	int m_delay;
	std::string m_entityID;
	double m_field_831_c;
	double m_field_830_d;

	virtual bool anyPlayerInRange();
	virtual void updateEntity();
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);

public: // was private
	void updateDelay();

};

#endif // TILEENTITYMOBSPAWNER_H