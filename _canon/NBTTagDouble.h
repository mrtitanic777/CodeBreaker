#ifndef NBTTAGDOUBLE_H
#define NBTTAGDOUBLE_H

// Auto-generated from NBTTagDouble.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagDouble : public NBTBase {
public:
	NBTTagDouble();
	NBTTagDouble(double var1);
	virtual ~NBTTagDouble();

	double doubleValue;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();

	static void initStatics();
};

#endif // NBTTAGDOUBLE_H