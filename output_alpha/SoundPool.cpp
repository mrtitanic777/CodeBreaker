#include "stdafx.h"
// Hand-written PORT override for SoundPool.java (Minecraft Alpha v1.2.6)
// --------------------------------------------------------------------
// Java SoundPool is the Paul Lamb SoundSystem registry: it walks java.io.File trees
// and maps logical sound names ("step.grass", "random.bow", ...) to lists of
// SoundPoolEntry (name + file URL), then hands back a random entry per cue. On the
// Xbox 360 the audio is served by prebuilt XACT3 wave/sound banks (see XactAudio.cpp +
// the SoundManager bridge), so there is no filesystem to enumerate and no URL to build.
//
// This shim keeps the FULL converted SoundPool.h interface AND the authentic registry
// behaviour: callers can register entries by logical name (cue) and pull a random one,
// exactly like the indev SoundPool. The only thing stubbed out is the unportable
// java.io/URL discovery -- entries register by logical name, never by scanning files.
//
// Storage:
//   m_nameToSoundPoolEntriesMapping : std::map<std::string, void*>* where each value is a
//       heap std::vector<SoundPoolEntry*>* (the Java Map<String, List> per-name bucket).
//   m_allSoundPoolEntries          : std::vector<void*> of SoundPoolEntry* (Java's flat list).
// We own the SoundPoolEntry* and the per-name vectors and free them in the dtor.

#include "SoundPool.h"
#include "SoundPoolEntry.h"
#include "Common.h"   // Random (defined here per Random.h)

#include <map>
#include <vector>
#include <cctype>   // isdigit

typedef std::vector<SoundPoolEntry*> EntryList;
typedef std::map<std::string, void*> NameMap;

SoundPool::SoundPool()
	: m_numberOfSoundPoolEntries(0)
	, m_field_1657_b(true)
	, rand(new Random())
	, m_nameToSoundPoolEntriesMapping(new NameMap())
{
}

SoundPool::~SoundPool() {
	// Free the per-name buckets and every entry we own (entries live once in
	// m_allSoundPoolEntries; the per-name buckets only borrow the same pointers).
	if (m_nameToSoundPoolEntriesMapping != NULL) {
		for (NameMap::iterator it = m_nameToSoundPoolEntriesMapping->begin();
			it != m_nameToSoundPoolEntriesMapping->end(); ++it) {
			delete (EntryList*)it->second;
		}
		delete m_nameToSoundPoolEntriesMapping;
		m_nameToSoundPoolEntriesMapping = NULL;
	}

	for (size_t i = 0; i < m_allSoundPoolEntries.size(); ++i) {
		delete (SoundPoolEntry*)m_allSoundPoolEntries[i];
	}
	m_allSoundPoolEntries.clear();

	delete rand;
	rand = NULL;
}

// Register a sound under a logical cue derived from var1.
// var2 is the (unused) source file -- on 360 there is no filesystem URL, so it is
// ignored and the entry carries a NULL URL. Faithfully reproduces the Java name
// normalisation: strip the extension, strip trailing digits (when field_1657_b),
// and turn '/' into '.'.
SoundPoolEntry SoundPool::addSound(const std::string& var1, std::string* var2) {
	(void)var2;  // PORT: java.io.File / URL discovery is not used on Xbox 360.

	const std::string fullName = var1;  // keep the original as the entry's sound name

	// var1 = var1.substring(0, var1.indexOf("."))
	std::string name = var1;
	std::string::size_type dot = name.find('.');
	if (dot != std::string::npos) {
		name = name.substr(0, dot);
	}

	// while(field_1657_b && Character.isDigit(last char)) drop the trailing digit.
	if (m_field_1657_b) {
		while (!name.empty() &&
			   isdigit((unsigned char)name[name.size() - 1])) {
			name.erase(name.size() - 1);
		}
	}

	// name = name.replaceAll("/", ".")
	for (std::string::size_type i = 0; i < name.size(); ++i) {
		if (name[i] == '/') {
			name[i] = '.';
		}
	}

	NameMap* nameMap = (NameMap*)m_nameToSoundPoolEntriesMapping;
	if (nameMap == NULL) {
		// Defensive: should never happen, but keep the registry alive.
		nameMap = new NameMap();
		m_nameToSoundPoolEntriesMapping = nameMap;
	}

	// if(!containsKey(name)) put(name, new ArrayList())
	NameMap::iterator it = nameMap->find(name);
	if (it == nameMap->end()) {
		it = nameMap->insert(std::make_pair(name, (void*)new EntryList())).first;
	}

	// SoundPoolEntry var4 = new SoundPoolEntry(fullName, url) -- url is NULL on 360.
	SoundPoolEntry* entry = new SoundPoolEntry(fullName, (std::string*)NULL);

	((EntryList*)it->second)->push_back(entry);   // per-name bucket
	m_allSoundPoolEntries.push_back((void*)entry);  // flat list (owns the pointer)
	++m_numberOfSoundPoolEntries;

	// Java returns the entry by value; mirror that (header returns SoundPoolEntry).
	return *entry;
}

// Return a random entry registered under the given cue, or a default-constructed
// (empty) entry when the cue is unknown. Java returns null here; the converted
// signature returns by value, so an empty SoundPoolEntry is the C++03 equivalent.
SoundPoolEntry SoundPool::getRandomSoundFromSoundPool(const std::string& var1) {
	NameMap* nameMap = (NameMap*)m_nameToSoundPoolEntriesMapping;
	if (nameMap == NULL) {
		return SoundPoolEntry();
	}

	NameMap::iterator it = nameMap->find(var1);
	if (it == nameMap->end()) {
		return SoundPoolEntry();  // Java: var2 == null ? null
	}

	EntryList* list = (EntryList*)it->second;
	if (list == NULL || list->empty()) {
		return SoundPoolEntry();
	}

	int idx = rand->nextInt((int)list->size());
	return *(*list)[idx];
}

// Return a random entry from the whole pool, or an empty entry when nothing is
// registered (Java returns null when allSoundPoolEntries.size() == 0).
SoundPoolEntry SoundPool::getRandomSound() {
	if (m_allSoundPoolEntries.empty()) {
		return SoundPoolEntry();
	}

	int idx = rand->nextInt((int)m_allSoundPoolEntries.size());
	return *(SoundPoolEntry*)m_allSoundPoolEntries[idx];
}
