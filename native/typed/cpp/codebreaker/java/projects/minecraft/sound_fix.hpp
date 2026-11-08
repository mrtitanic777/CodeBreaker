// codebreaker/java/projects/minecraft/sound_fix.hpp -- C++ port of codebreaker.java.projects.minecraft.sound_fix.
//
// sound_fix.py -- make converted ports actually play SOUND EFFECTS on the console.
//
// Java SoundManager gates every SFX on the OGG sound pool:
//
//     SoundPoolEntry* var7 = this->soundPoolSounds->getRandomSoundFromSoundPool(name);
//     if (var7 != NULL && var5 > 0.0F) {
//         ... s_sndSystem->newSource(.., var7->soundUrl, var7->soundName, ..);   // soundName -> XACT cue
//         s_sndSystem->play(..);
//     }
//
// On the console there are NO OGG files -- sound comes from XACT wave/sound banks through the Audio.reference
// SoundSystem->XACT bridge -- so soundPoolSounds is EMPTY, getRandomSoundFromSoundPool() returns NULL, and the
// whole play block is skipped => ALL sound effects are silent. The bridge is ready; the generated game code
// just never reaches it.
//
// The proven hand-fix (MinecraftAlpha / MinecraftIndev SoundManager.cpp) defeats the guard and routes the
// sound NAME straight through as the XACT cue: drop the `var7 != NULL` conjunct, and pass `(void*)NULL, name`
// where the decompiled code passed `var7->soundUrl, var7->soundName`. newSource's 4th arg (`identifier`,
// std::string) IS the cue key and its 3rd arg (`url`) is a void* -- see Audio.reference.h. No XACT call is
// emitted into game code; the bridge does the routing.
//
// This pass migrates that hand-fix into the GENERATOR so EVERY conversion gets working SFX from ONE place --
// the fragmentation it replaces: Alpha/Indev hand-patched it in their trees, while Beta shipped a full-file
// SoundManager overlay that FORGOT it and stayed silent. It keys on the paulscode `soundPoolSounds->
// getRandomSoundFromSoundPool` guard STRUCTURE, not on method names/signatures, so a single rule covers
// Indev/Alpha/Beta (whose enclosing method signatures differ) and any future version keeping the paulscode
// SoundManager.
//
// SCOPE -- deliberately narrow, to match exactly what the proven hand-fix changes:
//   * SFX only: the `soundPoolSounds` pool (playSound / playSoundFX).
//   * soundPoolStreaming (ambient/records) is LEFT ALONE -- the hand-fix left it guarded; forcing it would
//     turn looping streams into non-looping one-shots.
//   * soundPoolMusic (getRandomSound) is LEFT ALONE -- the music path has a different, NULL-deref-prone shape
//     and different XACT routing; it needs its own targeted fix, not this SFX rule.
//
// convert applies it on the IR before the back end writes (patch(), by tree shape), then checks the output
// for a surviving trigger (stranded()). run()/process() are the same fix on C++ text, for `codebreaker java <fix> <dir>` on
// a tree converted elsewhere; that text form runs over the converted tree (xbox360), BEFORE overlays, alongside reflection_factory.
// Idempotent: after the rewrite the guard no longer contains `!= NULL`, so a re-run matches nothing. Also
// exposed as `codebreaker java sound-fix <dir>`.
//
// FIXED BUG sfx-bypass: the converted SoundManager gates every sound effect on the empty console OGG pool
// (getRandomSoundFromSoundPool != NULL) -> ALL sound effects silent; this pass drops the guard and routes the
// sound name to the XACT bridge. Migrated here from per-game SoundManager overlays so every convert gets it.
// FIXED BUG music-bypass: playRandomMusic's play("BgMusic") (which starts the XACT playlist) sits inside the
// same empty-pool guard -> background music never starts; this pass makes the play() path unconditional,
// keeping only the SoundPoolEntry deref (backgroundMusic) guarded.
//
// Typing notes:
//   * the Python's result dicts {'files': [{'file','sfx','music'}...], 'sites': N, 'stranded': [files]} are the
//     structs Touched and Result (run() and patch() return the same record);
//   * patch() takes the units (Unit nodes) of the lowered program.
#pragma once

#include <cstdint>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/translate/ir.hpp"

namespace codebreaker::java::projects::minecraft::sound_fix {

// The SFX guard site the generator emits:
//     SoundPoolEntry* <V> = <recv>->soundPoolSounds->getRandomSoundFromSoundPool(<NAME>);
//     if(<V> != NULL[ && <EXTRA>]) {
// <recv> is usually `this` but kept permissive ([^;]*?). <NAME> is the sound-name argument (a simple
// identifier). ANCHORED to `soundPoolSounds` so the streaming (soundPoolStreaming) and music
// (soundPoolMusic->getRandomSound) sites are NOT matched.
extern const py::re::Pattern _SFX_SITE;

extern const py::str _NOTE;

// fail-loud: a soundPoolSounds SFX guard still gated on `!= NULL` after the pass ran (a future decompile
// whose guard shape changed) -- signals a silent-SFX regression that this rule no longer catches.
extern const py::re::Pattern _SURVIVOR;

// ---- MUSIC (playRandomMusic) -----------------------------------------------------------------------------
// The music site the generator emits:
//     SoundPoolEntry* <V> = <recv>->soundPoolMusic->getRandomSound();
//     if(<V> != NULL) { <BODY: flat statements, no nested braces> }
// On console soundPoolMusic is EMPTY, so <V>==NULL and the whole block (incl. play("BgMusic")) is skipped ->
// the XACT music playlist never starts. UNLIKE SFX, the fix is NOT (void*)NULL routing: the proven Alpha
// playRandomMusic hand-fix hoists play/setVolume/delay OUT of the guard so play("BgMusic") (which starts the
// XACT playlist / StartMusic) fires UNCONDITIONALLY, while backgroundMusic -- the only call that needs a real
// SoundPoolEntry -- stays guarded and is simply skipped. We reproduce that behaviour more robustly by making
// the outer guard `if(true)` and inline-guarding ONLY the <V>-> deref line, no statement code-motion.
// Requires a flat (no nested-brace) block, which the paulscode playRandomMusic body is.
extern const py::re::Pattern _MUSIC_SITE;

extern const py::str _MUSIC_NOTE;

// fail-loud twin for music: a soundPoolMusic guard still gated on `!= NULL` immediately after the decl.
extern const py::re::Pattern _MUSIC_SURVIVOR;

// One file the pass changed: {'file': name, 'sfx': n, 'music': n}.
struct Touched {
    py::str file;
    int64_t sfx = 0;
    int64_t music = 0;
};

// {'files': [...], 'sites': N, 'stranded': [files]}
struct Result {
    py::list<Touched> files;
    int64_t sites = 0;
    py::list<py::str> stranded;
};

// Rewrite the SFX (soundPoolSounds) and music (soundPoolMusic) empty-pool guards in one .cpp.
// Returns (new_text, n_sfx, n_music).
std::tuple<py::str, int64_t, int64_t> process(const py::str& text);

// Apply the SFX + music bypass across the converted tree. Returns
// {'files': [{'file','sfx','music'}...], 'sites': N, 'stranded': [files]}. `stranded` = files where a
// soundPool*->getRandomSound* null-guard SURVIVED the rewrite (fail-loud).
Result run(const py::str& out_dir);

// ---- the same fixes on the IR, before the back end writes (what convert runs) ----------------------------
// The SFX and music bypass on the lowered program: matched by tree shape (the pool query's local and
// the null test right after it), so no printing of the C++ can hide the site. Returns
// {'files': [{'file','sfx','music'}], 'sites': N, 'stranded': []} like run().
Result patch(const py::list<codebreaker::translate::ir::NodeP>& units);

// The .cpp files where a soundPool null guard survived (run after the program is written).
py::list<py::str> stranded(const py::str& out_dir);

}  // namespace codebreaker::java::projects::minecraft::sound_fix
