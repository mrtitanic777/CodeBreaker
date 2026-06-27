#ifndef TILEENTITYSIGN_H
#define TILEENTITYSIGN_H

// Auto-generated from TileEntitySign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TileEntity.h"

class NBTTagCompound;

class TileEntitySign : public TileEntity {
public:
	TileEntitySign();
	virtual ~TileEntitySign();

	JArray<JavaString > signText;
	int lineBeingEdited;

	virtual void writeToNBT(NBTTagCompound* var1);
	virtual void readFromNBT(NBTTagCompound* var1);

	static void initStatics();
};

#endif // TILEENTITYSIGN_H