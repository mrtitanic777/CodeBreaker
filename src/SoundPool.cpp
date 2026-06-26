#include "stdafx.h"
// Auto-generated from SoundPool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SoundPool.h"
#include "Common.h"
#include "Map.h"
#include "Random.h"
#include "SoundPoolEntry.h"
#include <string>

SoundPool::SoundPool()
	: rand(new Random())
	, m_nameToSoundPoolEntriesMapping(NULL)
	, m_numberOfSoundPoolEntries(0)
{
}

SoundPool::~SoundPool() {
	rand = NULL;
	m_nameToSoundPoolEntriesMapping = NULL;
}

SoundPoolEntry SoundPool::addSound(const std::string& var1, std::string /* PORT: was java.io.File */* var2) {
  { // was try
			std::string var3 = var1;

			for(var1 = var1.substring(0, var1.indexOf(".")); Character::isDigit(var1.charAt(var1.length() - 1)); var1 = var1.substring(0, var1.length() - 1)) {
			}

			var1 = var1.replaceAll("/", ".");
			if(!m_nameToSoundPoolEntriesMapping->containsKey(var1)) {
				// PORT-TODO: m_nameToSoundPoolEntriesMapping->put(var1, /* default vector */);
			}

			SoundPoolEntry var5 = SoundPoolEntry(var3, var2->toURI().toURL());
			((std::vector<void*>)m_nameToSoundPoolEntriesMapping[var1]).push_back(var5);
			++m_numberOfSoundPoolEntries;
			return var5;
		} { DbgPrintF("CATCH MalformedURLException\n"); // was catch(MalformedURLException var4)
   // Removed Java stream/exception call: var4.printStackTrace();
			DbgPrintF("ERROR [RuntimeException]: %s\n", var4);
		}
}

SoundPoolEntry SoundPool::getRandomSoundFromSoundPool(const std::string& var1) {
		std::vector<void*> var2 = (std::vector<void*>)m_nameToSoundPoolEntriesMapping[var1];
		return var2 == NULL ? NULL : (SoundPoolEntry)var2.get(rand->nextInt(var2.size()));
}
