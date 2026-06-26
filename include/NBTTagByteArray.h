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
	virtual const std::string& toString();

private:
	void writeTagContents(void* /* PORT: was DataOutput */ var1);
	void readTagContents(void* /* PORT: was DataInput */ var1);

};

#endif // NBTTAGBYTEARRAY_H