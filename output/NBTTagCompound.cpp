#include "stdafx.h"
// Auto-generated from NBTTagCompound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagCompound.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "Map.h"
#include "NBTTagList.h"
#include <string>
#include "NBTBase.h"
#include "NBTTagByte.h"
#include "NBTTagByteArray.h"
#include "NBTTagFloat.h"
#include "NBTTagInt.h"
#include "NBTTagLong.h"
#include "NBTTagShort.h"
#include "NBTTagString.h"
#include "NBTTagDouble.h"
#include "NBTTagEnd.h"

NBTTagCompound::NBTTagCompound()
	: m_tagMap(new std::map<std::string, void*>())
{
}

NBTTagCompound::~NBTTagCompound() {
	m_tagMap = NULL;
}

void NBTTagCompound::writeTagContents(DataOutput* var1) {
		for (std::map<std::string, void*>::iterator _i2 = m_tagMap->begin(); _i2 != m_tagMap->end(); ++_i2) {
			NBTBase* var3 = (NBTBase*)_i2->second;
			NBTBase::writeTag(var3, var1);
		}

		var1->writeByte(0);
}

void NBTTagCompound::readTagContents(DataInput* var1) {
		m_tagMap->clear();

		while(true) {
			NBTBase* var2 = NBTBase::readTag(var1);
			if(var2->getType() == 0) {
				return;
			}

			(*m_tagMap)[var2->getKey()] = (void*)var2;
		}
}

unsigned char NBTTagCompound::getType() {
		return (unsigned char)10;
}

void NBTTagCompound::setTag(const std::string& var1, NBTBase* var2) {
		(*m_tagMap)[var1] = (void*)(var2->setKey(var1));
}

void NBTTagCompound::setByte(const std::string& var1, unsigned char var2) {
		(*m_tagMap)[var1] = (void*)((new NBTTagByte(var2))->setKey(var1));
}

void NBTTagCompound::setShort(const std::string& var1, short var2) {
		(*m_tagMap)[var1] = (void*)((new NBTTagShort(var2))->setKey(var1));
}

void NBTTagCompound::setInteger(const std::string& var1, int var2) {
		(*m_tagMap)[var1] = (void*)((new NBTTagInt(var2))->setKey(var1));
}

void NBTTagCompound::setLong(const std::string& var1, long long var2) {
		(*m_tagMap)[var1] = (void*)((new NBTTagLong(var2))->setKey(var1));
}

void NBTTagCompound::setFloat(const std::string& var1, float var2) {
		(*m_tagMap)[var1] = (void*)((new NBTTagFloat(var2))->setKey(var1));
}

void NBTTagCompound::setString(const std::string& var1, const std::string& var2) {
		(*m_tagMap)[var1] = (void*)((new NBTTagString(var2))->setKey(var1));
}

void NBTTagCompound::setByteArray(const std::string& var1, unsigned char* var2) {
		(*m_tagMap)[var1] = (void*)((new NBTTagByteArray(var2))->setKey(var1));
}

void NBTTagCompound::setCompoundTag(const std::string& var1, NBTTagCompound* var2) {
		(*m_tagMap)[var1] = (void*)(var2->setKey(var1));
}

void NBTTagCompound::setBoolean(const std::string& var1, bool var2) {
		setByte(var1, (unsigned char)(var2 ? 1 : 0));
}

unsigned char NBTTagCompound::getByte(const std::string& var1) {
		return !m_tagMap->count(var1) ? 0 : ((NBTTagByte*)(*m_tagMap)[var1])->m_byteValue;
}

short NBTTagCompound::getShort(const std::string& var1) {
		return !m_tagMap->count(var1) ? 0 : ((NBTTagShort*)(*m_tagMap)[var1])->m_shortValue;
}

int NBTTagCompound::getInteger(const std::string& var1) {
		return !m_tagMap->count(var1) ? 0 : ((NBTTagInt*)(*m_tagMap)[var1])->m_intValue;
}

long long NBTTagCompound::getLong(const std::string& var1) {
		return !m_tagMap->count(var1) ? 0L : ((NBTTagLong*)(*m_tagMap)[var1])->m_longValue;
}

float NBTTagCompound::getFloat(const std::string& var1) {
		return !m_tagMap->count(var1) ? 0.0F : ((NBTTagFloat*)(*m_tagMap)[var1])->m_floatValue;
}

const std::string& NBTTagCompound::getString(const std::string& var1) {
		return !m_tagMap->count(var1) ? "" : ((NBTTagString*)(*m_tagMap)[var1])->m_stringValue;
}

unsigned char* NBTTagCompound::getByteArray(const std::string& var1) {
		return !m_tagMap->count(var1) ? new unsigned char[0] : ((NBTTagByteArray*)(*m_tagMap)[var1])->m_byteArray;
}

NBTTagCompound* NBTTagCompound::getCompoundTag(const std::string& var1) {
		return !m_tagMap->count(var1) ? new NBTTagCompound() : (NBTTagCompound*)(*m_tagMap)[var1];
}

NBTTagList* NBTTagCompound::getTagList(const std::string& var1) {
		return !m_tagMap->count(var1) ? new NBTTagList() : (NBTTagList*)(*m_tagMap)[var1];
}

bool NBTTagCompound::getBoolean(const std::string& var1) {
		return getByte(var1) != 0;
}

const std::string& NBTTagCompound::toString() {
		return toStr(m_tagMap->size()) + " entries";
}

bool NBTTagCompound::emptyNBTMap() {
		return m_tagMap->empty();
}
