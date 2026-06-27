#ifndef NBTTAGLIST_H
#define NBTTAGLIST_H

// Auto-generated from NBTTagList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagList : public NBTBase {
public:
	NBTTagList();
	virtual ~NBTTagList();

	JList<void*> tagList;
	unsigned char tagType;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();
	virtual void setTag(NBTBase* var1);
	virtual NBTBase* tagAt(int var1);
	virtual int tagCount();

	static void initStatics();
};

#endif // NBTTAGLIST_H