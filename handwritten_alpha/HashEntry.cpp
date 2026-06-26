#include "stdafx.h"
// Auto-generated from HashEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "HashEntry.h"
#include "Common.h"
#include <string>
#include <sstream>
#include "MCHashTable.h"

HashEntry::HashEntry()
	: m_hashEntry(0)
	, m_valueEntry(NULL)
	, m_nextEntry(NULL)
	, m_slotHash(0)
{
}

HashEntry::HashEntry(int var1, int var2, void* /* PORT: was Object */ var3, HashEntry* var4)
	: m_hashEntry(0)
	, m_valueEntry(NULL)
	, m_nextEntry(NULL)
	, m_slotHash(0)
{
		m_valueEntry = var3;
		m_nextEntry = var4;
		m_hashEntry = var2;
		m_slotHash = var1;
}

HashEntry::~HashEntry() {
	m_valueEntry = NULL;
	m_nextEntry = NULL;
}

int HashEntry::getHash() {
		return m_hashEntry;
}

void* /* PORT: was Object */ HashEntry::getValue() {
		return m_valueEntry;
}

bool HashEntry::equals(void* /* PORT: was Object */ var1) {
		HashEntry* var2 = (HashEntry*)var1;
		if(var2 == NULL) {
			return false;
		} else {
			int var3 = getHash();
			int var4 = var2->getHash();
			if(var3 == var4) {
				void* var5 = getValue();
				void* var6 = var2->getValue();
				if(var5 == var6) {
					return true;
				}
			}

			return false;
		}
}

int HashEntry::hashCode() {
		// PORT: MCHashTable::getHash is a non-static member here; replicate the
		// static spreader inline (same logic as MCHashTable::computeHash).
		int var0 = m_hashEntry;
		var0 ^= (int)((unsigned int)var0 >> 20) ^ (int)((unsigned int)var0 >> 12);
		return var0 ^ (int)((unsigned int)var0 >> 7) ^ (int)((unsigned int)var0 >> 4);
}

std::string HashEntry::toString() {
		static std::string s_result;
		std::stringstream ss;
		ss << getHash() << "=" << getValue();
		s_result = ss.str();
		return s_result;
}
