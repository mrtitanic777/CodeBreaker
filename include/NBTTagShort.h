#ifndef NBTTAGSHORT_H
#define NBTTAGSHORT_H

// Auto-generated from NBTTagShort.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;

class NBTTagShort : public NBTBase {
public:
	NBTTagShort();
	NBTTagShort(short var1);
	virtual ~NBTTagShort();

	short m_shortValue;

	virtual unsigned char getType();
	virtual const std::string& toString();

private:
	void writeTagContents(void* /* PORT: was DataOutput */ var1);
	void readTagContents(void* /* PORT: was DataInput */ var1);

};

#endif // NBTTAGSHORT_H