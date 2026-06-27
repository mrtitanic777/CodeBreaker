#ifndef NBTTAGBYTEARRAY_H
#define NBTTAGBYTEARRAY_H

// Auto-generated from NBTTagByteArray.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagByteArray : public NBTBase {
public:
	NBTTagByteArray();
	NBTTagByteArray(JArray<unsigned char > var1);
	virtual ~NBTTagByteArray();

	JArray<unsigned char > byteArray;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();

	static void initStatics();
};

#endif // NBTTAGBYTEARRAY_H