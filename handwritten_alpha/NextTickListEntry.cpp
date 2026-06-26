#include "stdafx.h"
// Auto-generated from NextTickListEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NextTickListEntry.h"
#include "Common.h"

long long NextTickListEntry::s_nextTickEntryID = 0LL;

NextTickListEntry::NextTickListEntry()
	: xCoord(0)
	, yCoord(0)
	, zCoord(0)
	, blockID(0)
	, m_scheduledTime(0)
	, m_tickEntryID(s_nextTickEntryID++)
{
}

NextTickListEntry::NextTickListEntry(int var1, int var2, int var3, int var4)
	: xCoord(0)
	, yCoord(0)
	, zCoord(0)
	, blockID(0)
	, m_scheduledTime(0)
	, m_tickEntryID(s_nextTickEntryID++)
{
		xCoord = var1;
		yCoord = var2;
		zCoord = var3;
		blockID = var4;
}

NextTickListEntry::~NextTickListEntry() {
	// No dynamic resources
}

bool NextTickListEntry::equals(void* /* PORT: was Object */ var1) {
		NextTickListEntry* var2 = (NextTickListEntry*)var1;
		if(var2 == NULL) {
			return false;
		} else {
			return xCoord == var2->xCoord && yCoord == var2->yCoord && zCoord == var2->zCoord && blockID == var2->blockID;
		}
}

int NextTickListEntry::hashCode() {
		return (xCoord * 128 * 1024 + zCoord * 128 + yCoord) * 256 + blockID;
}

NextTickListEntry NextTickListEntry::setScheduledTime(long long var1) {
		m_scheduledTime = var1;
		return *this;
}

int NextTickListEntry::a(NextTickListEntry var1) {
		return m_scheduledTime < var1.m_scheduledTime ? -1 : (m_scheduledTime > var1.m_scheduledTime ? 1 : (m_tickEntryID < var1.m_tickEntryID ? -1 : (m_tickEntryID > var1.m_tickEntryID ? 1 : 0)));
}

int NextTickListEntry::compareTo(void* /* PORT: was Object */ var1) {
		return a(*(NextTickListEntry*)var1);
}
