#include "stdafx.h"
// Audio.cpp — paulscode SoundSystem -> XACT3 bridge for the Xbox 360 j2x port.  [Step 6, Route A]
// REFERENCE IMPLEMENTATION (Xbox-side; not built by j2x). See Audio.reference.h for the design.
//
// Grounded in the OLD port's proven 360 audio path:
//   * XactAudio.cpp/.h  — the XACT3 engine, sounds.xwb/music.xwb wave banks, the audio thread on
//                         HW thread 5, the music playlist, and the free-function API:
//                         InitAudio / UpdateAudio / ShutdownAudio / StartMusic / StopMusic /
//                         XaPlayCue / XaSetSoundEnabled / XaSetMusicEnabled.
//   * SoundManager.cpp  — the OLD hand override's toCue() name map (ported verbatim below).
//
// In the NEW build SoundManager.cpp stays GENERATED (it is the faithful Java sound state machine);
// THIS file makes the SoundSystem it calls real. The new SoundManager funnels everything through
// SoundSystem, so every dotted/variant sound name arrives here as a SoundPoolEntry::soundName via
// newSource(.., identifier=soundName, ..) and is then triggered by play(channelName).

#include "Audio.h"
#include "XactAudio.h"     // InitAudio/UpdateAudio/XaPlayCue/StartMusic/StopMusic/XaSet*Enabled, g_audio
#include "Common.h"        // DbgPrintF (optional logging)
#include <cstdio>          // sprintf for variant cue names
#include <cstdlib>         // rand
#include <map>
#include <string>

// =============================================================================================
// name -> XACT cue map.  PORTED FROM the old SoundManager::toCue (MinecraftAlpha/SoundManager.cpp
// :62-166). The shipped sounds.xwb/.xsb uses SHORT cue names, NOT the dotted Java paths. Names with
// no bank cue return "" (caller skips). This is the single source of truth for Route A; if a sound
// is silent on device, the gap is almost always HERE or a cue missing from the bank.
//
// Bank cues (per old port): water, lava, lavapop, fire, ignite, hurt, pop, splash, pig1/2/3,
// pigdeath, sheep1/2/3, random_click/explode/fizz/bow/drr, fuse, glass1/2/3,
// step_{grass,gravel,stone,wood,sand,snow} (+ " 2".." 4" variants), and
// dig_/break_{grass,gravel,sand,snow,stone,wood}1..4.  ALPHA-NEW mob/portal/door cues are MAPPED
// but must be AUTHORED into the bank or GetCueIndex fails and the play silently no-ops.
// =============================================================================================
namespace {

// dig/break families: numeric 1..4 suffix, NO space (e.g. "dig_grass1","break_stone4"); cloth->grass.
std::string digBreakCue(const char* family, const std::string& matIn) {
	std::string m = matIn;
	if (m == "cloth") m = "grass";          // no dig_cloth/break_cloth in bank
	if (m == "grass" || m == "gravel" || m == "sand" ||
	    m == "snow"  || m == "stone"  || m == "wood") {
		char buf[32];
		sprintf(buf, "%s_%s%d", family, m.c_str(), ::rand() % 4 + 1);
		return std::string(buf);
	}
	return "";
}

} // namespace

namespace AudioBridge {

std::string toCue(const std::string& nameIn) {
	// FIXED BUG audio-tocue-bank-name-mismatch: the rebuilt Beta bank (j2x xact build) names EVERY cue as
	// the Java sound EVENT with '.'/'/'/' ' -> '_' (see j2x/xact.py _cue_name): liquid_water, fire_fire,
	// random_pop, random_hurt, mob_pig, mob_cow, step_grass, random_glass, ... The legacy Alpha-bank map
	// below returned SHORT names (water, lava, pop, hurt, pig1/2/3, cow) that DON'T EXIST in this bank, so
	// GetCueIndex failed and most SFX were silent ("only a couple sounds"). Each bank cue already carries
	// its wave variants internally, so no numbered names are needed -- just match the build convention.
	{
		std::string n = nameIn;
		std::string::size_type sp2 = n.find(' ');
		if (sp2 != std::string::npos) n = n.substr(0, sp2);   // drop the paulscode " 2"/" 3" variant suffix
		std::string cue;
		for (std::string::size_type i = 0; i < n.size(); ++i) {
			char ch = n[i];
			if (ch >= 'A' && ch <= 'Z') ch = (char)(ch - 'A' + 'a');
			if (ch == '.' || ch == '/' || ch == ' ') ch = '_';
			cue += ch;
		}
		return cue;
	}
	// ---- legacy Alpha-bank map (now UNREACHABLE; retained for reference only) ----
	// strip a trailing " 2"/" 3" variant token (paulscode keeps it on soundName), then lowercase
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
	if (name == "random.bow"     || name == "bow")      return "random_bow";   // skeleton/bow shoot
	if (name == "random.drr"     || name == "drr")      return "random_drr";   // arrow hit
	// TNT/creeper fuse: converter may rename Java "fuse" field to "m_fuse"; accept both spellings.
	if (name == "random.fuse"    || name == "random.m_fuse" || name == "fuse") return "fuse";

	// block dig (in-progress) and break (destroyed) -> dig_*/break_* families. glass handled below.
	if (name.compare(0, 4, "dig.")   == 0) return digBreakCue("dig",   name.substr(4));
	if (name.compare(0, 6, "break.") == 0) return digBreakCue("break", name.substr(6));

	// step.* footsteps. Java passes "step." + StepSound.stepSoundName (grass/gravel/stone/wood/
	// cloth/sand/snow). Bank has step_<mat> + " 2".." 4". No step_cloth -> fall back to step_grass.
	{
		std::string base;
		if      (name == "step.grass")  base = "step_grass";
		else if (name == "step.gravel") base = "step_gravel";
		else if (name == "step.stone")  base = "step_stone";
		else if (name == "step.wood")   base = "step_wood";
		else if (name == "step.sand")   base = "step_sand";
		else if (name == "step.snow")   base = "step_snow";
		else if (name == "step.cloth")  base = "step_grass";   // no step_cloth in bank
		if (!base.empty()) {
			int variant = ::rand() % 4;        // 0 -> base, 1..3 -> "base N+1"
			if (variant == 0) return base;
			char buf[32];
			sprintf(buf, "%s %d", base.c_str(), variant + 1);
			return std::string(buf);
		}
	}

	// glass break (StepSoundGlass -> "random.glass"); bank has glass1/glass2/glass3.
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

	// --- ALPHA-NEW cues: must be AUTHORED INTO sounds.xsb/.xwb or they silently no-op. ---
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

	// calm* music is handled by the XACT playlist (not via toCue); anything else -> skip.
	return "";
}

} // namespace AudioBridge

// =============================================================================================
// Per-"source" bookkeeping.
//
// SoundManager invents a channel name per call and immediately newSource()s then play()s it:
//   * SFX / 3D one-shots:  "sound_<0..255>"  (playSound / playSoundFX)
//   * looping ambient:     "streaming"       (playStreaming -- water/lava/fire ambience, records)
//   * background music:    "BgMusic"         (playRandomMusic)
// We don't hold real voices (XACT owns them). We only remember, per channel, the cue identity
// (the SoundPoolEntry.soundName passed as `identifier`) so play() can resolve it. We also remember
// whether a channel is the music/streaming channel so playing() can answer the scheduler honestly.
// =============================================================================================
namespace {

struct SourceRec {
	std::string cueName;     // soundName/identifier handed in by SoundManager (our toCue input)
	bool        isMusic;     // channel == "BgMusic"
	bool        isStreaming; // channel == "streaming"
};

static std::map<std::string, SourceRec> s_sources;

static bool isMusicChannel(const std::string& n)     { return n == "BgMusic"; }
static bool isStreamingChannel(const std::string& n) { return n == "streaming"; }

} // namespace

// =============================================================================================
// SoundSystem implementation (the bridge).
// =============================================================================================
SoundSystem::SoundSystem() {
	// SoundManager constructs `new SoundSystem()` inside tryToSetLibraryAndCodecs() AFTER it has
	// decided audio is wanted. That is our deterministic "bring up the engine" hook -- it mirrors
	// the old override's tryToSetLibraryAndCodecs()->InitAudio(). Idempotent: InitAudio/XactAudio
	// guards against double-start.
	InitAudio();   // XACT3 engine + sounds.xwb/music.xwb + audio thread on HW thread 5
	DbgPrintF("Audio: SoundSystem bridge up (Route A: XACT3)\n");
}

SoundSystem::~SoundSystem() {
	// Generated SoundManager calls cleanup() explicitly on shutdown; keep dtor harmless.
}

void SoundSystem::cleanup() {
	StopMusic();
	ShutdownAudio();
	s_sources.clear();
}

// ----- source creation: just record the cue identity for this channel -----
void SoundSystem::newSource(bool, const std::string& sourcename, void*,
                            const std::string& identifier, bool, float, float, float, int, float) {
	SourceRec r;
	r.cueName     = identifier;                     // SoundPoolEntry.soundName == our cue key
	r.isMusic     = isMusicChannel(sourcename);
	r.isStreaming = isStreamingChannel(sourcename);
	s_sources[sourcename] = r;
}

void SoundSystem::newStreamingSource(bool, const std::string& sourcename, void*,
                                     const std::string& identifier, bool, float, float, float, int, float) {
	SourceRec r;
	r.cueName     = identifier;
	r.isMusic     = isMusicChannel(sourcename);
	r.isStreaming = isStreamingChannel(sourcename);
	s_sources[sourcename] = r;
}

// ----- play: resolve the channel's cue and fire it on XACT -----
void SoundSystem::play(const std::string& sourcename) {
	std::map<std::string, SourceRec>::iterator it = s_sources.find(sourcename);

	// "BgMusic": defer to the XACT music playlist (shuffles calm1/calm2/calm3, auto-advances).
	// We ignore the specific track SoundManager picked -- the XACT playlist owns selection. This is
	// the same simplification the old override used (StartMusic in func_4033_c).
	if (isMusicChannel(sourcename)) {
		StartMusic();
		return;
	}

	if (it == s_sources.end()) return;             // play() without a prior newSource -> nothing

	std::string cue = AudioBridge::toCue(it->second.cueName);
	if (!cue.empty())
		XaPlayCue(cue.c_str());                    // -> audio thread ring -> SoundBank->Play (2D)
	// NOTE: "streaming" ambience (water/lava/fire) is currently fire-and-forget like any SFX -- it
	// will NOT loop. XACT cues loop only if authored to. HARDWARE-VERIFY: if ambient loops are
	// required, author looping cues in the bank (or move ambience to Route B). See doc 06b.
}

void SoundSystem::stop(const std::string& sourcename) {
	if (isMusicChannel(sourcename)) {
		StopMusic();
	}
	// SFX/streaming are fire-and-forget; there is no per-channel XACT handle to stop. (To stop ALL
	// audio on level change, the platform layer calls XactAudio StopAllAudio elsewhere.)
}

void SoundSystem::pause(const std::string& sourcename)      { (void)sourcename; /* no XACT pause path */ }

void SoundSystem::removeSource(const std::string& sourcename) {
	s_sources.erase(sourcename);
}

// ----- per-source params: reduced to a binary gate (no per-cue volume/pitch on the XACT path) -----
void SoundSystem::setVolume(const std::string& sourcename, float volume) {
	// XACT mixes at bank-authored levels; there is no per-cue volume API in the old path. We honor
	// only the on/off intent. Music mute is meaningful (SoundManager sets BgMusic volume to 0 to
	// mute) so route that to the XACT music gate; SFX volume is left to the bank.
	if (isMusicChannel(sourcename))
		XaSetMusicEnabled(volume > 0.0F);
}

void SoundSystem::setPitch(const std::string& sourcename, float pitch) {
	// Pitch is DISABLED on the old XACT path (playSFXWithPitch short-circuits, XactAudio.cpp:556).
	// Accept and ignore. If per-cue pitch is ever required, that's Route B (real SoundSystem+OGG).
	(void)sourcename; (void)pitch;
}

// ----- 3D listener: ignored (old XACT path mixes in 2D, fire-and-forget) -----
void SoundSystem::setListenerPosition(float, float, float) {}
void SoundSystem::setListenerOrientation(float, float, float, float, float, float) {}

// ----- query: drive the music/streaming scheduler honestly, everything else false -----
bool SoundSystem::playing(const std::string& sourcename) {
	// SoundManager::playRandomMusic only queues the next track when BOTH "BgMusic" and "streaming"
	// report not playing. Report XACT music state for "BgMusic"; "streaming" reports false (we don't
	// hold streaming voices) so it never blocks the scheduler.
	if (isMusicChannel(sourcename))
		return g_audio.isMusicCuePlaying();
	return false;     // SFX/streaming are fire-and-forget; not tracked
}
