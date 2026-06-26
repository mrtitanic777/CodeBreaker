#include "stdafx.h"
// Auto-generated from NBTTagByte.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagByte.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagByte::NBTTagByte()
	: m_byteValue(0)
{
}

NBTTagByte::NBTTagByte(unsigned char var1)
	: m_byteValue(0)
{
		m_byteValue = var1;
}

NBTTagByte::~NBTTagByte() {
	// No dynamic resources
}

void NBTTagByte::writeTagContents(DataOutput* var1) {
		var1->writeByte(m_byteValue);
}

void NBTTagByte::readTagContents(DataInput* var1) {
		m_byteValue = var1->readByte();
}

unsigned char NBTTagByte::getType() {
		return (unsigned char)1;
}

const std::string& NBTTagByte::toString() {
		return "" + m_byteValue;
}
