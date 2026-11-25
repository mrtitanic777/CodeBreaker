// java/projects/minecraft/music_guard.hpp -- no music to play is silence, not a crash.
//
// Indev's SoundManager.playRandomMusicIfReady takes a song from the music pool and plays it without asking whether there was one:
//
//     SoundPoolEntry var4 = this.soundPoolMusic.getRandomSoundFromSoundPool("calm");
//     this.sndSystem.newStreamingSource(true, "BgMusic", var4.soundUrl, ...);
//
// The pool is filled by ThreadDownloadResources from minecraft.net (RetroMCP's launcher serves it); with no music there the JVM throws a
// NullPointerException that ends the game the moment a world starts, and the converted game faults. The pass puts
// `if (var4 == NULL) return;` after the pick in the emitted C++ (the method returns nothing), so a game without its music plays without it.
// Idempotent; a file without the pick is returned unchanged.
#pragma once

#include <string>
#include <string_view>

namespace cb::java::projects::minecraft::music_guard {

std::string rewrite(std::string_view text);

}  // namespace cb::java::projects::minecraft::music_guard
