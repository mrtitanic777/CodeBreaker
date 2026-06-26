#ifndef NBTTAGEND_H
#define NBTTAGEND_H

// Auto-generated from NBTTagEnd.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;

class NBTTagEnd : public NBTBase {
public:
	NBTTagEnd();
	virtual ~NBTTagEnd();

	virtual unsigned char getType();
	virtual const std::string& toString();

private:
	void readTagContents(void* /* PORT: was DataInput */ var1);
	void writeTagContents(void* /* PORT: was DataOutput */ var1);

};

#endif // NBTTAGEND_H