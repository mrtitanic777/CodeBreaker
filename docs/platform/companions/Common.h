#ifndef COMMON_H
#define COMMON_H

#include "stdafx.h"

// --- Globals ---
extern IDirect3D9*              g_pD3D;
extern IDirect3DDevice9*        g_pd3dDevice;
extern IDirect3DTexture9*       g_terrainTexture;
extern IDirect3DTexture9*       g_guiTexture;
extern IDirect3DTexture9*       g_charTexture;
extern IDirect3DTexture9*       g_defaultTexture;

// Re-establish the game's baseline D3D render state (z, cull, blend, sampler WRAP + mip/point filtering,
// half-pixel offset). Called once at boot, and again after the XUI menu/loading renders (which leave the
// device in XUI's state -- LINEAR mag + CLAMP addressing) so the world doesn't render blurry/smeared.
void ApplyDefaultRenderState();

// --- Macros ---
#define DEG_TO_RAD (D3DX_PI / 180.0f)
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

// --- Random ---
// The j2ctool runtime provides the canonical Random (faithful Java 48-bit LCG) in Random.h, included
// by JavaRT.h everywhere. Use it here too instead of redefining (the old non-Java LCG that lived
// here would clash with the runtime one and produce different worlds from the same seed).
#include "Random.h"

// --- GetNanoTime ---
inline long long GetNanoTime() {
    LARGE_INTEGER counter, freq;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&freq);
    return (long long)((double)counter.QuadPart * 1000000000.0 / (double)freq.QuadPart);
}

// J2X-BUGFIX 2026-08-13 nexia-log-channel: Nexia360 SWALLOWS OutputDebugStringA AND the raw kernel
// DbgPrint on its own -- so boot logging was invisible and the black-screen looked un-debuggable. The one
// channel it always surfaces is HostPathDevice::ResolvePath. DbgPrintF below mirrors every message into a
// throwaway CreateFileA("game:\\J2X_<msg>") so it shows as `ResolvePath(\J2X_<msg>)`, plus a durable
// game:\J2X_trace.txt append. (After main() is reached, the (DbgPrint) lines DO appear -- but the resolve
// marker is the reliable one.) RULE: sparse one-shot calls only, never per-frame (that flooded the log).
// --- DbgPrint format helper (the port's ONE debug-output apparatus) ---
// Formats like printf and emits a single line to the host debug channel: OutputDebugStringA is what
// actually surfaces in the Nexia360 / kernel debug log; DbgPrint is the raw kernel print alongside it.
//
// THE RULE (learned the hard way): only call this at SPARSE, one-shot events -- boot checkpoints, world
// load/save, a caught error. NEVER from a per-frame / per-tick / per-entity path. It was briefly no-op'd
// after per-tick call sites flooded the log to multiple GB and crashed the host; that was a CALL-SITE
// mistake, not a reason to kill the mechanism. Flip J2X_DEBUG_LOG to 0 to compile every site out to nothing.
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
// TOGGLE: DbgPrintF file-mirror logging. Default OFF -- each call does a CreateFile (filesystem spam),
// so it must NEVER drive per-frame/per-packet logging. Flip to 1 (or override in the build) only for
// sparse one-shot boot/error tracing. Independent of the heap watchdog (J2X_HEAPTRACK).
#ifndef J2X_DEBUG_LOG
#define J2X_DEBUG_LOG 0
#endif
extern "C" unsigned long __cdecl DbgPrint(const char* Format, ...);
inline void DbgPrintF(const char* fmt, ...) {
#if J2X_DEBUG_LOG
    char buf[512];
    va_list ap; va_start(ap, fmt);
    _vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
    va_end(ap);
    buf[sizeof(buf) - 1] = '\0';
    OutputDebugStringA(buf);   // debug channel: works on real hw / other tools, but Nexia360 SWALLOWS it
    DbgPrint("%s", buf);       // raw kernel print: also swallowed by Nexia360
    // Nexia360 does NOT surface the debug channel -- but it logs EVERY HostPathDevice::ResolvePath. So
    // mirror the message into a throwaway file-open the emulator WILL print, in order:
    //     F> ... HostPathDevice::ResolvePath(\J2X_<message>)
    // Use raw CreateFileA (NOT fopen -- the CRT rejects paths with spaces/brackets/subdirs BEFORE the
    // device layer, so no line is logged). Root-level clean path, exactly like the \xam.xex probe we KNOW
    // logs: keep only [A-Za-z0-9], map space/._- to '_', DROP everything else. The file never exists (we
    // only want the logged resolve). Sparse one-shot calls only -- NEVER per-frame (that flooded the log).
    char path[128];
    strcpy(path, "game:\\J2X_");
    size_t j = strlen(path);
    for (size_t i = 0; buf[i] && j < sizeof(path) - 1; ++i) {
        char c = buf[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) path[j++] = c;
        else if (c == ' ' || c == '_' || c == '.' || c == '-') path[j++] = '_';
        // else: skip (brackets, colon, slash, newline, etc.)
    }
    path[j] = '\0';
    HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    // DURABLE disk trace -- the channel that does NOT depend on the emulator surfacing anything OR on the
    // log surviving a force-close. game:\ maps to the .xex OutDir on the host (Debug\), so this lands at
    // Debug\J2X_trace.txt, readable directly from Windows. Append + fflush + fclose per line => the last
    // line written is always durably on disk, even if the very next call hangs or faults.
    FILE* tf = fopen("game:\\J2X_trace.txt", "a");
    if (tf) { fputs(buf, tf); fflush(tf); fclose(tf); }
#else
    (void)fmt;
#endif
}

// --- [HEAPCHK] diagnostic: poke the REAL process heap (the one RtlValidateHeap@40000000 guards) with
// a malloc/free; the 360 heap auto-validates on free, so the LAST "[HEAPCHK] ck: X" printed right
// before an "Invalid Address ... RtlValidateHeap" message names the phase X that corrupted it.
// (_heapchk() checks a DIFFERENT/CRT heap and is blind to this corruption.) Capped to limit spam.
#include <malloc.h>
inline void DbgHeapChk(const char* where) {
    static int _n = 0;
    if (_n >= 4000) return;   // PORT(diag): raised so in-world per-frame probes work (was 70)
    ++_n;
    DbgPrintF("[HEAPCHK] ck: %s\n", where);
    void* p = malloc(64);
    if (p) free(p);
}

// --- Math helpers (Java equivalents) ---
inline float signumf(float x) { return (x > 0.0f) ? 1.0f : (x < 0.0f) ? -1.0f : 0.0f; }

// --- Java string concatenation helpers ('"" + x' / 'str + x') ---
// Java implicitly converts numbers to strings in concatenation; C++ does not.
inline std::string toStr(const std::string& s) { return s; }
inline std::string toStr(const char* s)        { return std::string(s ? s : ""); }
inline std::string toStr(int v)                { char b[32]; sprintf(b, "%d", v); return b; }
inline std::string toStr(unsigned int v)       { char b[32]; sprintf(b, "%u", v); return b; }
inline std::string toStr(long long v)          { char b[32]; sprintf(b, "%lld", v); return b; }
inline std::string toStr(unsigned char v)      { char b[32]; sprintf(b, "%d", (int)v); return b; }
inline std::string toStr(char v)               { return std::string(1, v); }
inline std::string toStr(bool v)               { return v ? "true" : "false"; }
inline std::string toStr(float v)              { char b[32]; sprintf(b, "%g", (double)v); return b; }
inline std::string toStr(double v)             { char b[32]; sprintf(b, "%g", v); return b; }

// --- Java String API helpers (no direct std::string equivalent) ---
inline bool javaStartsWith(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && s.compare(0, p.size(), p) == 0;
}
inline bool javaEndsWith(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && s.compare(s.size() - p.size(), p.size(), p) == 0;
}
inline int javaIndexOf(const std::string& s, const std::string& x) {
    size_t p = s.find(x); return p == std::string::npos ? -1 : (int)p;
}
inline int javaIndexOf(const std::string& s, char c) {
    size_t p = s.find(c); return p == std::string::npos ? -1 : (int)p;
}
inline std::string javaToLowerCase(std::string s) {
    for (size_t i = 0; i < s.size(); ++i) s[i] = (char)tolower((unsigned char)s[i]); return s;
}
inline std::string javaToUpperCase(std::string s) {
    for (size_t i = 0; i < s.size(); ++i) s[i] = (char)toupper((unsigned char)s[i]); return s;
}
inline std::string javaTrim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && (unsigned char)s[a] <= ' ') ++a;
    while (b > a && (unsigned char)s[b-1] <= ' ') --b;
    return s.substr(a, b - a);
}

// --- Java List.remove() helpers ---
// Java has remove(int index) [removes by position, returns the element] and remove(void*)
// [removes by value, returns bool]. Two distinctly-named helpers keep these unambiguous even
// for vector<int> (where a single overloaded name would be ambiguous). Both work in
// expression context (ternary / assignment), so 'list.remove(i)' -> '_vecRemoveAt(list, i)'.
template<class V> typename V::value_type _vecRemoveAt(V& v, int i) {
    typename V::value_type e = v[i];
    v.erase(v.begin() + i);
    return e;
}
template<class V, class T> bool _vecRemoveObj(V& v, const T& o) {
    typename V::iterator it = std::find(v.begin(), v.end(), o);
    if (it != v.end()) { v.erase(it); return true; }
    return false;
}

#endif // COMMON_H
