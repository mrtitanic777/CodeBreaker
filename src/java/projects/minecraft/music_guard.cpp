// java/projects/minecraft/music_guard.cpp -- see music_guard.hpp.
#include "java/projects/minecraft/music_guard.hpp"

#include <regex>

namespace cb::java::projects::minecraft::music_guard {

std::string rewrite(std::string_view text_in) {
    // `SoundPoolEntry* var4 = this->soundPoolMusic->getRandomSoundFromSoundPool("calm");` not already followed by a check of var4
    static const std::regex kPick(R"((SoundPoolEntry\* (\w+) = this->soundPoolMusic->getRandomSoundFromSoundPool\([^;]*\);)(?![ \t]*if \(\2 == NULL\)))");
    std::string text(text_in);
    if (text.find("soundPoolMusic->getRandomSoundFromSoundPool") == std::string::npos) return text;
    return std::regex_replace(text, kPick, "$1 if ($2 == NULL) return;   // (no music in the pool: silence, not a crash)");
}

}  // namespace cb::java::projects::minecraft::music_guard
