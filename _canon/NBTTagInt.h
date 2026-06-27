#ifndef NBTTAGINT_H
#define NBTTAGINT_H

// Auto-generated from NBTTagInt.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagInt : public NBTBase {
public:
	NBTTagInt();
	NBTTagInt(int var1);
	virtual ~NBTTagInt();

	int intValue;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();

	static void initStatics();
};

#endif // NBTTAGINT_H