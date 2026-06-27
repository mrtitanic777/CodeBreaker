#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NextTickListEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NextTickListEntry.h"
#include "Comparable.h"

long long NextTickListEntry::s_nextTickEntryID = 0L;

NextTickListEntry::NextTickListEntry(int var1, int var2, int var3, int var4)
	: xCoord(0)
	, yCoord(0)
	, zCoord(0)
	, blockID(0)
	, scheduledTime(0)
	, tickEntryID(0)
{
	this->tickEntryID = s_nextTickEntryID++;
	this->xCoord = var1;
	this->yCoord = var2;
	this->zCoord = var3;
	this->blockID = var4;
}

NextTickListEntry::~NextTickListEntry()
{
}

bool NextTickListEntry::equals(JObject var1)
{
	if(!(((var1).is<NextTickListEntry>()))) {
	return false;
	} else {
	NextTickListEntry* var2 = (NextTickListEntry*)var1;
	return this->xCoord == var2->xCoord && this->yCoord == var2->yCoord && this->zCoord == var2->zCoord && this->blockID == var2->blockID;
	}
}

int NextTickListEntry::hashCode()
{
	return (this->xCoord * 128 * 1024 + this->zCoord * 128 + this->yCoord) * 256 + this->blockID;
}

NextTickListEntry* NextTickListEntry::setScheduledTime(long long var1)
{
	this->scheduledTime = var1;
	return this;
}

int NextTickListEntry::a(NextTickListEntry* var1)
{
	return this->scheduledTime < var1->scheduledTime ? -1 : (this->scheduledTime > var1->scheduledTime ? 1 : (this->tickEntryID < var1->tickEntryID ? -1 : (this->tickEntryID > var1->tickEntryID ? 1 : 0)));
}

int NextTickListEntry::compareTo(JObject var1)
{
	return this->a((NextTickListEntry*)var1);
}

void NextTickListEntry::initStatics()
{
}
