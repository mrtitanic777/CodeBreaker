#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagList.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "NBTBase.h"

NBTTagList::NBTTagList()
	: tagType(0)
{
	this->tagList = JList<void*>();
}

NBTTagList::~NBTTagList()
{
}

void NBTTagList::writeTagContents(DataOutput* var1)
{
	if(this->tagList.size() > 0) {
	this->tagType = ((NBTBase*)this->tagList[0])->getType();
	} else {
	this->tagType = 1;
	}
	var1->writeByte(this->tagType);
	var1->writeInt(this->tagList.size());
	for(int var2 = 0; var2 < this->tagList.size(); ++var2) {
	((NBTBase*)this->tagList[var2])->writeTagContents(var1);
	}
}

void NBTTagList::readTagContents(DataInput* var1)
{
	this->tagType = var1->readByte();
	int var2 = var1->readInt();
	this->tagList = JList<void*>();
	for(int var3 = 0; var3 < var2; ++var3) {
	NBTBase* var4 = NBTBase::createTagOfType(this->tagType);
	var4->readTagContents(var1);
	this->tagList.push_back(var4);
	}
}

unsigned char NBTTagList::getType()
{
	return (unsigned char)9;
}

JavaString NBTTagList::toString()
{
	return (JCat() + "" + this->tagList.size() + " entries of type " + NBTBase::getTagName(this->tagType));
}

void NBTTagList::setTag(NBTBase* var1)
{
	this->tagType = var1->getType();
	this->tagList.push_back(var1);
}

NBTBase* NBTTagList::tagAt(int var1)
{
	return (NBTBase*)this->tagList[var1];
}

int NBTTagList::tagCount()
{
	return this->tagList.size();
}

void NBTTagList::initStatics()
{
}
