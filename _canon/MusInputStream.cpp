#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MusInputStream.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MusInputStream.h"
#include "CodecMus.h"
#include "InputStream.h"
#include "URL.h"

MusInputStream::MusInputStream(CodecMus* var1, URL* var2, InputStream* var3)
	: hash(0)
	, inputStream(NULL)
	, codec(NULL)
{
	this->codec = var1;
	this->buffer = JArray<unsigned char >(1);
	this->inputStream = var3;
	JavaString var4 = var2->getPath();
	var4 = var4.substr(var4.lastIndexOf("/") + 1);
	this->hash = var4.hashCode();
}

MusInputStream::~MusInputStream()
{
}

int MusInputStream::read()
{
	int var1 = this->read(this->buffer, 0, 1);
	return var1 < 0 ? var1 : this->buffer[0];
}

int MusInputStream::read(JArray<unsigned char > var1, int var2, int var3)
{
	var3 = this->inputStream->read(var1, var2, var3);
	for(int var4 = 0; var4 < var3; ++var4) {
	unsigned char var5 = var1[var2 + var4] = (unsigned char)(var1[var2 + var4] ^ this->hash >> 8);
	this->hash = this->hash * 498729871 + 85731 * var5;
	}
	return var3;
}

void MusInputStream::initStatics()
{
}
