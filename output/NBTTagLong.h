#ifndef NBTTAGLONG_H
#define NBTTAGLONG_H

// Auto-generated from NBTTagLong.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;

class NBTTagLong : public NBTBase {
public:
	NBTTagLong();
	NBTTagLong(long long var1);
	virtual ~NBTTagLong();

	long long m_longValue;

	virtual unsigned char getType();
	virtual const std::string& toString();

public: // was private
	void writeTagContents(DataOutput* var1);
	void readTagContents(DataInput* var1);

};

#endif // NBTTAGLONG_H