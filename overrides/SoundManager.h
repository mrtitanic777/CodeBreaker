#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

// Auto-generated from SoundManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class EntityLiving;
class File;
class GameSettings;
class Random;
class SoundPool;
class SoundSystem;

class SoundManager {
public:
	SoundManager();                 // constructs the SoundPools (Java `= new SoundPool()` field inits the override dropped)
	virtual ~SoundManager();

	SoundPool* soundPoolSounds;
	SoundPool* soundPoolStreaming;
	SoundPool* soundPoolMusic;
	int soundCounter;
	GameSettings* options;
	Random* rand;
	int nextMusicDelay;
	static SoundSystem* s_sndSystem;
	static bool s_loaded;

	virtual void loadSoundSettings(GameSettings* var1);
	virtual void tryToSetLibraryAndCodecs();
	virtual void onSoundOptionsChanged();
	virtual void closeMinecraft();
	virtual void addSound(const JavaString& var1, File* var2);
	virtual void addStreaming(const JavaString& var1, File* var2);
	virtual void addMusic(const JavaString& var1, File* var2);
	virtual void playRandomMusic();
	virtual void setListener(EntityLiving* var1, float var2);
	virtual void playStreaming(const JavaString& var1, float var2, float var3, float var4, float var5, float var6);
	virtual void playSound(const JavaString& var1, float var2, float var3, float var4, float var5, float var6);
	virtual void playSoundFX(const JavaString& var1, float var2, float var3);
};

#endif // SOUNDMANAGER_H