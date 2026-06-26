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
private:
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

#endif // COMMON_H
