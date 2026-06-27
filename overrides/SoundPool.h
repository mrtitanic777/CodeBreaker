#ifndef SOUNDPOOL_H
#define SOUNDPOOL_H

// Auto-generated from SoundPool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>

class File;
class Random;
class SoundPoolEntry;

class SoundPool {
public:
	SoundPool();   // synthesized: Java field initializers (override had no ctor)
	virtual ~SoundPool();

	Random* rand;
	std::map<std::string, void*> nameToSoundPoolEntriesMapping;
	JList<void*> allSoundPoolEntries;
	int numberOfSoundPoolEntries;
	bool stripDigitsFromName;

	virtual SoundPoolEntry* addSound(const JavaString& var1, File* var2);
	virtual SoundPoolEntry* getRandomSoundFromSoundPool(const JavaString& var1);
	virtual SoundPoolEntry* getRandomSound();
};

#endif // SOUNDPOOL_H