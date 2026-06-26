#include "stdafx.h"
// Auto-generated from NibbleArray.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NibbleArray.h"
#include "Common.h"

NibbleArray::NibbleArray()
	: m_data(0)
{
}

NibbleArray::NibbleArray(int var1)
	: m_data(0)
{
		m_data = new unsigned char[var1 >> 1];
}

NibbleArray::NibbleArray(unsigned char* var1)
	: m_data(0)
{
		m_data = var1;
}

NibbleArray::~NibbleArray() {
	delete[] m_data;
	m_data = NULL;
}

int NibbleArray::getNibble(int var1, int var2, int var3) {
		int var4 = var1 << 11 | var3 << 7 | var2;
		int var5 = var4 >> 1;
		int var6 = var4 & 1;
		return var6 == 0 ? m_data[var5] & 15 : m_data[var5] >> 4 & 15;
}

void NibbleArray::setNibble(int var1, int var2, int var3, int var4) {
		int var5 = var1 << 11 | var3 << 7 | var2;
		int var6 = var5 >> 1;
		int var7 = var5 & 1;
		if(var7 == 0) {
			m_data[var6] = (unsigned char)(m_data[var6] & 240 | var4 & 15);
		} else {
			m_data[var6] = (unsigned char)(m_data[var6] & 15 | (var4 & 15) << 4);
		}

}

bool NibbleArray::isValid() {
		return m_data != NULL;
}
