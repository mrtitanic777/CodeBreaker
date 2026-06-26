#ifndef MCHASHTABLE_H
#define MCHASHTABLE_H

// Auto-generated from MCHashTable.java
// Target: Xbox 360 / C++03 / DirectX 9

class HashEntry;

class MCHashTable {
public:
	MCHashTable();
	virtual ~MCHashTable();

	virtual void* /* PORT: was Object */ lookup(int var1);
	virtual void addKey(int var1, void* /* PORT: was Object */ var2);
	virtual void* /* PORT: was Object */ removeObject(int var1);
	virtual void clearMap();

public: // was private
	int m_threshold;
	float m_growFactor;

	int computeHash(int var0);
	int getSlotIndex(int var0, int var1);
	void grow(int var1);
	void copyTo(HashEntry* var1);
	HashEntry* removeEntry(int var1);
	void insert(int var1, int var2, void* /* PORT: was Object */ var3, int var4);
	int getHash(int var0);

};

#endif // MCHASHTABLE_H