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

void NBTTagList::writeTagContents(DataOutput* var1) {
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

void NBTTagList::readTagContents(DataInput* var1) {
		m_tagType = var1->readByte();
		int var2 = var1->readInt();
		m_tagList.clear();

		for(int var3 = 0; var3 < var2; ++var3) {
			NBTBase* var4 = NBTBase::createTagOfType(m_tagType);
			var4->readTagContents(var1);
			m_tagList.push_back(var4);
		}

}

unsigned char NBTTagList::getType() {
		return (unsigned char)9;
}

std::string NBTTagList::toString() {
		return toStr(m_tagList.size()) + " entries of type " + NBTBase::getTagName(m_tagType);
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
