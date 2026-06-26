#include "stdafx.h"
// Auto-generated from NBTTagFloat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagFloat.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagFloat::NBTTagFloat()
	: m_floatValue(0)
{
}

NBTTagFloat::NBTTagFloat(float var1)
	: m_floatValue(0)
{
		m_floatValue = var1;
}

NBTTagFloat::~NBTTagFloat() {
	// No dynamic resources
}

void NBTTagFloat::writeTagContents(DataOutput* var1) {
		var1->writeFloat(m_floatValue);
}

void NBTTagFloat::readTagContents(DataInput* var1) {
		m_floatValue = var1->readFloat();
}

unsigned char NBTTagFloat::getType() {
		return (unsigned char)5;
}

const std::string& NBTTagFloat::toString() {
		return "" + m_floatValue;
}
