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
	NBTTagDouble(double var1);
	virtual ~NBTTagDouble();

	double m_doubleValue;

	virtual unsigned char getType();
	virtual std::string toString();

public: // was private
	void writeTagContents(DataOutput* var1);
	void readTagContents(DataInput* var1);

};

#endif // NBTTAGDOUBLE_H