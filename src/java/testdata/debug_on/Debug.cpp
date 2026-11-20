// Debug.cpp -- backend for the single debug file (CodeBreaker-emitted). See Debug.h.
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
