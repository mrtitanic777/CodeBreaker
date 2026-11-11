// cb/java/debug.hpp -- emit the ONE debug file for the converted game.
//
// The converter routes every piece of debug instrumentation through a single header (`Debug.h`) with a single
// master switch. When `DEBUG_ENABLED` is 0 (retail), every debug macro compiles to nothing, so ALL debug code
// anywhere in the codebase -- as long as it goes through these macros -- is disabled by that one toggle. No
// scattered `#ifdef`s, no debug path left hot in a release build.
//
// Design constraints:
//   * concise, not spam -- thousands of log lines is the wrong answer. Every channel is rate-limited: DBG_ONCE (once
//     per call site), DBG_EVERY(n) (every Nth hit), DBG_THROTTLE(ms) (at most once per interval). Leaks and packet
//     loss are ACCUMULATED and reported as a periodic/at-exit SUMMARY, not a line per event.
//   * categorised so you can find one class of problem fast: BUG (invariants/asserts), LEAK (alloc tracking), CRASH
//     soft (recovered) + hard (fatal, with context), NET (connect/latency/packet loss).
//   * one file, toggleable at all times; toggling it off disables everything downstream.
//
// Output sink is OutputDebugStringA on the 360 (visible in the XDK / PIX / a debugger), plain stderr on PC builds --
// selected by the platform macro so the same code runs in both.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::debug {

// Debug.h with DEBUG_ENABLED defaulting to 1 (a debug build) or 0 (retail). Still overridable per build config.
std::string debug_header(bool enabled_default = true);
std::string_view debug_cpp();      // Debug.cpp, the backend
std::string_view debug_readme();   // DEBUG.md

// Emit Debug.h + Debug.cpp + DEBUG.md into out_dir (created if missing); returns the three paths in that order.
std::vector<std::filesystem::path> write_debug(const std::filesystem::path& out_dir, bool enabled_default = true);

struct MarkerSite {
    std::string file;   // the file's name inside the scanned folder
    std::uint32_t line = 0;   // 1-based
};

// Every `J2X-DEBUG[tag]` marker in the .cpp and .h files directly inside out_dir (files in name order, the first
// marker on each line): the full, greppable catalogue of debug facilities and (once instrumented) call sites.
std::map<std::string, std::vector<MarkerSite>> scan_debug_markers(const std::filesystem::path& out_dir);

}  // namespace cb::java::debug
