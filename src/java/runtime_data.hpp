// java/runtime_data.hpp -- the tables behind the Java runtime (runtime_data.cpp): constants of the stubs and the
// per-platform timer replacements. Private to runtime.cpp and its tables.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cb::java::runtime {

// The Java package of each external type the runtime stubs, so a stub's banner names the real library.
const std::map<std::string, std::string, std::less<>>& external_packages();

// java.awt.Color's constants: (name, ARGB).
const std::vector<std::pair<std::string_view, std::uint32_t>>& color_constants();

// LWJGL 2's Keyboard.getKeyName: key code -> the key's constant name without `KEY_`, sorted by code.
const std::vector<std::pair<int, std::string_view>>& lwjgl_key_names();

// What replaces the Windows timer calls in JavaRT.h and Random.h on a platform without them.
struct TimerPatch {
    std::string get_tick_count;         // for `(long long)GetTickCount()`
    std::string current_time_millis;    // the whole `inline long long currentTimeMillis() {...}`
    std::string nano_time;              // the whole `inline long long nanoTime() {...}`
};

// Keyed by platform key; "generic" is the platform-less conversion (the C library's clocks).
const std::map<std::string, TimerPatch, std::less<>>& timer_patches();

// The text of java.awt.Color: real HSB conversions and the constants of color_constants() (defined in runtime.cpp).
std::string color_header();

// The `getKeyName` and `getKeyIndex` functions of the Keyboard stub, written from lwjgl_key_names() (defined in runtime.cpp).
std::vector<std::string> keyboard_name_lines();

}  // namespace cb::java::runtime
