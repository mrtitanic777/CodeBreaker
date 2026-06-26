#include "stdafx.h"
// Auto-generated from SoundManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SoundManager.h"
#include "Common.h"
#include "EntityLiving.h"
#include "GameSettings.h"
#include "SoundPool.h"
#include "SoundSystem.h"
#include <string>

SoundManager::SoundManager()
	: m_sndSystem(NULL)
	, m_soundPoolSounds(new SoundPool())
	, m_soundPoolMusic(new SoundPool())
	, m_latestSoundID(0)
	, m_options(NULL)
	, m_loaded(false)
{
}

SoundManager::~SoundManager() {
	m_sndSystem = NULL;
	m_soundPoolSounds = NULL;
	m_soundPoolMusic = NULL;
	m_options = NULL;
}

void SoundManager::loadSoundSettings(GameSettings* var1) {
		m_options = var1;
		if(!m_loaded && (var1->m_sound || var1->m_music)) {
			tryToSetLibraryAndCodecs();
		}

}

void SoundManager::tryToSetLibraryAndCodecs() {
  { // was try
			bool var1 = m_options->m_sound;
			bool var2 = m_options->m_music;
			m_options->m_sound = false;
			m_options->m_music = false;
			m_options->saveOptions();
			// PORT-BLOCKED: SoundSystemConfig::addLibrary(LibraryLWJGLOpenAL.class);
			// PORT-BLOCKED: SoundSystemConfig::setCodec("ogg", CodecJOrbis.class);
			// PORT-BLOCKED: SoundSystemConfig::setCodec("wav", CodecWav.class);
			m_sndSystem = new SoundSystem();
			m_options->m_sound = var1;
			m_options->m_music = var2;
			m_options->saveOptions();
		} { DbgPrintF("CATCH Throwable\n"); // was catch(Throwable var3)
			DbgPrintF("%s\n", "error linking with the LibraryJavaSound plug-in");
		}

		m_loaded = true;
}

void SoundManager::onSoundOptionsChanged() {
		if(!m_loaded && (m_options->m_sound || m_options->m_music)) {
			tryToSetLibraryAndCodecs();
		}

		if(!m_options->m_music) {
			m_sndSystem->stop("BgMusic");
		}

}

void SoundManager::closeMinecraft() {
		if(m_loaded) {
			m_sndSystem->cleanup();
		}

}

void SoundManager::addSound(const std::string& var1, std::string /* PORT: was java.io.File */* var2) {
		m_soundPoolSounds->addSound(var1, var2);
}

void SoundManager::addMusic(const std::string& var1, std::string /* PORT: was java.io.File */* var2) {
		m_soundPoolMusic->addSound(var1, var2);
}

void SoundManager::playRandomMusicIfReady(float var1, float var2, float var3) {
		if(m_loaded && m_options->m_music) {
			if(!m_sndSystem->playing("BgMusic")) {
				SoundPoolEntry var4 = m_soundPoolMusic->getRandomSoundFromSoundPool("calm");
				m_sndSystem->newStreamingSource(true, "BgMusic", var4.m_soundUrl, var4.m_soundName, false, var1, var2, var3, 2, 32.0F);
				m_sndSystem->play("BgMusic");
			}

		}
}

void SoundManager::setListener(EntityLiving* var1, float var2) {
		if(m_loaded && m_options->m_sound) {
			if(var1 != NULL) {
				float var3 = var1->prevRotationPitch + (var1->rotationPitch - var1->prevRotationPitch) * var2;
				float var4 = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var2;
				float var5 = var1->prevPosX + (var1->posX - var1->prevPosX) * var2;
				float var6 = var1->prevPosY + (var1->posY - var1->prevPosY) * var2;
				float var10 = var1->prevPosZ + (var1->posZ - var1->prevPosZ) * var2;
				var2 = cosf(-var4 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
				var4 = sinf(-var4 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
				float var7 = cosf(-var3 * (3.14159265358979323846f / 180.0F));
				var3 = sinf(-var3 * (3.14159265358979323846f / 180.0F));
				float var8 = var4 * var7;
				float var9 = var2 * var7;
				var4 *= var3;
				var2 *= var3;
				m_sndSystem->setListenerPosition(var5, var6, var10);
				m_sndSystem->setListenerOrientation(var8, var3, var9, var4, var7, var2);
			}
		}
}

void SoundManager::playSound(const std::string& var1, float var2, float var3, float var4, float var5, float var6) {
		if(m_loaded && m_options->m_sound) {
			SoundPoolEntry var9 = m_soundPoolSounds->getRandomSoundFromSoundPool(var1);
			if(var9 != NULL && var5 > 0.0F) {
				m_latestSoundID = (m_latestSoundID + 1) % 256;
				std::string var7 = "sound_" + m_latestSoundID;
				float var8 = 16.0F;
				if(var5 > 1.0F) {
					var8 = 16.0F * var5;
				}

				m_sndSystem->newSource(var5 > 1.0F, var7, var9.m_soundUrl, var9.m_soundName, false, var2, var3, var4, 2, var8);
				m_sndSystem->setPitch(var7, var6);
				if(var5 > 1.0F) {
					var5 = 1.0F;
				}

				m_sndSystem->setVolume(var7, var5);
				m_sndSystem->play(var7);
			}

		}
}

void SoundManager::playSoundFX(const std::string& var1, float var2, float var3) {
		if(m_loaded && m_options->m_sound) {
			SoundPoolEntry var4 = m_soundPoolSounds->getRandomSoundFromSoundPool(var1);
			if(var4 != NULL) {
				m_latestSoundID = (m_latestSoundID + 1) % 256;
				std::string var5 = "sound_" + m_latestSoundID;
				m_sndSystem->newSource(false, var5, var4.m_soundUrl, var4.m_soundName, false, 0.0F, 0.0F, 0.0F, 0, 0.0F);
				m_sndSystem->setPitch(var5, 1.0F);
				m_sndSystem->setVolume(var5, 0.25F);
				m_sndSystem->play(var5);
			}

		}
}
