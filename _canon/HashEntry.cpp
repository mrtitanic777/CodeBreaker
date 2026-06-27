#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from HashEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "HashEntry.h"
#include "MCHashTable.h"

HashEntry::HashEntry(int var1, int var2, void* var3, HashEntry* var4)
	: hashEntry(0)
	, valueEntry(NULL)
	, nextEntry(NULL)
	, slotHash(0)
{
	this->valueEntry = var3;
	this->nextEntry = var4;
	this->hashEntry = var2;
	this->slotHash = var1;
}

HashEntry::~HashEntry()
{
}

int HashEntry::getHash()
{
	return this->hashEntry;
}

void* HashEntry::getValue()
{
	return this->valueEntry;
}

bool HashEntry::equals(void* var1)
{
	if(!(((var1) != NULL))) {
	return false;
	} else {
	HashEntry* var2 = (HashEntry*)var1;
	// Java boxed the int keys (Integer.valueOf) and compared with Integer.equals;
	// for ints that reduces to value equality.
	int var3 = Integer::valueOf(this->getHash());
	int var4 = Integer::valueOf(var2->getHash());
	if(var3 == var4) {
	void* var5 = this->getValue();
	void* var6 = var2->getValue();
	// Values are Object (void*); Object.equals here reduces to reference identity.
	if(var5 == var6) {
	return true;
	}
	}
	return false;
	}
}

int HashEntry::hashCode()
{
	return MCHashTable::getHash(this->hashEntry);
}

JavaString HashEntry::toString()
{
	return (JCat() + this->getHash() + "=" + this->getValue());
}
