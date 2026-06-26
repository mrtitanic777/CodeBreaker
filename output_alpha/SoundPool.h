#ifndef SOUNDPOOL_H
#define SOUNDPOOL_H

// Auto-generated from SoundPool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>
#include <vector>

class Map;
class Random;
class SoundPoolEntry;

class SoundPool {
public:
	SoundPool();
	virtual ~SoundPool();

	int m_numberOfSoundPoolEntries;
	bool m_field_1657_b;

	virtual SoundPoolEntry addSound(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
	virtual SoundPoolEntry getRandomSoundFromSoundPool(const std::string& var1);
	virtual SoundPoolEntry getRandomSound();

public: // was private
	Random* rand;
	std::map<std::string, void*>* m_nameToSoundPoolEntriesMapping;
	std::vector<void*> /* PORT: determine element type */ m_allSoundPoolEntries;

};

#endif // SOUNDPOOL_H