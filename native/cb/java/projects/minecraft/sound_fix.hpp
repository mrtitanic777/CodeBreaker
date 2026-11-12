// cb/java/projects/minecraft/sound_fix.hpp -- make converted ports actually play SOUND EFFECTS (and start the music) on
// the console.
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
// SoundSystem->XACT bridge -- so soundPoolSounds is EMPTY, getRandomSoundFromSoundPool() returns NULL, and the whole
// play block is skipped => ALL sound effects are silent. The bridge is ready; the generated game code just never
// reaches it.
//
// The proven hand-fix (MinecraftAlpha / MinecraftIndev SoundManager.cpp) defeats the guard and routes the sound NAME
// straight through as the XACT cue: drop the `var7 != NULL` conjunct, and pass `(void*)NULL, name` where the decompiled
// code passed `var7->soundUrl, var7->soundName`. newSource's 4th argument (`identifier`, std::string) IS the cue key
// and its 3rd argument (`url`) is a void* -- see Audio.reference.h. No XACT call is emitted into game code; the bridge
// does the routing.
//
// This pass migrates that hand-fix into the GENERATOR so EVERY conversion gets working SFX from ONE place -- the
// fragmentation it replaces: Alpha/Indev hand-patched it in their trees, while Beta shipped a full-file SoundManager
// overlay that FORGOT it and stayed silent. It keys on the paulscode `soundPoolSounds->getRandomSoundFromSoundPool`
// guard STRUCTURE, not on method names/signatures, so a single rule covers Indev/Alpha/Beta (whose enclosing method
// signatures differ) and any future version keeping the paulscode SoundManager.
//
// SCOPE -- deliberately narrow, to match exactly what the proven hand-fix changes:
//   * SFX: the `soundPoolSounds` pool (playSound / playSoundFX).
//   * soundPoolStreaming (ambient/records) is LEFT ALONE -- the hand-fix left it guarded; forcing it would turn
//     looping streams into non-looping one-shots.
//   * Music: playRandomMusic's play("BgMusic") (which starts the XACT playlist) sits inside the same empty-pool guard
//     -> background music never starts; the pass makes the play() path unconditional, keeping only the SoundPoolEntry
//     deref (backgroundMusic) guarded.
//
// The conversion applies it on the IR before the back end writes (patch(), by tree shape), then checks the output for a
// surviving trigger (stranded()). rewrite()/run() are the same fix on C++ text, for `codebreaker java sound-fix <dir>`
// on a tree converted elsewhere; that text form runs over the converted tree (xbox360), BEFORE overlays, alongside
// reflection_factory. Idempotent: after the rewrite the guard no longer contains `!= NULL`, so a re-run matches
// nothing.
#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/translate/ir.hpp"

namespace cb::java::projects::minecraft::sound_fix {

// How many SFX and music sites one file (or one class) had rewritten.
struct FileSites {
    std::string file;
    std::size_t sfx = 0;
    std::size_t music = 0;
};

struct Report {
    std::vector<FileSites> files;
    std::size_t sites = 0;
    // Files where a soundPool*->getRandomSound* null-guard SURVIVED the rewrite (fail loud).
    std::vector<std::string> stranded;
};

struct Rewritten {
    std::string text;
    std::size_t sfx = 0;
    std::size_t music = 0;
};

// Rewrite the SFX (soundPoolSounds) and music (soundPoolMusic) empty-pool guards in one .cpp's text.
Rewritten rewrite(std::string_view text);

// Apply the SFX and music bypass across the *.cpp files directly in a converted tree (rewritten in place).
Report run(const std::filesystem::path& out_dir);

// The same fixes on the lowered program (`units` are Unit nodes; new nodes are made in `arena`, the arena the tree
// lives in): matched by tree shape (the pool query's local and the null test right after it), so no printing of the
// C++ can hide the site. One report entry per class, named <class>.cpp; `stranded` is empty.
Report patch(Arena& arena, const std::vector<translate::ir::Node*>& units);

// The *.cpp files of a converted tree where a soundPool null guard survived (run after the program is written).
std::vector<std::string> stranded(const std::filesystem::path& out_dir);

}  // namespace cb::java::projects::minecraft::sound_fix
