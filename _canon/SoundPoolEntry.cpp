#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from SoundPoolEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SoundPoolEntry.h"
#include "URL.h"

SoundPoolEntry::SoundPoolEntry(const JavaString& var1, URL* var2)
	: soundUrl(NULL)
{
	this->soundName = var1;
	this->soundUrl = var2;
}

SoundPoolEntry::~SoundPoolEntry()
{
}

void SoundPoolEntry::initStatics()
{
}
