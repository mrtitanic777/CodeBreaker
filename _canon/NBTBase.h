#ifndef NBTBASE_H
#define NBTBASE_H

// Auto-generated from NBTBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class DataInput;
class DataOutput;

class NBTBase {
public:
	NBTBase();
	virtual ~NBTBase();

	JavaString key;

	virtual void writeTagContents(DataOutput* var1) = 0;
	virtual void readTagContents(DataInput* var1) = 0;
	virtual unsigned char getType() = 0;
	virtual JavaString getKey();
	virtual NBTBase* setKey(const JavaString& var1);
	static NBTBase* readTag(DataInput* var0);
	static void writeTag(NBTBase* var0, DataOutput* var1);
	static NBTBase* createTagOfType(unsigned char var0);
	static JavaString getTagName(unsigned char var0);

	static void initStatics();
};

#endif // NBTBASE_H