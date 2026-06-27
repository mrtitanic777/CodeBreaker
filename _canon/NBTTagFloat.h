#ifndef NBTTAGFLOAT_H
#define NBTTAGFLOAT_H

// Auto-generated from NBTTagFloat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagFloat : public NBTBase {
public:
	NBTTagFloat();
	NBTTagFloat(float var1);
	virtual ~NBTTagFloat();

	float floatValue;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();

	static void initStatics();
};

#endif // NBTTAGFLOAT_H