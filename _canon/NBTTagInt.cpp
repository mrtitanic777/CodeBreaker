#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagInt.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagInt.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "NBTBase.h"

NBTTagInt::NBTTagInt()
	: intValue(0)
{
}

NBTTagInt::NBTTagInt(int var1)
	: intValue(0)
{
	this->intValue = var1;
}

NBTTagInt::~NBTTagInt()
{
}

void NBTTagInt::writeTagContents(DataOutput* var1)
{
	var1->writeInt(this->intValue);
}

void NBTTagInt::readTagContents(DataInput* var1)
{
	this->intValue = var1->readInt();
}

unsigned char NBTTagInt::getType()
{
	return (unsigned char)3;
}

JavaString NBTTagInt::toString()
{
	return (JCat() + "" + this->intValue);
}

void NBTTagInt::initStatics()
{
}
