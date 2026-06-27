#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTBase.h"
#include "DataInput.h"
#include "DataOutput.h"
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
{
	this->key = NULL;
}

NBTBase::~NBTBase()
{
}

JavaString NBTBase::getKey()
{
	return this->key == NULL ? "" : this->key;
}

NBTBase* NBTBase::setKey(const JavaString& var1)
{
	this->key = var1;
	return this;
}

NBTBase* NBTBase::readTag(DataInput* var0)
{
	unsigned char var1 = var0->readByte();
	if(var1 == 0) {
	return new NBTTagEnd();
	} else {
	NBTBase* var2 = createTagOfType(var1);
	var2->key = var0->readUTF();
	var2->readTagContents(var0);
	return var2;
	}
}

void NBTBase::writeTag(NBTBase* var0, DataOutput* var1)
{
	var1->writeByte(var0->getType());
	if(var0->getType() != 0) {
	var1->writeUTF(var0->getKey());
	var0->writeTagContents(var1);
	}
}

NBTBase* NBTBase::createTagOfType(unsigned char var0)
{
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

JavaString NBTBase::getTagName(unsigned char var0)
{
	switch(var0) {
	case 0:
	return "TAG_End";
	case 1:
	return "TAG_Byte";
	case 2:
	return "TAG_Short";
	case 3:
	return "TAG_Int";
	case 4:
	return "TAG_Long";
	case 5:
	return "TAG_Float";
	case 6:
	return "TAG_Double";
	case 7:
	return "TAG_Byte_Array";
	case 8:
	return "TAG_String";
	case 9:
	return "TAG_List";
	case 10:
	return "TAG_Compound";
	default:
	return "UNKNOWN";
	}
}

void NBTBase::initStatics()
{
}
