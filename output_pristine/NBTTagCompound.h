#ifndef NBTTAGCOMPOUND_H
#define NBTTAGCOMPOUND_H

// Auto-generated from NBTTagCompound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include <string>

class DataInput;
class DataOutput;
class Map;
class NBTTagList;

class NBTTagCompound : public NBTBase {
public:
	NBTTagCompound();
	virtual ~NBTTagCompound();

	virtual unsigned char getType();
	virtual void setTag(const std::string& var1, NBTBase* var2);
	virtual void setByte(const std::string& var1, unsigned char var2);
	virtual void setShort(const std::string& var1, short var2);
	virtual void setInteger(const std::string& var1, int var2);
	virtual void setLong(const std::string& var1, long long var2);
	virtual void setFloat(const std::string& var1, float var2);
	virtual void setString(const std::string& var1, const std::string& var2);
	virtual void setByteArray(const std::string& var1, unsigned char* var2);
	virtual void setCompoundTag(const std::string& var1, NBTTagCompound* var2);
	virtual void setBoolean(const std::string& var1, bool var2);
	virtual unsigned char getByte(const std::string& var1);
	virtual short getShort(const std::string& var1);
	virtual int getInteger(const std::string& var1);
	virtual long long getLong(const std::string& var1);
	virtual float getFloat(const std::string& var1);
	virtual const std::string& getString(const std::string& var1);
	virtual unsigned char* getByteArray(const std::string& var1);
	virtual NBTTagCompound* getCompoundTag(const std::string& var1);
	virtual NBTTagList* getTagList(const std::string& var1);
	virtual bool getBoolean(const std::string& var1);
	virtual const std::string& toString();
	virtual bool emptyNBTMap();

private:
	Map* m_tagMap;

	void writeTagContents(DataOutput* var1);
	void readTagContents(DataInput* var1);

};

#endif // NBTTAGCOMPOUND_H