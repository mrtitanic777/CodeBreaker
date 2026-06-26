#ifndef NBTTAGBYTE_H
#define NBTTAGBYTE_H

// Auto-generated from NBTTagByte.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;

class NBTTagByte : public NBTBase {
public:
	NBTTagByte();
	NBTTagByte(unsigned char var1);
	virtual ~NBTTagByte();

	unsigned char m_byteValue;

	virtual unsigned char getType();
	virtual const std::string& toString();

private:
	void writeTagContents(void* /* PORT: was DataOutput */ var1);
	void readTagContents(void* /* PORT: was DataInput */ var1);

};

#endif // NBTTAGBYTE_H