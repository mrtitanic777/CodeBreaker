#include "stdafx.h"
// Auto-generated from NextTickListEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NextTickListEntry.h"
#include "Common.h"

NextTickListEntry::NextTickListEntry()
	: m_xCoord(0)
	, m_yCoord(0)
	, m_zCoord(0)
	, blockID(0)
	, m_scheduledTime(0)
{
}

NextTickListEntry::NextTickListEntry(int var1, int var2, int var3, int var4)
	: m_xCoord(0)
	, m_yCoord(0)
	, m_zCoord(0)
	, blockID(0)
	, m_scheduledTime(0)
{
		m_xCoord = var1;
		m_yCoord = var2;
		m_zCoord = var3;
		blockID = var4;
}

NextTickListEntry::~NextTickListEntry() {
	// No dynamic resources
}
