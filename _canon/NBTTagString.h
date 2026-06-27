#ifndef NBTTAGSTRING_H
#define NBTTAGSTRING_H

// Auto-generated from NBTTagString.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagString : public NBTBase {
public:
	NBTTagString();
	NBTTagString(const JavaString& var1);
	virtual ~NBTTagString();

	JavaString stringValue;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();

	static void initStatics();
};

#endif // NBTTAGSTRING_H