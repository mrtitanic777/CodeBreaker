// java/projects/minecraft/memory_text.hpp -- the debug memory line in gigabytes when there are that many.
//
// Minecraft prints its memory as megabytes (GuiMainMenu, GuiIngame):
//
//     "Free memory: " + free * 100L / max + "% of " + max / 1024L / 1024L + "MB"
//
// In the JVM `max` was the heap (-Xmx1024M: "of 1024MB"); converted, the runtime reports the computer's memory, and on a PC with
// 128 GB the line reads "of 128599MB". The pass turns every `<x> / 1024LL / 1024LL + "MB` of the emitted C++ into
// `j2xMemorySize(<x>) + "`, which writes megabytes below a gigabyte and gigabytes above ("512MB", "15.6GB", "125GB"), and defines
// that helper once at the top of a file that uses it. Idempotent; a file without the pattern is returned unchanged.
#pragma once

#include <string>
#include <string_view>

namespace cb::java::projects::minecraft::memory_text {

// The emitted C++ of one file with its megabyte texts rewritten; `text` itself when it has none.
std::string rewrite(std::string_view text);

// What j2xMemorySize writes for `bytes` (the same rule, here for the tests): "512MB", "15.6GB", "125GB".
std::string size_text(long long bytes);

}  // namespace cb::java::projects::minecraft::memory_text
