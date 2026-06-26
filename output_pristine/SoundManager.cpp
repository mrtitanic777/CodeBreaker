#include "stdafx.h"
// Hand-written PORT override for SoundManager.java
// ------------------------------------------------
// The converted SoundManager is built on Java's Paul Lamb SoundSystem library
// (SoundSystem / SoundPool / SoundPoolEntry) which does not exist on Xbox 360.
// The proven 360 audio backend is XACT3 (see XactAudio.cpp, poured in from the
// reference port). This file keeps the converter-generated SoundManager.h
// interface — so every converted caller (World, RenderGlobal, PlayerController,
// GuiScreen, Item*, Block*) is unchanged — and bridges each method onto the
// XACT3 free-function API. Sound names from the Java side ("step.grass",
// "random.click") are mapped to the bank cue convention (underscores).

#include "SoundManager.h"
#include "Common.h"
#include "GameSettings.h"
#include "XactAudio.h"
#include <string>

// "step.grass" / "random.click" / "step.grass 2" -> "step_grass" / "random_click"
static std::string toCue(const std::string& name) {
	std::string c = name;
	for (size_t i = 0; i < c.size(); ++i) {
		if (c[i] == '.' || c[i] == ' ') c[i] = '_';
	}
	return c;
}

SoundManager::SoundManager()
	: m_sndSystem(NULL)
	, m_soundPoolSounds(NULL)
	, m_soundPoolMusic(NULL)
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

void SoundManager::tryToSetLibraryAndCodecs() {
	if (!m_loaded) {
		InitAudio();   // XACT3 engine + sounds/music banks + audio thread
		m_loaded = true;
	}
}

void SoundManager::loadSoundSettings(GameSettings* var1) {
	m_options = var1;
	if (var1 != NULL) {
		XaSetSoundEnabled(var1->m_sound);
		XaSetMusicEnabled(var1->m_music);
	}
	if (!m_loaded && var1 != NULL && (var1->m_sound || var1->m_music)) {
		tryToSetLibraryAndCodecs();
	}
}

void SoundManager::onSoundOptionsChanged() {
	bool sound = (m_options != NULL && m_options->m_sound);
	bool music = (m_options != NULL && m_options->m_music);
	XaSetSoundEnabled(sound);
	XaSetMusicEnabled(music);
	if (!m_loaded && (sound || music)) {
		tryToSetLibraryAndCodecs();
	}
	if (!music) {
		StopMusic();
	}
}

void SoundManager::closeMinecraft() {
	if (m_loaded) {
		ShutdownAudio();
	}
}

// XACT uses prebuilt wave/sound banks, so runtime sound registration is a no-op.
void SoundManager::addSound(const std::string& var1, std::string* var2) {
	(void)var1; (void)var2;
}

void SoundManager::addMusic(const std::string& var1, std::string* var2) {
	(void)var1; (void)var2;
}

void SoundManager::playRandomMusicIfReady(float var1, float var2, float var3) {
	(void)var1; (void)var2; (void)var3;
	if (m_loaded && m_options != NULL && m_options->m_music) {
		StartMusic();
	}
}

void SoundManager::setListener(EntityLiving* var1, float var2) {
	// XACT mixes in 2D here; there is no per-frame listener orientation to set.
	// Pump the music playlist state machine each frame (auto-advance tracks).
	(void)var1; (void)var2;
	if (m_loaded) {
		UpdateAudio();
	}
}

void SoundManager::playSound(const std::string& var1, float var2, float var3, float var4, float var5, float var6) {
	(void)var2; (void)var3; (void)var4; (void)var6;
	if (m_loaded && m_options != NULL && m_options->m_sound && var5 > 0.0F) {
		XaPlayCue(toCue(var1).c_str());
	}
}

void SoundManager::playSoundFX(const std::string& var1, float var2, float var3) {
	(void)var2; (void)var3;
	if (m_loaded && m_options != NULL && m_options->m_sound) {
		XaPlayCue(toCue(var1).c_str());
	}
}
