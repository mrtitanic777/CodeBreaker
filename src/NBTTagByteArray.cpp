#include "stdafx.h"
// Auto-generated from NBTTagByteArray.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagByteArray.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagByteArray::NBTTagByteArray()
	: m_byteArray(0)
{
}

NBTTagByteArray::NBTTagByteArray(unsigned char* var1)
	: m_byteArray(0)
{
		m_byteArray = var1;
}

NBTTagByteArray::~NBTTagByteArray() {
	delete[] m_byteArray;
	m_byteArray = NULL;
}

void NBTTagByteArray::writeTagContents(void* /* PORT: was DataOutput */ var1) {
		var1->writeInt(m_byteArray /* PORT: array length - use tracked size */);
		var1->write(m_byteArray);
}

void NBTTagByteArray::readTagContents(void* /* PORT: was DataInput */ var1) {
		int var2 = var1->readInt();
		m_byteArray = new unsigned char[var2];
		var1->readFully(m_byteArray);
}

unsigned char NBTTagByteArray::getType() {
		return (unsigned char)7;
}

const std::string& NBTTagByteArray::toString() {
		return "[" + m_byteArray /* PORT: array length - use tracked size */ + " bytes]";
}
