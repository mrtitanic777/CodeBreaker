#ifndef HASHENTRY_H
#define HASHENTRY_H

// Auto-generated from HashEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class HashEntry {
public:
	HashEntry();
	HashEntry(int var1, int var2, void* /* PORT: was Object */ var3, HashEntry* var4);
	virtual ~HashEntry();

	virtual int getHash();
	virtual void* /* PORT: was Object */ getValue();
	virtual bool equals(void* /* PORT: was Object */ var1);
	virtual int hashCode();
	virtual std::string toString();

public: // was private
	int m_hashEntry;
	void* /* PORT: was Object */ m_valueEntry;
	HashEntry* m_nextEntry;
	int m_slotHash;


};

#endif // HASHENTRY_H