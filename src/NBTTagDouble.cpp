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

NBTTagDouble::~NBTTagDouble() {
	// No dynamic resources
}

void NBTTagDouble::writeTagContents(void* /* PORT: was DataOutput */ var1) {
		var1->writeDouble(m_doubleValue);
}

void NBTTagDouble::readTagContents(void* /* PORT: was DataInput */ var1) {
		m_doubleValue = var1->readDouble();
}

unsigned char NBTTagDouble::getType() {
		return (unsigned char)6;
}

const std::string& NBTTagDouble::toString() {
		return "" + m_doubleValue;
}
