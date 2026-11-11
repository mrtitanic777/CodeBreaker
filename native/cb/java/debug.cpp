#include "cb/java/debug.hpp"

#include <algorithm>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::debug {

namespace {

// The three files, as the C++ and C they ARE -- the text is the product, so it is held verbatim. (A raw string
// literal may not pass 16 KB on MSVC; each is well under.)
constexpr std::string_view kDebugH = R"DBGH(// Debug.h -- SINGLE toggle point for ALL debug instrumentation (CodeBreaker-emitted).
// ==========================================================================
//  Flip DEBUG_ENABLED to 0 for retail: every macro below becomes a no-op and
//  every debug site in the codebase that routes through these macros goes dark.
//  This is the ONLY place you need to touch. Do not add raw printf/OutputDebug-
//  String debug code elsewhere -- route it through a DBG_* macro so this switch
//  governs it. (CodeBreaker flags stray debug output as J2X-REVIEW[debug-bypass].)
// ==========================================================================
#ifndef J2X_DEBUG_H
#define J2X_DEBUG_H

// ---- MASTER SWITCH -------------------------------------------------------
#ifndef DEBUG_ENABLED
#define DEBUG_ENABLED 1          // 1 = debug build, 0 = retail (all DBG_* vanish)
#endif

// ---- per-category switches (only meaningful when DEBUG_ENABLED) ----------
// Silence one class without touching call sites.
#define DBG_CAT_BUG    1         // invariant/assert/logic errors
#define DBG_CAT_LEAK   1         // allocation + leak tracking (summarized)
#define DBG_CAT_CRASH  1         // soft (recovered) + hard (fatal) context
#define DBG_CAT_NET    1         // connects, latency, packet loss (summarized)
#define DBG_CAT_PERF   1         // hot-path / frame-budget spikes

// ---- category ids --------------------------------------------------------
enum DbgCat { DBGC_BUG = 0, DBGC_LEAK, DBGC_CRASH, DBGC_NET, DBGC_PERF, DBGC_COUNT };

// ---- backend (implemented in Debug.cpp) ----------------------------------
void  Debug_Printf(int cat, const char* file, int line, const char* fmt, ...);
bool  Debug_Throttle(const void* siteKey, unsigned intervalMs);   // true => allowed to print now
void  Debug_LeakTrack(void* p, unsigned long size, const char* file, int line);
void  Debug_LeakFree(void* p);
void  Debug_NetTally(int lostPackets, int totalPackets, int rttMs);
void  Debug_Summary();          // print accumulated LEAK/NET summary (call at shutdown + on demand)
void  Debug_HardCrash(const char* file, int line, const char* fmt, ...);  // logs + breaks/aborts

// Every macro below carries a greppable // J2X-DEBUG[tag] marker so the WHOLE debug surface is
// catalogued: `grep -rn "J2X-DEBUG" .` enumerates every debug facility, and any call site can be
// found the same way. Nothing debug-related is emitted without one of these markers.
#if DEBUG_ENABLED

#define DBG_ON(cat) (cat)
// basic, every hit (use sparingly -- prefer the rate-limited forms):
#define DBG(cat, catflag, ...) do { if (catflag) Debug_Printf((cat), __FILE__, __LINE__, __VA_ARGS__); } while (0)  // J2X-DEBUG[dbg]

// once per call site (static latch) -- the default for "something happened here":
#define DBG_ONCE(cat, catflag, ...) do { static bool _dbg_seen=false; \
    if ((catflag) && !_dbg_seen) { _dbg_seen=true; Debug_Printf((cat), __FILE__, __LINE__, __VA_ARGS__); } } while (0)  // J2X-DEBUG[once]

// every Nth hit -- for high-frequency events you still want a pulse from:
#define DBG_EVERY(cat, catflag, n, ...) do { static unsigned _dbg_n=0; \
    if ((catflag) && (++_dbg_n % (n))==0) Debug_Printf((cat), __FILE__, __LINE__, __VA_ARGS__); } while (0)  // J2X-DEBUG[every]

// at most once per intervalMs at this site -- for per-frame conditions:
#define DBG_THROTTLE(cat, catflag, ms, ...) do { static char _k; \
    if ((catflag) && Debug_Throttle(&_k, (ms))) Debug_Printf((cat), __FILE__, __LINE__, __VA_ARGS__); } while (0)  // J2X-DEBUG[throttle]

// invariant check -> BUG channel (does NOT abort; soft):
#define DBG_ASSERT(cond, ...) do { if (DBG_CAT_BUG && !(cond)) \
    Debug_Printf(DBGC_BUG, __FILE__, __LINE__, "ASSERT(" #cond ") " __VA_ARGS__); } while (0)  // J2X-DEBUG[assert]

// crash context:
#define DBG_CRASH_SOFT(...) do { if (DBG_CAT_CRASH) Debug_Printf(DBGC_CRASH, __FILE__, __LINE__, "SOFT: " __VA_ARGS__); } while (0)  // J2X-DEBUG[crash-soft]
#define DBG_CRASH_HARD(...) Debug_HardCrash(__FILE__, __LINE__, __VA_ARGS__)  // J2X-DEBUG[crash-hard]

// leak tracking (accumulated; see Debug_Summary):
#define DBG_LEAK_NEW(p, size)  do { if (DBG_CAT_LEAK) Debug_LeakTrack((p), (size), __FILE__, __LINE__); } while (0)  // J2X-DEBUG[leak-new]
#define DBG_LEAK_DELETE(p)     do { if (DBG_CAT_LEAK) Debug_LeakFree((p)); } while (0)  // J2X-DEBUG[leak-free]

// networking: tally loss/latency, summarized rather than per-packet spam:
#define DBG_NET(lost, total, rtt) do { if (DBG_CAT_NET) Debug_NetTally((lost), (total), (rtt)); } while (0)  // J2X-DEBUG[net]
#define DBG_NET_EVENT(...)         do { if (DBG_CAT_NET) Debug_Printf(DBGC_NET, __FILE__, __LINE__, __VA_ARGS__); } while (0)  // J2X-DEBUG[net-event]

#define DBG_SUMMARY() Debug_Summary()  // J2X-DEBUG[summary]

#else  // DEBUG_ENABLED == 0  -> everything vanishes

#define DBG(cat, catflag, ...)              ((void)0)
#define DBG_ONCE(cat, catflag, ...)         ((void)0)
#define DBG_EVERY(cat, catflag, n, ...)     ((void)0)
#define DBG_THROTTLE(cat, catflag, ms, ...) ((void)0)
#define DBG_ASSERT(cond, ...)               ((void)0)
#define DBG_CRASH_SOFT(...)                 ((void)0)
#define DBG_CRASH_HARD(...)                 ((void)0)
#define DBG_LEAK_NEW(p, size)               ((void)0)
#define DBG_LEAK_DELETE(p)                  ((void)0)
#define DBG_NET(lost, total, rtt)           ((void)0)
#define DBG_NET_EVENT(...)                  ((void)0)
#define DBG_SUMMARY()                       ((void)0)

#endif // DEBUG_ENABLED
#endif // J2X_DEBUG_H
)DBGH";
constexpr std::string_view kDebugCpp = R"DBGC(// Debug.cpp -- backend for the single debug file (CodeBreaker-emitted). See Debug.h.
// J2X-ALWAYS-LINK: the debug facility is a developer tool that must ship whenever debug is enabled even
// if no game code references it yet (you add Debug:: log calls while chasing a bug). The dead-subsystem
// prune treats this file as a reachability ROOT and never cuts it. NOTE: the boot/error trace the port
// relies on (DbgPrintF) lives in Common.h and is independent of this file.
#include "stdafx.h"                                          // forced PCH: must be the first include
#include "Debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#if defined(_MSC_VER) && !defined(snprintf)
  #define snprintf _snprintf                                // MSVC/Xbox 360 CRT spells it _snprintf
#endif

#if defined(_XBOX) || defined(_XENON)
  #include <xtl.h>
  #define DBG_SINK(s)   OutputDebugStringA(s)
  #define DBG_NOW_MS()  ((unsigned)GetTickCount())
  #define DBG_BREAK()   DebugBreak()
#elif defined(_WIN32)
  #include <windows.h>
  #define DBG_SINK(s)   OutputDebugStringA(s)
  #define DBG_NOW_MS()  ((unsigned)GetTickCount())
  #define DBG_BREAK()   DebugBreak()
#else
  #include <time.h>
  #define DBG_SINK(s)   fputs((s), stderr)
  static unsigned DBG_NOW_MS() { return (unsigned)(clock() * 1000u / CLOCKS_PER_SEC); }
  #define DBG_BREAK()   (*(volatile int*)0 = 0)
#endif

#if DEBUG_ENABLED

static const char* const kCatName[DBGC_COUNT] = { "BUG", "LEAK", "CRASH", "NET", "PERF" };

void Debug_Printf(int cat, const char* file, int line, const char* fmt, ...)  // J2X-DEBUG[printf]
{
    char msg[512];
    va_list ap; va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    // strip path, keep basename -- concise
    const char* base = file; for (const char* p = file; *p; ++p) if (*p=='/'||*p=='\\') base = p+1;
    char out[640];
    const char* name = (cat>=0 && cat<DBGC_COUNT) ? kCatName[cat] : "DBG";
    snprintf(out, sizeof(out), "[%-5s] %s:%d  %s\n", name, base, line, msg);
    DBG_SINK(out);
}

// ---- throttle: at most one print per interval at a site keyed by &static ----
struct ThrottleRec { const void* key; unsigned last; };
static ThrottleRec g_thr[256];
static int g_thrN = 0;
bool Debug_Throttle(const void* siteKey, unsigned intervalMs)  // J2X-DEBUG[throttle-impl]
{
    unsigned now = DBG_NOW_MS();
    for (int i = 0; i < g_thrN; ++i)
        if (g_thr[i].key == siteKey) {
            if (now - g_thr[i].last < intervalMs) return false;
            g_thr[i].last = now; return true;
        }
    if (g_thrN < 256) { g_thr[g_thrN].key = siteKey; g_thr[g_thrN].last = now; ++g_thrN; }
    return true;
}

// ---- leak tally: accumulate, summarize -- never a line per alloc ----
static unsigned long g_liveBytes = 0, g_peakBytes = 0;
static long g_liveCount = 0, g_totalAllocs = 0;
void Debug_LeakTrack(void* p, unsigned long size, const char*, int)  // J2X-DEBUG[leak-impl]
{
    if (!p) return;
    g_liveBytes += size; ++g_liveCount; ++g_totalAllocs;
    if (g_liveBytes > g_peakBytes) g_peakBytes = g_liveBytes;
}
void Debug_LeakFree(void* p) { if (p && g_liveCount > 0) --g_liveCount; }  // J2X-DEBUG[leak-free-impl]

// ---- net tally: accumulate loss/latency, summarize ----
static long g_pktLost = 0, g_pktTotal = 0, g_rttSum = 0, g_rttN = 0;
void Debug_NetTally(int lost, int total, int rttMs)  // J2X-DEBUG[net-impl]
{
    g_pktLost += lost; g_pktTotal += total;
    if (rttMs >= 0) { g_rttSum += rttMs; ++g_rttN; }
    // only SURFACE when loss crosses a threshold, throttled -- otherwise silent accumulation
    if (total > 0 && lost * 100 >= total * 5) {          // >=5% loss burst
        static char k;
        if (Debug_Throttle(&k, 2000))
            Debug_Printf(DBGC_NET, "net", 0, "packet loss %d/%d (%.0f%%) rtt~%dms",
                         lost, total, 100.0 * lost / total, rttMs);
    }
}

void Debug_Summary()  // J2X-DEBUG[summary-impl]
{
    char out[320];
    snprintf(out, sizeof(out),
             "[SUMMARY] mem live=%luB peak=%luB liveAllocs=%ld totalAllocs=%ld | "
             "net lost=%ld/%ld avgRtt=%ldms\n",
             g_liveBytes, g_peakBytes, g_liveCount, g_totalAllocs,
             g_pktLost, g_pktTotal, g_rttN ? g_rttSum / g_rttN : 0);
    DBG_SINK(out);
    if (g_liveCount > 0) {
        snprintf(out, sizeof(out), "[LEAK ] %ld allocation(s) still live at summary (%lu bytes)\n",
                 g_liveCount, g_liveBytes);
        DBG_SINK(out);
    }
}

void Debug_HardCrash(const char* file, int line, const char* fmt, ...)  // J2X-DEBUG[crash-impl]
{
    char msg[512];
    va_list ap; va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    Debug_Printf(DBGC_CRASH, file, line, "HARD: %s", msg);
    Debug_Summary();
    DBG_BREAK();
}

#else  // DEBUG_ENABLED == 0 : real symbols still exist (so non-macro callers link) but do nothing

void Debug_Printf(int, const char*, int, const char*, ...) {}
bool Debug_Throttle(const void*, unsigned) { return false; }
void Debug_LeakTrack(void*, unsigned long, const char*, int) {}
void Debug_LeakFree(void*) {}
void Debug_NetTally(int, int, int) {}
void Debug_Summary() {}
void Debug_HardCrash(const char*, int, const char*, ...) {}

#endif // DEBUG_ENABLED
)DBGC";
constexpr std::string_view kDebugReadme = R"DBGR(# DEBUG.md -- the single debug file

All debug instrumentation for the port lives behind one switch: `DEBUG_ENABLED` in `Debug.h`.

- **Retail build:** set `DEBUG_ENABLED 0` (or define it in the project's Release config). Every `DBG_*`
  macro compiles to `((void)0)`, so all debug code across the codebase goes dark from this one place.
- **Debug build:** leave it `1`. Toggle individual channels with `DBG_CAT_BUG/LEAK/CRASH/NET/PERF`.

## Rules (so it never becomes console spam)

| Need | Macro | Behavior |
|---|---|---|
| Mark a spot hit once | `DBG_ONCE(DBGC_BUG, DBG_CAT_BUG, "...")` | prints once per call site |
| High-frequency pulse | `DBG_EVERY(DBGC_PERF, DBG_CAT_PERF, 600, "...")` | every 600th hit |
| Per-frame condition | `DBG_THROTTLE(DBGC_NET, DBG_CAT_NET, 1000, "...")` | at most once/second/site |
| Invariant | `DBG_ASSERT(ptr != 0, "null chunk")` | BUG channel, does not abort |
| Recovered error | `DBG_CRASH_SOFT("bad id %d", id)` | one line |
| Fatal | `DBG_CRASH_HARD("heap corrupt")` | logs + summary + break |
| Allocation | `DBG_LEAK_NEW(p, size)` / `DBG_LEAK_DELETE(p)` | accumulated, not per-line |
| Networking | `DBG_NET(lost, total, rttMs)` | silent until >=5% loss burst |
| Dump totals | `DBG_SUMMARY()` | mem live/peak + net loss + live allocs |

Leaks and packet loss are **accumulated and summarized** (`DBG_SUMMARY()` at shutdown / on a key),
never a line per event -- so the console stays readable and a real leak stands out.

Do **not** add raw `printf` / `OutputDebugString` debug code elsewhere; route it through a `DBG_*`
macro so the master switch governs it. The converter flags stray debug output as
`J2X-REVIEW[debug-bypass]`.
)DBGR";

}  // namespace

std::string debug_header(bool enabled_default) {
    return enabled_default ? std::string(kDebugH) : text::replace_all(kDebugH, "#define DEBUG_ENABLED 1", "#define DEBUG_ENABLED 0");
}

std::string_view debug_cpp() { return kDebugCpp; }

std::string_view debug_readme() { return kDebugReadme; }

std::vector<std::filesystem::path> write_debug(const std::filesystem::path& out_dir, bool enabled_default) {
    const std::vector<std::filesystem::path> paths = {out_dir / "Debug.h", out_dir / "Debug.cpp", out_dir / "DEBUG.md"};
    fs::write_text(paths[0], debug_header(enabled_default));
    fs::write_text(paths[1], kDebugCpp);
    fs::write_text(paths[2], kDebugReadme);
    return paths;
}

namespace {

constexpr std::string_view kMarker = "J2X-DEBUG[";

bool is_tag_char(char c) { return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-'; }

// The tag of the first well-formed `J2X-DEBUG[tag]` on the line (tag is [a-z0-9-]+), or empty.
std::string_view marker_tag(std::string_view line) {
    for (std::size_t at = line.find(kMarker); at != std::string_view::npos; at = line.find(kMarker, at + 1)) {
        std::size_t i = at + kMarker.size();
        const std::size_t start = i;
        while (i < line.size() && is_tag_char(line[i])) ++i;
        if (i > start && i < line.size() && line[i] == ']') return line.substr(start, i - start);
    }
    return {};
}

}  // namespace

std::map<std::string, std::vector<MarkerSite>> scan_debug_markers(const std::filesystem::path& out_dir) {
    std::error_code ec;
    if (!std::filesystem::is_directory(out_dir, ec)) throw NotFoundError(out_dir, 0, "not a folder");
    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(out_dir)) {
        const std::string name = fs::to_text(entry.path().filename());
        if (entry.is_regular_file() && (name.ends_with(".cpp") || name.ends_with(".h"))) files.push_back(entry.path());
    }
    std::sort(files.begin(), files.end());

    std::map<std::string, std::vector<MarkerSite>> out;
    for (const auto& file : files) {
        const auto content = textutil::try_read_text(file);
        if (!content || content->find("J2X-DEBUG") == std::string::npos) continue;
        std::uint32_t number = 0;
        for (std::string_view line : text::split(*content, '\n')) {
            ++number;
            if (std::string_view tag = marker_tag(line); !tag.empty()) {
                out[std::string(tag)].push_back({fs::to_text(file.filename()), number});
            }
        }
    }
    return out;
}

}  // namespace cb::java::debug

