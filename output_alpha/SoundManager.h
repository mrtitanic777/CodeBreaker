#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

// Auto-generated from SoundManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class EntityLiving;
class GameSettings;
class Random;
class SoundPool;
class SoundSystem;

class SoundManager {
public:
	SoundManager();
	virtual ~SoundManager();

	virtual void func_340_a(GameSettings* var1);
	virtual void onSoundOptionsChanged();
	virtual void closeMinecraft();
	virtual void func_6372_a(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
	virtual void addStreaming(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
	virtual void addMusic(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
	virtual void func_4033_c();
	virtual void func_338_a(EntityLiving* var1, float var2);
	virtual void func_331_a(const std::string& var1, float var2, float var3, float var4, float var5, float var6);
	virtual void func_336_b(const std::string& var1, float var2, float var3, float var4, float var5, float var6);
	virtual void func_337_a(const std::string& var1, float var2, float var3);

public: // was private
	static SoundSystem* s_sndSystem;
	SoundPool* m_soundPoolSounds;
	SoundPool* m_soundPoolStreaming;
	SoundPool* m_soundPoolMusic;
	int m_field_587_e;
	GameSettings* m_options;
	static bool s_loaded;
	Random* rand;
	int m_field_583_i;

	void tryToSetLibraryAndCodecs();

};

#endif // SOUNDMANAGER_H