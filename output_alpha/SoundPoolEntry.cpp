#include "stdafx.h"
// Auto-generated from SoundPoolEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SoundPoolEntry.h"
#include "Common.h"
#include "URL.h"
#include <string>

SoundPoolEntry::SoundPoolEntry()
	: m_soundName("")
	, m_soundUrl(NULL)
{
}

SoundPoolEntry::SoundPoolEntry(const std::string& var1, std::string /* PORT: was java.net.URL */* var2)
	: m_soundName("")
	, m_soundUrl(NULL)
{
		m_soundName = var1;
		m_soundUrl = var2;
}

SoundPoolEntry::~SoundPoolEntry() {
	m_soundUrl = NULL;
}
