#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

// Auto-generated from SoundManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class EntityLiving;
class GameSettings;
class SoundPool;
class SoundSystem;

class SoundManager {
public:
	SoundManager();
	virtual ~SoundManager();

	virtual void loadSoundSettings(GameSettings* var1);
	virtual void onSoundOptionsChanged();
	virtual void closeMinecraft();
	virtual void addSound(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
	virtual void addMusic(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
	virtual void playRandomMusicIfReady(float var1, float var2, float var3);
	virtual void setListener(EntityLiving* var1, float var2);
	virtual void playSound(const std::string& var1, float var2, float var3, float var4, float var5, float var6);
	virtual void playSoundFX(const std::string& var1, float var2, float var3);

private:
	SoundSystem* m_sndSystem;
	SoundPool* m_soundPoolSounds;
	SoundPool* m_soundPoolMusic;
	int m_latestSoundID;
	GameSettings* m_options;
	bool m_loaded;

	void tryToSetLibraryAndCodecs();

};

#endif // SOUNDMANAGER_H