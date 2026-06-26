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
#include <cstdio>   // sprintf for step.* variant cue names
#include <cstdlib>  // rand

// Map a Java sound name ("liquid.water", "mob.pig", "random.fizz 2") to a cue in
// the shipped XACT sound bank. The bank uses SHORT cue names, NOT the dotted Java
// paths -- the old dot->underscore code emitted "liquid_water"/"mob_pig", which
// matched NO cue, so absolutely no in-world sounds played. Names with no bank cue
// return "" (caller skips). Bank cues: water, lava, lavapop, fire, ignite, hurt,
// pop, splash, pig1/2/3, pigdeath, sheep1/2/3, random_click/explode/fizz/bow/drr,
// fuse, glass1/2/3, step_{grass,gravel,stone,wood,sand,snow} (+ " 2".." 4"
// variants), and dig_/break_{grass,gravel,sand,snow,stone,wood}1..4.
// DIG/BREAK: Indev's StepSound returned "step.<mat>" for BOTH the footstep method
// (stepSoundDir2) and the block-destroy method (stepSoundDir), so digging and
// breaking collapsed onto the step_* cues and the shipped dig_/break_ cues were
// silent. Fixed at the source: PlayerControllerSP now emits "dig.<mat>" for the
// mining tap and StepSound::stepSoundDir() now emits "break.<mat>" for destroy;
// both are routed here. See PlayerControllerSP.cpp, StepSound.cpp, StepSoundGravel.cpp.

// Map a dig/break event ("dig.<mat>" / "break.<mat>") to a bank cue. The bank ships
// dig_/break_ families for grass,gravel,sand,snow,stone,wood with a 1..4 NUMERIC
// suffix and NO space (e.g. "dig_grass1", "break_stone4") -- unlike the step_ cues
// which use a SPACE (" 2".." 4"). There is no dig_cloth/break_cloth, so cloth (wool)
// falls back to the grass family. Unknown material -> "" (caller skips).
static std::string digBreakCue(const char* family, const std::string& matIn) {
	std::string m = matIn;
	if (m == "cloth") m = "grass";   // no dig_cloth/break_cloth in bank
	if (m == "grass" || m == "gravel" || m == "sand" ||
	    m == "snow"  || m == "stone"  || m == "wood") {
		char buf[32];
		sprintf(buf, "%s_%s%d", family, m.c_str(), ::rand() % 4 + 1);
		return std::string(buf);
	}
	return "";
}

static std::string toCue(const std::string& nameIn) {
	// strip trailing " 2"/" 3" variant index, then lowercase
	std::string name = nameIn;
	size_t sp = name.find(' ');
	if (sp != std::string::npos) name = name.substr(0, sp);
	for (size_t i = 0; i < name.size(); ++i) {
		char ch = name[i];
		if (ch >= 'A' && ch <= 'Z') name[i] = (char)(ch - 'A' + 'a');
	}

	if (name == "liquid.water"   || name == "water")    return "water";
	if (name == "liquid.lava"    || name == "lava")     return "lava";
	if (name == "liquid.lavapop" || name == "lavapop")  return "lavapop";
	if (name == "fire.fire"      || name == "fire")     return "fire";
	if (name == "fire.ignite"    || name == "ignite")   return "ignite";
	if (name == "random.fizz"    || name == "fizz")     return "random_fizz";
	if (name == "random.explode" || name == "explode")  return "random_explode";
	if (name == "random.click"   || name == "click")    return "random_click";
	if (name == "random.pop"     || name == "pop")      return "pop";
	if (name == "random.splash"  || name == "splash")   return "splash";
	if (name == "random.hurt"    || name == "hurt")     return "hurt";
	if (name == "random.bow"     || name == "bow")      return "random_bow";  // skeleton/bow shoot
	if (name == "random.drr"     || name == "drr")      return "random_drr";  // arrow hit
	// TNT/creeper fuse. The converter renamed the Java field "fuse" to "m_fuse",
	// so callers (BlockTNT.cpp:43, EntityCreeper.cpp:67) pass the literal
	// "random.m_fuse"; accept both spellings. Bank cue is just "fuse".
	if (name == "random.fuse"    || name == "random.m_fuse" || name == "fuse") return "fuse";

	// Block dig (in-progress mining tap) and break (block destroyed). PlayerControllerSP
	// emits "dig.<mat>"; StepSound::stepSoundDir() emits "break.<mat>". Route both to the
	// dig_*/break_* bank cue families (glass break stays "random.glass", handled below).
	if (name.compare(0, 4, "dig.")   == 0) return digBreakCue("dig",   name.substr(4));
	if (name.compare(0, 6, "break.") == 0) return digBreakCue("break", name.substr(6));

	// step.* footsteps. The shipped bank DOES contain step cues with " 2"/" 3"/" 4"
	// variant suffixes (step_grass, step_grass 2..4, step_gravel*, step_stone*,
	// step_wood*, step_sand*, step_snow*). Java passes "step." + StepSound.m_sound
	// where m_sound is one of: grass/gravel/stone/wood/cloth/sand (see Block.cpp
	// s_sound*Footstep + StepSound::stepSoundDir2). Pick a random variant 1..4.
	// NOTE: the bank has NO "step_cloth" cue, so wool/cloth falls back to step_grass
	// (closest soft footstep) rather than being silent.
	{
		std::string base;
		if      (name == "step.grass")  base = "step_grass";
		else if (name == "step.gravel") base = "step_gravel";
		else if (name == "step.stone")  base = "step_stone";
		else if (name == "step.wood")   base = "step_wood";
		else if (name == "step.sand")   base = "step_sand";
		else if (name == "step.snow")   base = "step_snow";
		else if (name == "step.cloth")  base = "step_grass"; // no step_cloth in bank
		if (!base.empty()) {
			int variant = ::rand() % 4;        // 0 -> base, 1..3 -> "base N+1"
			if (variant == 0) return base;
			char buf[32];
			sprintf(buf, "%s %d", base.c_str(), variant + 1);
			return std::string(buf);
		}
	}

	// glass break. StepSoundGlass::stepSoundDir() returns "random.glass" (see
	// Block::s_soundGlassFootstep); the player-break path PlayerController.cpp:49
	// emits it when a glass block is destroyed. Bank has glass1/glass2/glass3.
	if (name == "random.glass" || name == "glass") {
		const char* v[3] = { "glass1", "glass2", "glass3" };
		return v[::rand() % 3];
	}

	// multi-variant mobs: rotate through pig1/2/3, sheep1/2/3
	static int s_variant = 0;
	if (name.find("pig") != std::string::npos) {
		if (name.find("death") != std::string::npos) return "pigdeath";
		const char* v[3] = { "pig1", "pig2", "pig3" };
		return v[(s_variant++) % 3];
	}
	if (name.find("sheep") != std::string::npos) {
		const char* v[3] = { "sheep1", "sheep2", "sheep3" };
		return v[(s_variant++) % 3];
	}

	// calm (music handled separately) and anything else not in the bank -> skip
	return "";
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
		std::string cue = toCue(var1);
		if (!cue.empty()) XaPlayCue(cue.c_str());
	}
}

void SoundManager::playSoundFX(const std::string& var1, float var2, float var3) {
	(void)var2; (void)var3;
	if (m_loaded && m_options != NULL && m_options->m_sound) {
		std::string cue = toCue(var1);
		if (!cue.empty()) XaPlayCue(cue.c_str());
	}
}
