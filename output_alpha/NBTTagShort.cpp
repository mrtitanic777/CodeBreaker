#include "stdafx.h"
// Auto-generated from NBTTagShort.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagShort.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagShort::NBTTagShort()
	: m_shortValue(0)
{
}

NBTTagShort::NBTTagShort(short var1)
	: m_shortValue(0)
{
		m_shortValue = var1;
}

NBTTagShort::~NBTTagShort() {
	// No dynamic resources
}

void NBTTagShort::writeTagContents(DataOutput* var1) {
		var1->writeShort(m_shortValue);
}

void NBTTagShort::readTagContents(DataInput* var1) {
		m_shortValue = var1->readShort();
}

unsigned char NBTTagShort::getType() {
		return (unsigned char)2;
}

std::string NBTTagShort::toString() {
		return toStr(m_shortValue);
}
