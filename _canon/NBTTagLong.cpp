#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagLong.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagLong.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "NBTBase.h"

NBTTagLong::NBTTagLong()
	: longValue(0)
{
}

NBTTagLong::NBTTagLong(long long var1)
	: longValue(0)
{
	this->longValue = var1;
}

NBTTagLong::~NBTTagLong()
{
}

void NBTTagLong::writeTagContents(DataOutput* var1)
{
	var1->writeLong(this->longValue);
}

void NBTTagLong::readTagContents(DataInput* var1)
{
	this->longValue = var1->readLong();
}

unsigned char NBTTagLong::getType()
{
	return (unsigned char)4;
}

JavaString NBTTagLong::toString()
{
	return (JCat() + "" + this->longValue);
}

void NBTTagLong::initStatics()
{
}
