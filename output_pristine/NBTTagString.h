#ifndef NBTTAGSTRING_H
#define NBTTAGSTRING_H

// Auto-generated from NBTTagString.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;

class NBTTagString : public NBTBase {
public:
	NBTTagString();
	NBTTagString(const std::string& var1);
	virtual ~NBTTagString();

	std::string m_stringValue;

	virtual unsigned char getType();
	virtual const std::string& toString();

private:
	void writeTagContents(DataOutput* var1);
	void readTagContents(DataInput* var1);

};

#endif // NBTTAGSTRING_H