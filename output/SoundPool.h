#ifndef SOUNDPOOL_H
#define SOUNDPOOL_H

// Auto-generated from SoundPool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class Map;
class Random;
class SoundPoolEntry;

class SoundPool {
public:
	SoundPool();
	virtual ~SoundPool();

	virtual SoundPoolEntry addSound(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
	virtual SoundPoolEntry getRandomSoundFromSoundPool(const std::string& var1);

private:
	Random* rand;
	Map* m_nameToSoundPoolEntriesMapping;
	int m_numberOfSoundPoolEntries;

};

#endif // SOUNDPOOL_H