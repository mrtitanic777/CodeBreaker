#include "stdafx.h"
// Auto-generated from NBTTagDouble.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagDouble.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagDouble::NBTTagDouble()
	: m_doubleValue(0)
{
}

NBTTagDouble::NBTTagDouble(double var1)
	: m_doubleValue(0)
{
		m_doubleValue = var1;
}

NBTTagDouble::~NBTTagDouble() {
	// No dynamic resources
}

void NBTTagDouble::writeTagContents(DataOutput* var1) {
		var1->writeDouble(m_doubleValue);
}

void NBTTagDouble::readTagContents(DataInput* var1) {
		m_doubleValue = var1->readDouble();
}

unsigned char NBTTagDouble::getType() {
		return (unsigned char)6;
}

std::string NBTTagDouble::toString() {
		return toStr(m_doubleValue);
}
