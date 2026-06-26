#ifndef NBTBASE_H
#define NBTBASE_H

// Auto-generated from NBTBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class DataInput;
class DataOutput;

class NBTBase {
public:
	NBTBase();
	virtual ~NBTBase();

	virtual unsigned char getType() = 0;
	virtual const std::string& getKey();
	virtual NBTBase* setKey(const std::string& var1);
	static NBTBase* readTag(DataInput* var0);
	static void writeTag(NBTBase* var0, DataOutput* var1);
	static NBTBase* createTagOfType(unsigned char var0);

private:
	std::string m_key;

	void writeTagContents(DataOutput* var1);
	void readTagContents(DataInput* var1);

};

#endif // NBTBASE_H