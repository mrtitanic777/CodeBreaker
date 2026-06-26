#ifndef TILEENTITYSIGN_H
#define TILEENTITYSIGN_H

// Auto-generated from TileEntitySign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntity.h"
#include <string>

class NBTTagCompound;

class TileEntitySign : public TileEntity {
public:
	TileEntitySign();
	virtual ~TileEntitySign();

	std::string* m_signText;
	int m_lineBeingEdited;

	virtual void writeToNBT(NBTTagCompound* var1);
	virtual void readFromNBT(NBTTagCompound* var1);

public:  // array-length companions
	int m_signText_count;

};

#endif // TILEENTITYSIGN_H