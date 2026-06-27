#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NBTTagString.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagString.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "IllegalArgumentException.h"
#include "NBTBase.h"

NBTTagString::NBTTagString()
{
}

NBTTagString::NBTTagString(const JavaString& var1)
{
	this->stringValue = var1;
	if(var1 == NULL) {
	throw new IllegalArgumentException("Empty string not allowed");
	}
}

NBTTagString::~NBTTagString()
{
}

void NBTTagString::writeTagContents(DataOutput* var1)
{
	var1->writeUTF(this->stringValue);
}

void NBTTagString::readTagContents(DataInput* var1)
{
	this->stringValue = var1->readUTF();
}

unsigned char NBTTagString::getType()
{
	return (unsigned char)8;
}

JavaString NBTTagString::toString()
{
	return (JCat() + "" + this->stringValue);
}

void NBTTagString::initStatics()
{
}
