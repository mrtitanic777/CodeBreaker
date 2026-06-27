#ifndef HASHENTRY_H
#define HASHENTRY_H

// Auto-generated from HashEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class HashEntry {
public:
	HashEntry(int var1, int var2, void* var3, HashEntry* var4);
	virtual ~HashEntry();

	int hashEntry;
	void* valueEntry;
	HashEntry* nextEntry;
	int slotHash;

	virtual int getHash();
	virtual void* getValue();
	virtual bool equals(void* var1);
	virtual int hashCode();
	virtual JavaString toString();
};

#endif // HASHENTRY_H