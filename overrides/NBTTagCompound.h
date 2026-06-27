#ifndef NBTTAGCOMPOUND_H
#define NBTTAGCOMPOUND_H

// Auto-generated from NBTTagCompound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>
#include "NBTBase.h"

class DataInput;
class DataOutput;
class NBTTagList;

class NBTTagCompound : public NBTBase {
public:
	virtual ~NBTTagCompound();

	std::map<std::string, void*> tagMap;

	virtual void writeTagContents(DataOutput* var1);
	virtual void readTagContents(DataInput* var1);
	virtual unsigned char getType();
	virtual void setTag(const JavaString& var1, NBTBase* var2);
	virtual void setByte(const JavaString& var1, unsigned char var2);
	virtual void setShort(const JavaString& var1, short var2);
	virtual void setInteger(const JavaString& var1, int var2);
	virtual void setLong(const JavaString& var1, long long var2);
	virtual void setFloat(const JavaString& var1, float var2);
	virtual void setDouble(const JavaString& var1, double var2);
	virtual void setString(const JavaString& var1, const JavaString& var2);
	virtual void setByteArray(const JavaString& var1, JArray<unsigned char > var2);
	virtual void setCompoundTag(const JavaString& var1, NBTTagCompound* var2);
	virtual void setBoolean(const JavaString& var1, bool var2);
	virtual bool hasKey(const JavaString& var1);
	virtual unsigned char getByte(const JavaString& var1);
	virtual short getShort(const JavaString& var1);
	virtual int getInteger(const JavaString& var1);
	virtual long long getLong(const JavaString& var1);
	virtual float getFloat(const JavaString& var1);
	virtual double getDouble(const JavaString& var1);
	virtual JavaString getString(const JavaString& var1);
	virtual JArray<unsigned char > getByteArray(const JavaString& var1);
	virtual NBTTagCompound* getCompoundTag(const JavaString& var1);
	virtual NBTTagList* getTagList(const JavaString& var1);
	virtual bool getBoolean(const JavaString& var1);
	virtual JavaString toString();
};

#endif // NBTTAGCOMPOUND_H