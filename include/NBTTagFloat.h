#ifndef NBTTAGFLOAT_H
#define NBTTAGFLOAT_H

// Auto-generated from NBTTagFloat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;

class NBTTagFloat : public NBTBase {
public:
	NBTTagFloat();
	NBTTagFloat(float var1);
	virtual ~NBTTagFloat();

	float m_floatValue;

	virtual unsigned char getType();
	virtual const std::string& toString();

private:
	void writeTagContents(void* /* PORT: was DataOutput */ var1);
	void readTagContents(void* /* PORT: was DataInput */ var1);

};

#endif // NBTTAGFLOAT_H