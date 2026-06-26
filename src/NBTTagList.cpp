#include "stdafx.h"
// Auto-generated from NBTTagList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagList.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>
#include "NBTBase.h"

NBTTagList::NBTTagList()
	: m_tagType(0)
{
}

NBTTagList::~NBTTagList() {
	// No dynamic resources
}

void NBTTagList::writeTagContents(void* /* PORT: was DataOutput */ var1) {
		if(m_tagList.size() > 0) {
			m_tagType = m_tagList[0]->getType();
		} else {
			m_tagType = 1;
		}

		var1->writeByte(m_tagType);
		var1->writeInt(m_tagList.size());

		for(size_t var2 = 0; var2 < m_tagList.size(); ++var2) {
			m_tagList[var2]->writeTagContents(var1);
		}

}

void NBTTagList::readTagContents(void* /* PORT: was DataInput */ var1) {
		m_tagType = var1->readByte();
		int var2 = var1->readInt();
		m_tagList = /* default vector */;

		for(int var3 = 0; var3 < var2; ++var3) {
			NBTBase* var4 = NBTBase::createTagOfType(m_tagType);
			var4->readTagContents(var1);
			m_tagList.push_back(var4);
		}

}

unsigned char NBTTagList::getType() {
		return (unsigned char)9;
}

const std::string& NBTTagList::toString() {
		std::string var10000 = (new std::string()).append("").append(m_tagList.size()).append(" entries of type ");
		unsigned char var1 = m_tagType;
		std::string var10001;
		switch(var1) {
		case 0:
			var10001 = "TAG_End";
			break;
		case 1:
			var10001 = "TAG_Byte";
			break;
		case 2:
			var10001 = "TAG_Short";
			break;
		case 3:
			var10001 = "TAG_Int";
			break;
		case 4:
			var10001 = "TAG_Long";
			break;
		case 5:
			var10001 = "TAG_Float";
			break;
		case 6:
			var10001 = "TAG_Double";
			break;
		case 7:
			var10001 = "TAG_Byte_Array";
			break;
		case 8:
			var10001 = "TAG_String";
			break;
		case 9:
			var10001 = "TAG_List";
			break;
		case 10:
			var10001 = "TAG_Compound";
			break;
		default:
			var10001 = "UNKNOWN";
		}

		return var10000.append(var10001) /* PORT: toString */;
}

void NBTTagList::setTag(NBTBase* var1) {
		m_tagType = var1->getType();
		m_tagList.push_back(var1);
}

NBTBase* NBTTagList::tagAt(int var1) {
		return (NBTBase*)m_tagList[var1];
}

int NBTTagList::tagCount() {
		return m_tagList.size();
}
