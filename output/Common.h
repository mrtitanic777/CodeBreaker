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

// --- Macros ---
#define DEG_TO_RAD (D3DX_PI / 180.0f)
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

// --- Random helper (replaces java.util.Random) ---
class Random {
public:
    Random() { seed = GetTickCount(); }
    Random(unsigned long long s) { seed = (unsigned long long)s; }
    void setSeed(long long s) { seed = (unsigned long long)s; }
    int nextInt(int bound) {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        return (int)((seed >> 33) % bound);
    }
    int nextInt() { return nextInt(0x7FFFFFFF); }
    float nextFloat() { return (float)nextInt(10000) / 10000.0f; }
    double nextDouble() { return (double)nextFloat(); }
    double nextGaussian() { return (nextFloat() - 0.5f) * 2.0f; }
    bool nextBoolean() { return nextInt(2) == 0; }
    long long nextLong() { seed = seed * 6364136223846793005ULL + 1; return (long long)seed; }
public: // was private
    unsigned long long seed;
};

// --- GetNanoTime ---
inline long long GetNanoTime() {
    LARGE_INTEGER counter, freq;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&freq);
    return (long long)((double)counter.QuadPart * 1000000000.0 / (double)freq.QuadPart);
}

// --- DbgPrint format helper ---
inline void DbgPrintF(const char* fmt, ...) {
    char buf[512];
    va_list args;
    va_start(args, fmt);
    _vsnprintf(buf, sizeof(buf)-1, fmt, args);
    va_end(args);
    buf[sizeof(buf)-1] = 0;
    OutputDebugStringA(buf);
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
