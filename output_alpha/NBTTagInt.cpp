#include "stdafx.h"
// Auto-generated from NBTTagInt.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagInt.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagInt::NBTTagInt()
	: m_intValue(0)
{
}

NBTTagInt::NBTTagInt(int var1)
	: m_intValue(0)
{
		m_intValue = var1;
}

NBTTagInt::~NBTTagInt() {
	// No dynamic resources
}

void NBTTagInt::writeTagContents(DataOutput* var1) {
		var1->writeInt(m_intValue);
}

void NBTTagInt::readTagContents(DataInput* var1) {
		m_intValue = var1->readInt();
}

unsigned char NBTTagInt::getType() {
		return (unsigned char)3;
}

std::string NBTTagInt::toString() {
		return toStr(m_intValue);
}
