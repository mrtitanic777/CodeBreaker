#include "stdafx.h"
// Auto-generated from NBTTagString.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagString.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagString::NBTTagString()
	: m_stringValue("")
{
}

NBTTagString::NBTTagString(const std::string& var1)
	: m_stringValue("")
{
		m_stringValue = var1;
		if(var1.empty()) {
			DbgPrintF("ERROR [IllegalArgumentException]: %s\n", "Empty string not allowed");
		}
}

NBTTagString::~NBTTagString() {
	// No dynamic resources
}

void NBTTagString::writeTagContents(DataOutput* var1) {
		var1->writeUTF(m_stringValue);
}

void NBTTagString::readTagContents(DataInput* var1) {
		m_stringValue = var1->readUTF();
}

unsigned char NBTTagString::getType() {
		return (unsigned char)8;
}

std::string NBTTagString::toString() {
		return m_stringValue;
}
