#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NibbleArray.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NibbleArray.h"

NibbleArray::NibbleArray(int var1)
{
	this->data = JArray<unsigned char >(var1 >> 1);
}

NibbleArray::NibbleArray(JArray<unsigned char > var1)
{
	this->data = var1;
}

NibbleArray::~NibbleArray()
{
}

int NibbleArray::getNibble(int var1, int var2, int var3)
{
	int var4 = (var1 << 11) | (var3 << 7) | var2;
	int var5 = var4 >> 1;
	int var6 = var4 & 1;
	return var6 == 0 ? this->data[var5] & 15 : this->data[var5] >> 4 & 15;
}

void NibbleArray::setNibble(int var1, int var2, int var3, int var4)
{
	int var5 = (var1 << 11) | (var3 << 7) | var2;
	int var6 = var5 >> 1;
	int var7 = var5 & 1;
	if(var7 == 0) {
	this->data[var6] = (unsigned char)(this->data[var6] & 240 | var4 & 15);
	} else {
	this->data[var6] = (unsigned char)(this->data[var6] & 15 | (var4 & 15) << 4);
	}
}

bool NibbleArray::isValid()
{
	return this->data != NULL;
}

void NibbleArray::initStatics()
{
}
