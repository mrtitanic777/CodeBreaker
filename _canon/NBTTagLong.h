#ifndef NBTTAGLONG_H
#define NBTTAGLONG_H

// Auto-generated from NBTTagLong.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagLong : public NBTBase {
public:
	NBTTagLong();
	NBTTagLong(long long var1);
	virtual ~NBTTagLong();

	long long longValue;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();

	static void initStatics();
};

#endif // NBTTAGLONG_H