#ifndef NBTTAGBYTE_H
#define NBTTAGBYTE_H

// Auto-generated from NBTTagByte.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagByte : public NBTBase {
public:
	NBTTagByte();
	NBTTagByte(unsigned char var1);
	virtual ~NBTTagByte();

	unsigned char byteValue;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();

	static void initStatics();
};

#endif // NBTTAGBYTE_H