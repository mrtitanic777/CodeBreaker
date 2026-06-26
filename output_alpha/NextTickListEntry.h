#ifndef NEXTTICKLISTENTRY_H
#define NEXTTICKLISTENTRY_H

// Auto-generated from NextTickListEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Comparable.h"

class NextTickListEntry : public Comparable {
public:
	NextTickListEntry();
	NextTickListEntry(int var1, int var2, int var3, int var4);
	virtual ~NextTickListEntry();

	int xCoord;
	int yCoord;
	int zCoord;
	int blockID;
	long long m_scheduledTime;

	virtual bool equals(void* /* PORT: was Object */ var1);
	virtual int hashCode();
	virtual NextTickListEntry setScheduledTime(long long var1);
	virtual int a(NextTickListEntry var1);
	virtual int compareTo(void* /* PORT: was Object */ var1);

public: // was private
	static long long s_nextTickEntryID;
	long long m_tickEntryID;

};

#endif // NEXTTICKLISTENTRY_H