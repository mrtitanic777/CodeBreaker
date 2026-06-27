#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagByte.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagByte.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "NBTBase.h"

NBTTagByte::NBTTagByte()
	: byteValue(0)
{
}

NBTTagByte::NBTTagByte(unsigned char var1)
	: byteValue(0)
{
	this->byteValue = var1;
}

NBTTagByte::~NBTTagByte()
{
}

void NBTTagByte::writeTagContents(DataOutput* var1)
{
	var1->writeByte(this->byteValue);
}

void NBTTagByte::readTagContents(DataInput* var1)
{
	this->byteValue = var1->readByte();
}

unsigned char NBTTagByte::getType()
{
	return (unsigned char)1;
}

JavaString NBTTagByte::toString()
{
	return (JCat() + "" + this->byteValue);
}

void NBTTagByte::initStatics()
{
}
