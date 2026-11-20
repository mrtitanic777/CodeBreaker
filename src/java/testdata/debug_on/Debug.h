// Debug.h -- SINGLE toggle point for ALL debug instrumentation (CodeBreaker-emitted).
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
