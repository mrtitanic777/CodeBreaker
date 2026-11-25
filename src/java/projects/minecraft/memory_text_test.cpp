// Tests of java/projects/minecraft/memory_text.hpp and music_guard.hpp: the two text passes every platform's Minecraft gets.
#include "java/projects/minecraft/memory_text.hpp"

#include "core/testing.hpp"
#include "java/projects/minecraft/music_guard.hpp"

namespace cb::java::projects::minecraft {
namespace {

constexpr long long kMb = 1024LL * 1024LL;

CB_TEST("java.minecraft.memory_text.megabytes_below_a_gigabyte_gigabytes_above", [] {
    CB_CHECK_EQ(memory_text::size_text(512 * kMb), "512MB");
    CB_CHECK_EQ(memory_text::size_text(1023 * kMb), "1023MB");
    CB_CHECK_EQ(memory_text::size_text(1024 * kMb), "1.0GB");
    CB_CHECK_EQ(memory_text::size_text(16000 * kMb), "15.6GB");
    CB_CHECK_EQ(memory_text::size_text(128599 * kMb), "125GB");   // (the 128 GB PC the line read "of 128599MB" on)
});

CB_TEST("java.minecraft.memory_text.the_megabyte_texts_call_the_helper_defined_once", [] {
    const std::string in = "#include \"stdafx.h\"\n#include \"GuiMainMenu.h\"\n\nvoid f() {\n"
                           "\tvar16 = (JCat() + \"Free memory: \" + var13 * 100LL / var7 + \"% of \" + var7 / 1024LL / 1024LL + \"MB\");\n"
                           "\tvar16 = (JCat() + \"Allocated memory: \" + var9 * 100LL / var7 + \"% (\" + var9 / 1024LL / 1024LL + \"MB)\");\n}\n";
    const std::string out = memory_text::rewrite(in);
    CB_CHECK(out.find("+ j2xMemorySize(var7) + \"\");") != std::string::npos);
    CB_CHECK(out.find("+ j2xMemorySize(var9) + \")\");") != std::string::npos);
    CB_CHECK(out.find("/ 1024LL / 1024LL + \"MB") == std::string::npos);
    const std::size_t helper = out.find("static JavaString j2xMemorySize(long long bytes)");
    CB_CHECK(helper != std::string::npos && helper > out.find("#include \"GuiMainMenu.h\""));   // after the includes
    CB_CHECK_EQ(memory_text::rewrite(out), out);                                                  // idempotent
    CB_CHECK_EQ(memory_text::rewrite("int x = y / 1024LL;\n"), "int x = y / 1024LL;\n");        // nothing to change
});

CB_TEST("java.minecraft.music_guard.no_music_is_silence_not_a_crash", [] {
    const std::string in = "\t\tSoundPoolEntry* var4 = this->soundPoolMusic->getRandomSoundFromSoundPool(\"calm\");\n"
                           "\t\tthis->sndSystem->newStreamingSource(true, \"BgMusic\", var4->soundUrl);\n";
    const std::string out = music_guard::rewrite(in);
    CB_CHECK(out.find("getRandomSoundFromSoundPool(\"calm\"); if (var4 == NULL) return;") != std::string::npos);
    CB_CHECK_EQ(music_guard::rewrite(out), out);   // idempotent
    const std::string sounds = "SoundPoolEntry* e = this->soundPoolSounds->getRandomSoundFromSoundPool(name);\n";
    CB_CHECK_EQ(music_guard::rewrite(sounds), sounds);   // (the sound effects' pool is checked by the game itself)
});

}  // namespace
}  // namespace cb::java::projects::minecraft
