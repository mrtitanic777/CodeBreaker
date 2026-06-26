#ifndef NBTTAGBYTEARRAY_H
#define NBTTAGBYTEARRAY_H

// Auto-generated from NBTTagByteArray.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;

class NBTTagByteArray : public NBTBase {
public:
	NBTTagByteArray();
	NBTTagByteArray(unsigned char* var1);
	virtual ~NBTTagByteArray();

	unsigned char* m_byteArray;

	virtual unsigned char getType();
	virtual std::string toString();

public: // was private
	void writeTagContents(DataOutput* var1);
	void readTagContents(DataInput* var1);

public:  // array-length companions
	int m_byteArray_count;

};

#endif // NBTTAGBYTEARRAY_H