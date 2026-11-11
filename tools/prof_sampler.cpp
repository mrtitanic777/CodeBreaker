// In-process sampling profiler: link into a harness test.exe (build with /Zi and link /DEBUG).
// A static constructor starts a sampler thread; at exit it prints the top self and inclusive functions to stderr.
// Usage: add tools\prof_sampler.cpp to the cl line with /Zi /O2 /link /DEBUG.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <dbghelp.h>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <cstring>
#pragma comment(lib, "dbghelp.lib")

namespace {
HANDLE g_main;
volatile bool g_stop = false;
HANDLE g_thr;
constexpr int DEPTH = 32;
constexpr int MAXS = 400000;
DWORD64* g_buf;            // MAXS * DEPTH return addresses, preallocated: the sampler never allocates
int* g_depth;
int g_samples = 0;

DWORD WINAPI sampler(LPVOID) {
    while (!g_stop && g_samples < MAXS) {
        Sleep(4);
        if (SuspendThread(g_main) == (DWORD)-1) continue;
        CONTEXT c; c.ContextFlags = CONTEXT_FULL;
        if (GetThreadContext(g_main, &c)) {
            DWORD64* out = g_buf + (size_t)g_samples * DEPTH;
            int n = 0;
            for (; n < DEPTH; n++) {
                out[n] = c.Rip;
                DWORD64 base = 0;
                PRUNTIME_FUNCTION rf = RtlLookupFunctionEntry(c.Rip, &base, nullptr);
                if (!rf) { if (!c.Rsp) break; c.Rip = *(DWORD64*)c.Rsp; c.Rsp += 8; }
                else {
                    PVOID hd; DWORD64 ef[2];
                    RtlVirtualUnwind(UNW_FLAG_NHANDLER, base, c.Rip, rf, &c, &hd, ef, nullptr);
                }
                if (!c.Rip) { n++; break; }
            }
            g_depth[g_samples] = n;
            g_samples++;
        }
        ResumeThread(g_main);
    }
    return 0;
}

std::map<DWORD64, std::string> g_names;
std::string name_of(DWORD64 a) {
    auto it = g_names.find(a);
    if (it != g_names.end()) return it->second;
    char buf[sizeof(SYMBOL_INFO) + 512];
    SYMBOL_INFO* s = (SYMBOL_INFO*)buf;
    s->SizeOfStruct = sizeof(SYMBOL_INFO); s->MaxNameLen = 500;
    DWORD64 d = 0;
    std::string r;
    if (SymFromAddr(GetCurrentProcess(), a, &d, s)) r = s->Name; else { char t[32]; sprintf(t, "?%llx", a); r = t; }
    if (r.size() > 140) r.resize(140);
    g_names[a] = r;
    return r;
}

void report() {
    g_stop = true;
    WaitForSingleObject(g_thr, 2000);
    std::map<std::string, int> self, incl, rolled, rolled2, heapc, focus;
    const char* fo = getenv("PROF_FOCUS");
    auto generic = [](const std::string& n) {
        static const char* pre[] = {"Rtl", "_", "operator", "std::", "mem", "Heap", "free", "malloc", "`anonymous namespace'::_Finding", "?", "ntdll", "Base", "Nt", "Zw", "Sleep", "SRW", "Acquire"};
        for (auto p : pre) if (n.compare(0, strlen(p), p) == 0) return true;
        return false;
    };
    for (int k = 0; k < g_samples; k++) {
        std::set<std::string> seen;
        for (int i = 0; i < g_depth[k]; i++) {
            std::string n = name_of(g_buf[(size_t)k * DEPTH + i]);
            if (i == 0 && fo && n.find(fo) != std::string::npos) {
                std::string key;
                for (int q = 0; q < 7 && q < g_depth[k]; q++) { if (q) key += "  <-  "; key += name_of(g_buf[(size_t)k * DEPTH + q]).substr(0, 60); }
                focus[key]++;
            }
            if (i == 0) {
                self[n]++;
                int j = 0;
                while (j < g_depth[k] && generic(name_of(g_buf[(size_t)k * DEPTH + j]))) j++;
                if (j < g_depth[k]) {
                    rolled[name_of(g_buf[(size_t)k * DEPTH + j])]++;
                    if (j > 0) { const std::string& ln = name_of(g_buf[(size_t)k * DEPTH]); if (ln.find("Heap") != std::string::npos || ln.find("malloc") != std::string::npos || ln.find("free") != std::string::npos || ln.find("operator") != std::string::npos) { std::string key = name_of(g_buf[(size_t)k * DEPTH + j]); if (j + 1 < g_depth[k]) key += "  <-  " + name_of(g_buf[(size_t)k * DEPTH + j + 1]); if (j + 2 < g_depth[k]) key += "  <-  " + name_of(g_buf[(size_t)k * DEPTH + j + 2]); heapc[key]++; } }
                    int j2 = j + 1;
                    if (j2 < g_depth[k]) rolled2[name_of(g_buf[(size_t)k * DEPTH + j]) + "  <-  " + name_of(g_buf[(size_t)k * DEPTH + j2])]++;
                }
            }
            if (seen.insert(n).second) incl[n]++;
        }
    }
    auto dump = [&](const char* title, std::map<std::string, int>& m) {
        std::vector<std::pair<int, std::string>> v;
        for (auto& kv : m) v.push_back({kv.second, kv.first});
        std::sort(v.rbegin(), v.rend());
        fprintf(stderr, "== %s (%d samples)\n", title, g_samples);
        for (size_t i = 0; i < v.size() && i < 110; i++) fprintf(stderr, "%6d %5.1f%%  %s\n", v[i].first, 100.0 * v[i].first / (g_samples ? g_samples : 1), v[i].second.c_str());
    };
    if (fo) dump("FOCUS", focus);
    dump("SELF", self);
    dump("ROLLED (self + runtime internals, charged to the first user function)", rolled);
    dump("ROLLED with caller", rolled2);
    dump("HEAP (alloc/free time) by caller chain", heapc);
    dump("INCLUSIVE", incl);
}

struct Init {
    Init() {
        DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_main, 0, FALSE, DUPLICATE_SAME_ACCESS);
        SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
        SymInitialize(GetCurrentProcess(), nullptr, TRUE);
        g_buf = (DWORD64*)calloc((size_t)MAXS * DEPTH, sizeof(DWORD64)); g_depth = (int*)calloc(MAXS, sizeof(int));
        g_thr = CreateThread(nullptr, 0, sampler, nullptr, 0, nullptr);
        atexit(report);
    }
} g_init;
}
