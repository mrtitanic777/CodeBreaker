#ifndef NBTTAGLIST_H
#define NBTTAGLIST_H

// Auto-generated from NBTTagList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>
#include <vector>

class DataInput;
class DataOutput;

class NBTTagList : public NBTBase {
public:
	NBTTagList();
	virtual ~NBTTagList();

	virtual unsigned char getType();
	virtual std::string toString();
	virtual void setTag(NBTBase* var1);
	virtual NBTBase* tagAt(int var1);
	virtual int tagCount();

public: // was private
	std::vector<NBTBase*> m_tagList;
	unsigned char m_tagType;

	void writeTagContents(DataOutput* var1);
	void readTagContents(DataInput* var1);

};

#endif // NBTTAGLIST_H