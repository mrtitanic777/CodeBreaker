// Audio.h — paulscode SoundSystem -> XACT3 bridge for the Xbox 360 j2x port.  [Step 6, Route A]
//
// REFERENCE IMPLEMENTATION for the Xbox-project integration. It REPLACES the converter's no-op
// SoundSystem stub (j2x/runtime.py:1012-1015, _GENERATED['SoundSystem.h']): the new game code is
// FAITHFUL to the Java paulscode sound model -- the generated SoundManager.cpp calls
// s_sndSystem->newSource/newStreamingSource/play/setVolume/setPitch/setListener*/stop/playing/...
// So unlike the OLD port (which hand-overrode SoundManager to call XACT directly), here we leave
// SoundManager.cpp GENERATED and make the SoundSystem class it talks to REAL, routing each call
// onto the existing, shipped XACT3 backend (XactAudio.cpp/.h + sounds.xwb/music.xwb).
//
// Lives in the Xbox project against the XDK's <xact3.h> + the platform layer (XactAudio.h). It is
// NOT compiled by the j2x build (which only syntax-checks the no-op stub). Grounded in the old
// port's XactAudio.cpp + SoundManager.cpp (the proven 360 audio path) and
// docs/platform/03-input-and-audio.md (AUDIO section = the spec for this file).
//
// WHY THE BRIDGE MOVED (vs old port):
//   OLD: SoundManager.cpp was a hand override -> toCue(name) -> XaPlayCue().  SoundSystem unused.
//   NEW: SoundManager.cpp is generated and is the Java sound state machine. It funnels EVERY sound
//        through s_sndSystem (a SoundSystem*). The sound's bank-relevant identity is carried in
//        SoundPoolEntry::soundName (e.g. "step.grass3", "random.click1"), which SoundManager hands
//        to SoundSystem::newSource(.., name, soundUrl, soundName, ..). So the natural Route-A seam
//        is SoundSystem itself: implement newSource/play/... for real, map soundName -> XACT cue.
//
// Design (matches the old XACT path's limits exactly -- see HARDWARE-VERIFY / NOTES):
//   * Each newSource()/newStreamingSource() records a tiny per-"source" record keyed by the channel
//     name SoundManager invents ("sound_<n>", "streaming", "BgMusic"). It stores the SoundPoolEntry
//     soundName so play() can resolve it to a cue. No real mixer/voice is held -- XACT owns voices.
//   * play(name) resolves the record's soundName -> XACT cue via toCue() (ported from the old
//     SoundManager::toCue), then enqueues it through the existing audio thread (XaPlayCue), OR for
//     the "BgMusic" channel, starts the XACT music playlist (StartMusic).
//   * setVolume()/setPitch() are accepted but DROPPED to a binary gate -- the old XACT path has no
//     per-cue volume and pitch is disabled (XactAudio.cpp:556-561). 3D is 2D fire-and-forget:
//     setListenerPosition/Orientation and the per-source x/y/z are ignored (XactAudio.cpp:764-771).
//   * playing(name) reports music state for "BgMusic"/"streaming" (so SoundManager's playRandomMusic
//     scheduler advances correctly) and false otherwise (SFX are fire-and-forget, not tracked).
//   * CodecJOrbis stays a NO-OP (runtime.py:1007-1011) -- XACT plays prebuilt banks; no OGG decode.
//
// HARDWARE-VERIFY (can't be checked off-device):
//   * Cue coverage: soundName -> cue mapping silently no-ops when GetCueIndex fails (cue not authored
//     in sounds.xwb/.xsb). Confirm the bank actually contains every cue toCue() can emit; several
//     Alpha-new mob cues are MAPPED but may not be AUTHORED (see old SoundManager.cpp:138-162).
//   * Music start/advance: "BgMusic" playing() must go false when an XACT music cue ends so
//     playRandomMusic queues the next track; verify via isMusicCuePlaying() timing (60-frame guard).
//   * Init order: XACT engine + banks + audio thread must be up before the first play (boot seam).

#ifndef AUDIO_H
#define AUDIO_H

// ---------------------------------------------------------------------------------------------
// IMPORTANT: this header REPLACES the generated SoundSystem.h stub. The class name, the variadic
// method shapes, and the playing()->bool signature MUST stay call-compatible with what the
// generated SoundManager.cpp invokes (it passes paulscode's arg lists positionally). The generated
// stub used `(...)` varargs for every method; we keep typed overloads for the exact call shapes
// SoundManager uses and fall back to a `(...)` catch-all so any other paulscode call still links.
// ---------------------------------------------------------------------------------------------

#include <string>

class SoundSystem
{
public:
	SoundSystem();
	virtual ~SoundSystem();

	// --- lifecycle ---
	virtual void cleanup();                                   // SoundManager::closeMinecraft

	// --- source creation (SoundManager::playSound / playSoundFX / playStreaming) ---
	// Java: newSource(priority, sourcename, url, identifier, loop, x, y, z, attModel, distOrRoll)
	// We only need `sourcename` (the channel SoundManager invents) and `identifier` (== the
	// SoundPoolEntry.soundName, our cue key). The rest are accepted and dropped (2D, no priority).
	virtual void newSource(bool priority, const std::string& sourcename,
	                       void* url, const std::string& identifier,
	                       bool loop, float x, float y, float z, int attModel, float dist);
	virtual void newStreamingSource(bool priority, const std::string& sourcename,
	                                void* url, const std::string& identifier,
	                                bool loop, float x, float y, float z, int attModel, float dist);

	// --- playback control (keyed by the SoundManager channel name) ---
	virtual void play(const std::string& sourcename);
	virtual void stop(const std::string& sourcename);
	virtual void pause(const std::string& sourcename);
	virtual void removeSource(const std::string& sourcename);

	// --- per-source params: accepted, reduced to a binary gate (XACT has no per-cue vol/pitch) ---
	virtual void setVolume(const std::string& sourcename, float volume);
	virtual void setPitch(const std::string& sourcename, float pitch);

	// --- 3D listener: ignored (old XACT path mixes in 2D) ---
	virtual void setListenerPosition(float x, float y, float z);
	virtual void setListenerOrientation(float lx, float ly, float lz, float ux, float uy, float uz);

	// --- query (drives SoundManager::playRandomMusic / playStreaming scheduling) ---
	virtual bool playing(const std::string& sourcename);

	// Catch-all so any unmodeled paulscode SoundSystem call still links as a no-op.
	// (No cleanup(...) catch-all: it would be ambiguous with the 0-arg cleanup() the game calls.)
	void newSource(...) {}
	void newStreamingSource(...) {}
	void play(...) {}
	void stop(...) {}
	void pause(...) {}
	void removeSource(...) {}
	void setVolume(...) {}
	void setPitch(...) {}
	void setListenerPosition(...) {}
	void setListenerOrientation(...) {}
	bool playing(...) { return false; }
};

// ---------------------------------------------------------------------------------------------
// Ported name->cue map (from old SoundManager::toCue). Exposed so it can be unit-tested off-device
// and reused if a future override wants to call XACT directly. Returns "" if the name has no cue.
// ---------------------------------------------------------------------------------------------
namespace AudioBridge {
	std::string toCue(const std::string& soundName);   // dotted/variant Java name -> short XACT cue
}

#endif // AUDIO_H
