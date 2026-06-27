#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagFloat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagFloat.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "NBTBase.h"

NBTTagFloat::NBTTagFloat()
	: floatValue(0)
{
}

NBTTagFloat::NBTTagFloat(float var1)
	: floatValue(0)
{
	this->floatValue = var1;
}

NBTTagFloat::~NBTTagFloat()
{
}

void NBTTagFloat::writeTagContents(DataOutput* var1)
{
	var1->writeFloat(this->floatValue);
}

void NBTTagFloat::readTagContents(DataInput* var1)
{
	this->floatValue = var1->readFloat();
}

unsigned char NBTTagFloat::getType()
{
	return (unsigned char)5;
}

JavaString NBTTagFloat::toString()
{
	return (JCat() + "" + this->floatValue);
}

void NBTTagFloat::initStatics()
{
}
