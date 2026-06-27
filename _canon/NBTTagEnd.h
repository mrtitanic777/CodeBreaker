#ifndef NBTTAGEND_H
#define NBTTAGEND_H

// Auto-generated from NBTTagEnd.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NBTBase.h"

class DataInput;
class DataOutput;

class NBTTagEnd : public NBTBase {
public:
	virtual ~NBTTagEnd();

	virtual void readTagContents(DataInput* var1);
	virtual void writeTagContents(DataOutput* var1);
	virtual unsigned char getType();
	virtual JavaString toString();

	static void initStatics();
};

#endif // NBTTAGEND_H