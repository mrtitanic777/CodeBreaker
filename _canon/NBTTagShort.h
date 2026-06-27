#ifndef NBTTAGSHORT_H
#define NBTTAGSHORT_H

// Auto-generated from NBTTagShort.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagShort : public NBTBase {
public:
	NBTTagShort();
	NBTTagShort(short var1);
	virtual ~NBTTagShort();

	short shortValue;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();

	static void initStatics();
};

#endif // NBTTAGSHORT_H