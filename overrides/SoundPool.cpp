#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from SoundPool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SoundPool.h"
#include "File.h"
#include "MalformedURLException.h"
#include "Random.h"
#include "RuntimeException.h"
#include "SoundPoolEntry.h"
#include "URL.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  SoundPoolEntry var4 = new SoundPoolEntry(var3, var2.toURI().toURL());
 *  ---- end original Java ---- */
// java.io.File.toURI().toURL(): the File runtime stub returns void* with no URI/URL
// bridge, so build the file:// URL from the path locally. See "runtime-stub gaps".
static URL* fileToURL(File* f) { JavaString u = (JCat() + "file:" + f->getPath()); return new URL(u); }

SoundPool::SoundPool()
	: rand(NULL)
	, numberOfSoundPoolEntries(0)
	, stripDigitsFromName(0)
{
	this->rand = new Random();
	this->stripDigitsFromName = true;
}

SoundPool::~SoundPool()
{
}

SoundPoolEntry* SoundPool::addSound(const JavaString& var1Param, File* var2)
{
	JavaString var1 = var1Param;
	try {
	JavaString var3 = var1;
	var1 = var1.substr(0, (var1.indexOf(".")) - (0));
	if(this->stripDigitsFromName) {
	while(Character::isDigit(var1[var1.length() - 1])) {
	var1 = var1.substr(0, (var1.length() - 1) - (0));
	}
	}
	var1 = var1.replaceAll("/", ".");
	if(!this->nameToSoundPoolEntriesMapping.count(var1)) {
	this->nameToSoundPoolEntriesMapping[var1] = (void*)(new JList<void*>());
	}
	SoundPoolEntry* var4 = new SoundPoolEntry(var3, fileToURL(var2));
	((JList<void*>*)this->nameToSoundPoolEntriesMapping[var1])->add(var4);
	this->allSoundPoolEntries.push_back(var4);
	++this->numberOfSoundPoolEntries;
	return var4;
	} catch (MalformedURLException var5) {
	var5.printStackTrace();
	throw new RuntimeException(var5);
	}
}

SoundPoolEntry* SoundPool::getRandomSoundFromSoundPool(const JavaString& var1)
{
	JList<void*>* var2 = this->nameToSoundPoolEntriesMapping.count(var1)
	                     ? (JList<void*>*)this->nameToSoundPoolEntriesMapping[var1] : NULL;
	return var2 == NULL ? NULL : (SoundPoolEntry*)(*var2)[this->rand->nextInt((int)var2->size())];
}

SoundPoolEntry* SoundPool::getRandomSound()
{
	return this->allSoundPoolEntries.size() == 0 ? NULL : (SoundPoolEntry*)this->allSoundPoolEntries[this->rand->nextInt(this->allSoundPoolEntries.size())];
}
