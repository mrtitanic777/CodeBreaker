#ifndef NBTTAGINT_H
#define NBTTAGINT_H

// Auto-generated from NBTTagInt.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;

class NBTTagInt : public NBTBase {
public:
	NBTTagInt();
	NBTTagInt(int var1);
	virtual ~NBTTagInt();

	int m_intValue;

	virtual unsigned char getType();
	virtual std::string toString();

public: // was private
	void writeTagContents(DataOutput* var1);
	void readTagContents(DataInput* var1);

};

#endif // NBTTAGINT_H