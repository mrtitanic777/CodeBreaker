#include "stdafx.h"
// Auto-generated from NBTBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>
#include "NBTTagByte.h"
#include "NBTTagByteArray.h"
#include "NBTTagCompound.h"
#include "NBTTagDouble.h"
#include "NBTTagEnd.h"
#include "NBTTagFloat.h"
#include "NBTTagInt.h"
#include "NBTTagList.h"
#include "NBTTagLong.h"
#include "NBTTagShort.h"
#include "NBTTagString.h"

NBTBase::NBTBase()
	: m_key("")
{
}

NBTBase::~NBTBase() {
	// No dynamic resources
}

const std::string& NBTBase::getKey() {
		return m_key.empty() ? "" : m_key;
}

NBTBase* NBTBase::setKey(const std::string& var1) {
		m_key = var1;
		return this;
}

NBTBase* NBTBase::readTag(DataInput* var0) {
		unsigned char var1 = var0->readByte();
		if(var1 == 0) {
			return new NBTTagEnd();
		} else {
			NBTBase* var3 = createTagOfType(var1);
			short var2 = var0->readShort();
			unsigned char* var4 = new unsigned char[var2];
			int var4_count = 0;
			var0->readFully(var4, var4_count);
			var3->m_key = std::string((const char*)var4, var4_count);
			var3->readTagContents(var0);
			return var3;
		}
}

void NBTBase::writeTag(NBTBase* var0, DataOutput* var1) {
		var1->writeByte(var0->getType());
		if(var0->getType() != 0) {
			const unsigned char* var2 = (const unsigned char*)(var0->getKey()).c_str();
			int var2_count = 0;
			var1->writeShort(var2_count);
			var1->write(var2, var2_count);
			var0->writeTagContents(var1);
		}
}

NBTBase* NBTBase::createTagOfType(unsigned char var0) {
		switch(var0) {
		case 0:
			return new NBTTagEnd();
		case 1:
			return new NBTTagByte();
		case 2:
			return new NBTTagShort();
		case 3:
			return new NBTTagInt();
		case 4:
			return new NBTTagLong();
		case 5:
			return new NBTTagFloat();
		case 6:
			return new NBTTagDouble();
		case 7:
			return new NBTTagByteArray();
		case 8:
			return new NBTTagString();
		case 9:
			return new NBTTagList();
		case 10:
			return new NBTTagCompound();
		default:
			return NULL;
		}
}
