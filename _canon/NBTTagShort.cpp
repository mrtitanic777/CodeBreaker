#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagShort.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagShort.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "NBTBase.h"

NBTTagShort::NBTTagShort()
	: shortValue(0)
{
}

NBTTagShort::NBTTagShort(short var1)
	: shortValue(0)
{
	this->shortValue = var1;
}

NBTTagShort::~NBTTagShort()
{
}

void NBTTagShort::writeTagContents(DataOutput* var1)
{
	var1->writeShort(this->shortValue);
}

void NBTTagShort::readTagContents(DataInput* var1)
{
	this->shortValue = var1->readShort();
}

unsigned char NBTTagShort::getType()
{
	return (unsigned char)2;
}

JavaString NBTTagShort::toString()
{
	return (JCat() + "" + this->shortValue);
}

void NBTTagShort::initStatics()
{
}
