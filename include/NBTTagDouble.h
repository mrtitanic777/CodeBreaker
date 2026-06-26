#ifndef NBTTAGDOUBLE_H
#define NBTTAGDOUBLE_H

// Auto-generated from NBTTagDouble.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;

class NBTTagDouble : public NBTBase {
public:
	NBTTagDouble();
	virtual ~NBTTagDouble();

	virtual unsigned char getType();
	virtual const std::string& toString();

private:
	double m_doubleValue;

	void writeTagContents(void* /* PORT: was DataOutput */ var1);
	void readTagContents(void* /* PORT: was DataInput */ var1);

};

#endif // NBTTAGDOUBLE_H