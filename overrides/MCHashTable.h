#ifndef MCHASHTABLE_H
#define MCHASHTABLE_H

// Auto-generated from MCHashTable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class HashEntry;

class MCHashTable {
public:
	MCHashTable();   // synthesized: Java field initializers (override had no ctor)
	virtual ~MCHashTable();

	JArray<HashEntry* > slots;
	int count;
	int threshold;
	float growFactor;
	int versionStamp;

	static int computeHash(int var0);
	static int getSlotIndex(int var0, int var1);
	virtual void* lookup(int var1);
	virtual void addKey(int var1, void* var2);
	virtual void grow(int var1);
	virtual void copyTo(JArray<HashEntry* > var1);
	virtual void* removeObject(int var1);
	virtual HashEntry* removeEntry(int var1);
	virtual void clearMap();
	virtual void insert(int var1, int var2, void* var3, int var4);
	static int getHash(int var0);
};

#endif // MCHASHTABLE_H