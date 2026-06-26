#include "stdafx.h"
// Auto-generated from NBTTagLong.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagLong.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagLong::NBTTagLong()
	: m_longValue(0)
{
}

NBTTagLong::NBTTagLong(long long var1)
	: m_longValue(0)
{
		m_longValue = var1;
}

NBTTagLong::~NBTTagLong() {
	// No dynamic resources
}

void NBTTagLong::writeTagContents(DataOutput* var1) {
		var1->writeLong(m_longValue);
}

void NBTTagLong::readTagContents(DataInput* var1) {
		m_longValue = var1->readLong();
}

unsigned char NBTTagLong::getType() {
		return (unsigned char)4;
}

std::string NBTTagLong::toString() {
		return toStr(m_longValue);
}
