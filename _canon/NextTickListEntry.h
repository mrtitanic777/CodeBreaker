#ifndef NEXTTICKLISTENTRY_H
#define NEXTTICKLISTENTRY_H

// Auto-generated from NextTickListEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"
#include "Comparable.h"

class NextTickListEntry : public Comparable {
public:
	NextTickListEntry(int var1, int var2, int var3, int var4);
	virtual ~NextTickListEntry();

	int xCoord;
	int yCoord;
	int zCoord;
	int blockID;
	long long scheduledTime;
	long long tickEntryID;
	static long long s_nextTickEntryID;

	virtual bool equals(JObject var1);
	virtual int hashCode();
	virtual NextTickListEntry* setScheduledTime(long long var1);
	virtual int a(NextTickListEntry* var1);
	virtual int compareTo(JObject var1);

	static void initStatics();
};

#endif // NEXTTICKLISTENTRY_H