#include "stdafx.h"
// Auto-generated from NBTTagString.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagString.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagString::NBTTagString()
{
}

NBTTagString::NBTTagString(const std::string& var1)
{
		m_stringValue = var1;
}

NBTTagString::~NBTTagString() {
	// No dynamic resources
}

void NBTTagString::writeTagContents(void* /* PORT: was DataOutput */ var1) {
		unsigned char[] var2 = m_stringValue.getBytes("UTF-8");
		var1->writeShort(var2 /* PORT: array length - use tracked size */);
		var1->write(var2);
}

void NBTTagString::readTagContents(void* /* PORT: was DataInput */ var1) {
		short var2 = var1->readShort();
		unsigned char[] var3 = new unsigned char[var2];
		var1->readFully(var3);
		m_stringValue = new std::string(var3, "UTF-8");
}

unsigned char NBTTagString::getType() {
		return (unsigned char)8;
}

const std::string& NBTTagString::toString() {
		return "" + m_stringValue;
}
