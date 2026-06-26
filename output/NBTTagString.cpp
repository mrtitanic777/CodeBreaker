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

void NBTTagString::writeTagContents(DataOutput* var1) {
		const unsigned char* var2 = (const unsigned char*)(m_stringValue).c_str();
		int var2_count = 0;
		var1->writeShort(var2_count);
		var1->write(var2, var2_count);
}

void NBTTagString::readTagContents(DataInput* var1) {
		short var2 = var1->readShort();
		unsigned char* var3 = new unsigned char[var2];
		int var3_count = 0;
		var1->readFully(var3, var3_count);
		m_stringValue = std::string((const char*)var3, var3_count);
}

unsigned char NBTTagString::getType() {
		return (unsigned char)8;
}

const std::string& NBTTagString::toString() {
		return toStr(m_stringValue);
}
