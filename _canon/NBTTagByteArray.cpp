#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagByteArray.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagByteArray.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "NBTBase.h"

NBTTagByteArray::NBTTagByteArray()
{
}

NBTTagByteArray::NBTTagByteArray(JArray<unsigned char > var1)
{
	this->byteArray = var1;
}

NBTTagByteArray::~NBTTagByteArray()
{
}

void NBTTagByteArray::writeTagContents(DataOutput* var1)
{
	var1->writeInt(this->byteArray.length);
	var1->write(this->byteArray);
}

void NBTTagByteArray::readTagContents(DataInput* var1)
{
	int var2 = var1->readInt();
	this->byteArray = JArray<unsigned char >(var2);
	var1->readFully(this->byteArray);
}

unsigned char NBTTagByteArray::getType()
{
	return (unsigned char)7;
}

JavaString NBTTagByteArray::toString()
{
	return (JCat() + "[" + this->byteArray.length + " bytes]");
}

void NBTTagByteArray::initStatics()
{
}
