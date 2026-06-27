#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagDouble.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagDouble.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "NBTBase.h"

NBTTagDouble::NBTTagDouble()
	: doubleValue(0)
{
}

NBTTagDouble::NBTTagDouble(double var1)
	: doubleValue(0)
{
	this->doubleValue = var1;
}

NBTTagDouble::~NBTTagDouble()
{
}

void NBTTagDouble::writeTagContents(DataOutput* var1)
{
	var1->writeDouble(this->doubleValue);
}

void NBTTagDouble::readTagContents(DataInput* var1)
{
	this->doubleValue = var1->readDouble();
}

unsigned char NBTTagDouble::getType()
{
	return (unsigned char)6;
}

JavaString NBTTagDouble::toString()
{
	return (JCat() + "" + this->doubleValue);
}

void NBTTagDouble::initStatics()
{
}
