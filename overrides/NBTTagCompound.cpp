#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagCompound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagCompound.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "NBTBase.h"
#include "NBTTagByte.h"
#include "NBTTagByteArray.h"
#include "NBTTagDouble.h"
#include "NBTTagFloat.h"
#include "NBTTagInt.h"
#include "NBTTagList.h"
#include "NBTTagLong.h"
#include "NBTTagShort.h"
#include "NBTTagString.h"

NBTTagCompound::~NBTTagCompound()
{
}

void NBTTagCompound::writeTagContents(DataOutput* var1)
{
	for(std::map<std::string, void*>::iterator it = this->tagMap.begin(); it != this->tagMap.end(); ++it) {
	NBTBase* var3 = (NBTBase*)it->second;
	NBTBase::writeTag(var3, var1);
	}
	var1->writeByte(0);
}

void NBTTagCompound::readTagContents(DataInput* var1)
{
	this->tagMap.clear();
	while(true) {
	NBTBase* var2 = NBTBase::readTag(var1);
	if(var2->getType() == 0) {
	return;
	}
	this->tagMap[var2->getKey()] = var2;
	}
}

unsigned char NBTTagCompound::getType()
{
	return (unsigned char)10;
}

void NBTTagCompound::setTag(const JavaString& var1, NBTBase* var2)
{
	this->tagMap[var1] = var2->setKey(var1);
}

void NBTTagCompound::setByte(const JavaString& var1, unsigned char var2)
{
	this->tagMap[var1] = (new NBTTagByte(var2))->setKey(var1);
}

void NBTTagCompound::setShort(const JavaString& var1, short var2)
{
	this->tagMap[var1] = (new NBTTagShort(var2))->setKey(var1);
}

void NBTTagCompound::setInteger(const JavaString& var1, int var2)
{
	this->tagMap[var1] = (new NBTTagInt(var2))->setKey(var1);
}

void NBTTagCompound::setLong(const JavaString& var1, long long var2)
{
	this->tagMap[var1] = (new NBTTagLong(var2))->setKey(var1);
}

void NBTTagCompound::setFloat(const JavaString& var1, float var2)
{
	this->tagMap[var1] = (new NBTTagFloat(var2))->setKey(var1);
}

void NBTTagCompound::setDouble(const JavaString& var1, double var2)
{
	this->tagMap[var1] = (new NBTTagDouble(var2))->setKey(var1);
}

void NBTTagCompound::setString(const JavaString& var1, const JavaString& var2)
{
	this->tagMap[var1] = (new NBTTagString(var2))->setKey(var1);
}

void NBTTagCompound::setByteArray(const JavaString& var1, JArray<unsigned char > var2)
{
	this->tagMap[var1] = (new NBTTagByteArray(var2))->setKey(var1);
}

void NBTTagCompound::setCompoundTag(const JavaString& var1, NBTTagCompound* var2)
{
	this->tagMap[var1] = var2->setKey(var1);
}

void NBTTagCompound::setBoolean(const JavaString& var1, bool var2)
{
	this->setByte(var1, (unsigned char)(var2 ? 1 : 0));
}

bool NBTTagCompound::hasKey(const JavaString& var1)
{
	return this->tagMap.count(var1);
}

unsigned char NBTTagCompound::getByte(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? 0 : ((NBTTagByte*)this->tagMap[var1])->byteValue;
}

short NBTTagCompound::getShort(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? 0 : ((NBTTagShort*)this->tagMap[var1])->shortValue;
}

int NBTTagCompound::getInteger(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? 0 : ((NBTTagInt*)this->tagMap[var1])->intValue;
}

long long NBTTagCompound::getLong(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? 0L : ((NBTTagLong*)this->tagMap[var1])->longValue;
}

float NBTTagCompound::getFloat(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? 0.0F : ((NBTTagFloat*)this->tagMap[var1])->floatValue;
}

double NBTTagCompound::getDouble(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? 0.0D : ((NBTTagDouble*)this->tagMap[var1])->doubleValue;
}

JavaString NBTTagCompound::getString(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? "" : ((NBTTagString*)this->tagMap[var1])->stringValue;
}

JArray<unsigned char > NBTTagCompound::getByteArray(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? JArray<unsigned char >(0) : ((NBTTagByteArray*)this->tagMap[var1])->byteArray;
}

NBTTagCompound* NBTTagCompound::getCompoundTag(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? new NBTTagCompound() : (NBTTagCompound*)this->tagMap[var1];
}

NBTTagList* NBTTagCompound::getTagList(const JavaString& var1)
{
	return !this->tagMap.count(var1) ? new NBTTagList() : (NBTTagList*)this->tagMap[var1];
}

bool NBTTagCompound::getBoolean(const JavaString& var1)
{
	return this->getByte(var1) != 0;
}

JavaString NBTTagCompound::toString()
{
	return (JCat() + "" + this->tagMap.size() + " entries");
}
