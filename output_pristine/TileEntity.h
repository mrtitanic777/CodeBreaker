#ifndef TILEENTITY_H
#define TILEENTITY_H

// Auto-generated from TileEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

class NBTTagCompound;

class TileEntity {
public:
	TileEntity();
	virtual ~TileEntity();

	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);

};

#endif // TILEENTITY_H