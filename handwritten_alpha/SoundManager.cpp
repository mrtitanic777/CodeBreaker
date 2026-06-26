#include "stdafx.h"
// Hand-written PORT override for SoundManager.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// The converted SoundManager is built on Java's Paul Lamb SoundSystem library
// (SoundSystem / SoundPool / SoundPoolEntry / CodecJOrbis / LibraryLWJGLOpenAL)
// which does not exist on Xbox 360. The proven 360 audio backend is XACT3 (see
// XactAudio.cpp/.h, poured in from the reference port and shared with the indev
// build). This file keeps the converter-generated SoundManager.h interface — so
// every converted caller (World, RenderGlobal, PlayerController, GuiScreen,
// Item*, Block*) is unchanged — and bridges each obfuscated method onto the
// XACT3 free-function API. Sound names from the Java side ("step.grass",
// "random.click") are mapped to the shipped bank's short cue names.
//
// Adapted from the working indev override (handwritten/SoundManager.cpp). The
// Alpha interface differs: methods carry obfuscated names (func_340_a etc.),
// there is a third "streaming" pool, sndSystem/loaded are STATIC members, and
// GameSettings gates on FLOAT volumes (m_soundVolume/m_musicVolume) rather than
// indev's bool m_sound/m_music. We treat volume > 0 as "enabled" for the XACT
// on/off gates; XACT mixes at bank-authored levels (no per-cue volume API here).

#include "SoundManager.h"
#include "Common.h"
#include "XactAudio.h"
#include "GameSettings.h"
#include <string>
#include <cstdio>   // sprintf for step.* variant cue names
#include <cstdlib>  // rand

// ---------------------------------------------------------------------------
// Static members (Java: private static SoundSystem sndSystem / boolean loaded).
// On 360 the actual engine lives in g_audio (XactAudio), so s_sndSystem stays
// NULL — it exists only to satisfy the header/ABI. s_loaded mirrors the Java
// one-shot init guard.
// ---------------------------------------------------------------------------
SoundSystem* SoundManager::s_sndSystem = NULL;
bool         SoundManager::s_loaded    = false;

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

// Map a Java sound name ("liquid.water", "mob.pig", "random.fizz 2") to a cue in
// the shipped XACT sound bank. The bank uses SHORT cue names, NOT the dotted Java
// paths. Names with no bank cue return "" (caller skips). Bank cues: water, lava,
// lavapop, fire, ignite, hurt, pop, splash, pig1/2/3, pigdeath, sheep1/2/3,
// random_click/explode/fizz/bow/drr, fuse, glass1/2/3,
// step_{grass,gravel,stone,wood,sand,snow} (+ " 2".." 4" variants), and
// dig_/break_{grass,gravel,sand,snow,stone,wood}1..4.
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
	// so some callers pass the literal "random.m_fuse"; accept both spellings.
	if (name == "random.fuse"    || name == "random.m_fuse" || name == "fuse") return "fuse";

	// Block dig (in-progress mining tap) and break (block destroyed). The player
	// controller emits "dig.<mat>"; StepSound destroy emits "break.<mat>". Route both
	// to the dig_*/break_* bank cue families (glass break stays "random.glass", below).
	if (name.compare(0, 4, "dig.")   == 0) return digBreakCue("dig",   name.substr(4));
	if (name.compare(0, 6, "break.") == 0) return digBreakCue("break", name.substr(6));

	// step.* footsteps. The shipped bank contains step cues with " 2"/" 3"/" 4"
	// variant suffixes (step_grass, step_grass 2..4, step_gravel*, step_stone*,
	// step_wood*, step_sand*, step_snow*). Java passes "step." + StepSound.m_sound
	// where m_sound is one of: grass/gravel/stone/wood/cloth/sand. Pick a random
	// variant 1..4. The bank has NO "step_cloth", so wool/cloth falls back to
	// step_grass (closest soft footstep) rather than being silent.
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

	// glass break. StepSoundGlass returns "random.glass"; the break path emits it
	// when a glass block is destroyed. Bank has glass1/glass2/glass3.
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

	// --- ALPHA-NEW mobs/effects: cues that must be AUTHORED INTO sounds.xsb/xwb. ---
	// These map Java names to the short bank cue names the wave/sound bank should
	// contain. Until the cue exists in the bank GetCueIndex() fails and XaPlayCue
	// no-ops (silent, safe). See the "NEW SOUNDS TO BUILD" list handed to the user.
	if (name == "mob.chicken")      return "chicken";
	if (name == "mob.chickenhurt")  return "chickenhurt";
	if (name == "mob.chickenplop")  return "chickenplop";
	if (name == "mob.cow")          return "cow";
	if (name == "mob.cowhurt")      return "cowhurt";
	if (name == "mob.creeper")      return "creeper";
	if (name == "mob.creeperdeath") return "creeperdeath";
	if (name == "mob.skeleton")     return "skeleton";
	if (name == "mob.skeletonhurt") return "skeletonhurt";
	if (name == "mob.slime")        return "slime";
	if (name == "mob.slimeattack")  return "slimeattack";
	if (name == "mob.spider")       return "spider";
	if (name == "mob.spiderdeath")  return "spiderdeath";
	if (name == "mob.zombie")       return "zombie";
	if (name == "mob.zombiehurt")   return "zombiehurt";
	if (name == "mob.zombiedeath")  return "zombiedeath";
	if (name == "random.door_open"  || name == "door_open")  return "door_open";
	if (name == "random.door_close" || name == "door_close") return "door_close";
	if (name == "portal.portal")    return "portal";
	if (name == "portal.travel")    return "portal_travel";
	if (name == "portal.trigger")   return "portal_trigger";

	// calm (music handled separately) and anything else not in the bank -> skip
	return "";
}

// Volume floats gate the XACT on/off switches: > 0 means "enabled".
static bool soundOn(GameSettings* o) { return o != NULL && o->m_soundVolume > 0.0F; }
static bool musicOn(GameSettings* o) { return o != NULL && o->m_musicVolume > 0.0F; }

// ---------------------------------------------------------------------------

SoundManager::SoundManager()
	: m_soundPoolSounds(NULL)
	, m_soundPoolStreaming(NULL)
	, m_soundPoolMusic(NULL)
	, m_field_587_e(0)
	, m_options(NULL)
	, rand(NULL)
	, m_field_583_i(0)
{
}

SoundManager::~SoundManager() {
	m_soundPoolSounds = NULL;
	m_soundPoolStreaming = NULL;
	m_soundPoolMusic = NULL;
	m_options = NULL;
	rand = NULL;
}

// Java tryToSetLibraryAndCodecs(): one-shot engine + codec/library bring-up.
// On 360 that collapses to bringing up the XACT3 engine and loading the prebuilt
// wave/sound banks once.
void SoundManager::tryToSetLibraryAndCodecs() {
	if (!s_loaded) {
		InitAudio();   // XACT3 engine + sounds/music banks + audio thread
		s_loaded = true;
	}
}

// Java func_340_a(GameSettings): store options, lazily init when a volume is set.
void SoundManager::func_340_a(GameSettings* var1) {
	m_options = var1;
	if (var1 != NULL) {
		XaSetSoundEnabled(var1->m_soundVolume > 0.0F);
		XaSetMusicEnabled(var1->m_musicVolume > 0.0F);
	}
	// Java: init if (options == null || soundVolume != 0 || musicVolume != 0).
	if (!s_loaded && (var1 == NULL || soundOn(var1) || musicOn(var1))) {
		tryToSetLibraryAndCodecs();
	}
}

// Java onSoundOptionsChanged(): re-apply gates; lazy-init; stop music if muted.
void SoundManager::onSoundOptionsChanged() {
	bool sound = soundOn(m_options);
	bool music = musicOn(m_options);
	XaSetSoundEnabled(sound);
	XaSetMusicEnabled(music);
	if (!s_loaded && (sound || music)) {
		tryToSetLibraryAndCodecs();
	}
	if (!music) {
		StopMusic();
	}
}

void SoundManager::closeMinecraft() {
	if (s_loaded) {
		ShutdownAudio();
	}
}

// XACT uses prebuilt wave/sound banks, so runtime sound registration is a no-op.
// (Java added File handles to SoundPools; there is no equivalent on 360.)
void SoundManager::func_6372_a(const std::string& var1, std::string* var2) {
	(void)var1; (void)var2;
}

void SoundManager::addStreaming(const std::string& var1, std::string* var2) {
	(void)var1; (void)var2;
}

void SoundManager::addMusic(const std::string& var1, std::string* var2) {
	(void)var1; (void)var2;
}

// Java func_4033_c(): background-music scheduler (random track after a delay).
// XACT's music playlist handles selection/advance internally; just keep it
// pumped and started when music is enabled.
void SoundManager::func_4033_c() {
	if (s_loaded && musicOn(m_options)) {
		StartMusic();
	}
}

// Java func_338_a(EntityLiving, float): set 3D listener pos/orientation.
// XACT mixes in 2D here (fire-and-forget cues, no per-frame listener), matching
// the indev port. Pump the engine/music state machine each frame instead.
void SoundManager::func_338_a(EntityLiving* var1, float var2) {
	(void)var1; (void)var2;
	if (s_loaded) {
		UpdateAudio();
	}
}

// Java func_331_a(name, x,y,z, volume, pitch): streaming positional source.
// On 360 this is a fire-and-forget cue (no 3D attenuation), gated by volume>0.
void SoundManager::func_331_a(const std::string& var1, float var2, float var3, float var4, float var5, float var6) {
	(void)var2; (void)var3; (void)var4; (void)var6;
	if (s_loaded && soundOn(m_options) && var5 > 0.0F) {
		std::string cue = toCue(var1);
		if (!cue.empty()) XaPlayCue(cue.c_str());
	}
}

// Java func_336_b(name, x,y,z, volume, pitch): one-shot positional sound.
void SoundManager::func_336_b(const std::string& var1, float var2, float var3, float var4, float var5, float var6) {
	(void)var2; (void)var3; (void)var4; (void)var6;
	if (s_loaded && soundOn(m_options) && var5 > 0.0F) {
		std::string cue = toCue(var1);
		if (!cue.empty()) XaPlayCue(cue.c_str());
	}
}

// Java func_337_a(name, volume, pitch): non-positional UI/effect sound.
void SoundManager::func_337_a(const std::string& var1, float var2, float var3) {
	(void)var2; (void)var3;
	if (s_loaded && soundOn(m_options)) {
		std::string cue = toCue(var1);
		if (!cue.empty()) XaPlayCue(cue.c_str());
	}
}
