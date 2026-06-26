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

NBTTagCompound::NBTTagCompound()
	: m_tagMap(NULL)
{
}

NBTTagCompound::~NBTTagCompound() {
	m_tagMap = NULL;
}

void NBTTagCompound::writeTagContents(DataOutput* var1) {
		for (size_t _i2 = 0; _i2 < m_tagMap->values().size(); ++_i2) {
			NBTBase* var3 = (NBTBase*)m_tagMap->values()[_i2];
			NBTBase::writeTag(var3, var1);
		}

		var1->writeByte(0);
}

void NBTTagCompound::readTagContents(DataInput* var1) {
		m_tagMap->clear();

		while(true) {
			NBTBase var2 = NBTBase::readTag(var1);
			if(var2.getType() == 0) {
				return;
			}

			m_tagMap->put(var2.getKey(), var2);
		}
}

unsigned char NBTTagCompound::getType() {
		return (unsigned char)10;
}

void NBTTagCompound::setTag(const std::string& var1, NBTBase* var2) {
		m_tagMap->put(var1, var2->setKey(var1));
}

void NBTTagCompound::setByte(const std::string& var1, unsigned char var2) {
		m_tagMap->put(var1, (new NBTTagByte(var2)).setKey(var1));
}

void NBTTagCompound::setShort(const std::string& var1, short var2) {
		m_tagMap->put(var1, (new NBTTagShort(var2)).setKey(var1));
}

void NBTTagCompound::setInteger(const std::string& var1, int var2) {
		m_tagMap->put(var1, (new NBTTagInt(var2)).setKey(var1));
}

void NBTTagCompound::setLong(const std::string& var1, long long var2) {
		m_tagMap->put(var1, (new NBTTagLong(var2)).setKey(var1));
}

void NBTTagCompound::setFloat(const std::string& var1, float var2) {
		m_tagMap->put(var1, (new NBTTagFloat(var2)).setKey(var1));
}

void NBTTagCompound::setString(const std::string& var1, const std::string& var2) {
		m_tagMap->put(var1, (new NBTTagString(var2)).setKey(var1));
}

void NBTTagCompound::setByteArray(const std::string& var1, unsigned char* var2) {
		m_tagMap->put(var1, (new NBTTagByteArray(var2)).setKey(var1));
}

void NBTTagCompound::setCompoundTag(const std::string& var1, NBTTagCompound* var2) {
		m_tagMap->put(var1, var2->setKey(var1));
}

void NBTTagCompound::setBoolean(const std::string& var1, bool var2) {
		setByte(var1, (unsigned char)(var2 ? 1 : 0));
}

unsigned char NBTTagCompound::getByte(const std::string& var1) {
		return !m_tagMap->containsKey(var1) ? 0 : m_tagMap[var1]->m_byteValue;
}

short NBTTagCompound::getShort(const std::string& var1) {
		return !m_tagMap->containsKey(var1) ? 0 : m_tagMap[var1]->m_shortValue;
}

int NBTTagCompound::getInteger(const std::string& var1) {
		return !m_tagMap->containsKey(var1) ? 0 : m_tagMap[var1]->m_intValue;
}

long long NBTTagCompound::getLong(const std::string& var1) {
		return !m_tagMap->containsKey(var1) ? 0L : m_tagMap[var1]->m_longValue;
}

float NBTTagCompound::getFloat(const std::string& var1) {
		return !m_tagMap->containsKey(var1) ? 0.0F : m_tagMap[var1]->m_floatValue;
}

const std::string& NBTTagCompound::getString(const std::string& var1) {
		return !m_tagMap->containsKey(var1) ? "" : m_tagMap[var1]->m_stringValue;
}

unsigned char* NBTTagCompound::getByteArray(const std::string& var1) {
		return !m_tagMap->containsKey(var1) ? new unsigned char[0] : m_tagMap[var1]->m_byteArray;
}

NBTTagCompound* NBTTagCompound::getCompoundTag(const std::string& var1) {
		return !m_tagMap->containsKey(var1) ? new NBTTagCompound() : (NBTTagCompound)m_tagMap[var1];
}

NBTTagList* NBTTagCompound::getTagList(const std::string& var1) {
		return !m_tagMap->containsKey(var1) ? new NBTTagList() : (NBTTagList)m_tagMap[var1];
}

bool NBTTagCompound::getBoolean(const std::string& var1) {
		return getByte(var1) != 0;
}

const std::string& NBTTagCompound::toString() {
		return "" + m_tagMap->size() + " entries";
}

bool NBTTagCompound::emptyNBTMap() {
		return m_tagMap->isEmpty();
}
