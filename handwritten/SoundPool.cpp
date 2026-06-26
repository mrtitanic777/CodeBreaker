#include "stdafx.h"
// Hand-written PORT override for SoundPool.java
// ---------------------------------------------
// SoundPool/SoundPoolEntry are part of Java's Paul Lamb SoundSystem path (build
// name->file URL pools from java.io.File). On Xbox 360 audio is served by XACT3
// prebuilt wave/sound banks (see XactAudio.cpp + the SoundManager bridge), so the
// pool is vestigial. These no-op stubs preserve the converted SoundPool.h
// interface and translation unit without the unportable java.io/URL machinery.

#include "SoundPool.h"
#include "SoundPoolEntry.h"

SoundPool::SoundPool()
	: rand(NULL)
	, m_nameToSoundPoolEntriesMapping(NULL)
	, m_numberOfSoundPoolEntries(0)
{
}

SoundPool::~SoundPool() {
	rand = NULL;
	m_nameToSoundPoolEntriesMapping = NULL;
}

SoundPoolEntry SoundPool::addSound(const std::string& var1, std::string* var2) {
	(void)var1; (void)var2;
	return SoundPoolEntry();
}

SoundPoolEntry SoundPool::getRandomSoundFromSoundPool(const std::string& var1) {
	(void)var1;
	return SoundPoolEntry();
}
