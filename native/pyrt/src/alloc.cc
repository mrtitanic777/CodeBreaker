// alloc.cc -- the program's allocator for small blocks: operator new / delete of up to 512 bytes come from
// free lists by size class (16-byte steps), each thread keeping its own, carved from 1 MB slabs of one
// reserved address range; larger blocks, and any block not from that range, are malloc's and free's.
//
// A translated program allocates as CPython does -- an object, a vector of arguments, a string, a bound
// method at every turn -- and the process heap was a quarter of its time (RtlAllocateHeap / RtlFreeHeap).
// CPython has pymalloc for the same reason. PYRT_MALLOC=1 in the environment turns this off (every block
// malloc's), to tell a memory bug from the allocator's.
#include <windows.h>

#include <cstdint>
#include <cstdlib>
#include <mutex>
#include <new>

namespace {

constexpr size_t kMax = 512;                     // largest pooled block
constexpr size_t kStep = 16;
constexpr int kClasses = (int)(kMax / kStep);    // 16, 32, ... 512
constexpr size_t kSlab = size_t(1) << 20;        // committed at a time, all of one class
constexpr size_t kRange = size_t(64) << 30;      // address space reserved once
constexpr int kBatch = 64;                       // blocks a thread takes or gives back at a time
constexpr int kKeep = 512;                       // a thread's list beyond this goes back to the shared one

struct Block { Block* next; };

char* g_base = nullptr;                          // the reserved range (null: not set up, or off)
char* g_top = nullptr;                           // next slab not yet committed
uint8_t* g_class = nullptr;                      // the class of each slab (index by (p - base) / kSlab)
int g_state = 0;                                 // 0 not set up, 1 on, -1 off
std::mutex* g_mu = nullptr;

struct Shared {
    Block* list[kClasses];
    char* bump[kClasses];
    char* bump_end[kClasses];
};
Shared g_shared;

// (plain data: no constructor or destructor runs for it, so new and delete work before main and after a
// thread's own destructors; what a thread holds when it ends stays with it -- at most kKeep per class)
struct Cache {
    Block* list[kClasses];
    int count[kClasses];
};
thread_local Cache t_cache;

void set_up() {
    // (the first new: before main, one thread)
    char e[8];
    size_t n = 0;
    if (getenv_s(&n, e, sizeof e, "PYRT_MALLOC") == 0 && n > 1 && e[0] == '1') {
        g_state = -1;
        return;
    }
    char* base = (char*)VirtualAlloc(nullptr, kRange, MEM_RESERVE, PAGE_NOACCESS);
    uint8_t* cls = (uint8_t*)VirtualAlloc(nullptr, kRange / kSlab, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!base || !cls) {
        g_state = -1;
        return;
    }
    static std::mutex mu;   // (constant-initialised)
    g_mu = &mu;
    g_class = cls;
    g_base = g_top = base;
    g_state = 1;
}

inline int class_of(size_t n) { return n ? (int)((n - 1) / kStep) : 0; }

// blocks for this thread's list of class c (the shared list first, then new memory); false: none to be had
bool refill(Cache& t, int c) {
    std::lock_guard<std::mutex> l(*g_mu);
    Shared& s = g_shared;
    int got = 0;
    while (s.list[c] && got < kBatch) {
        Block* b = s.list[c];
        s.list[c] = b->next;
        b->next = t.list[c];
        t.list[c] = b;
        got++;
    }
    if (got) {
        t.count[c] += got;
        return true;
    }
    size_t size = (size_t)(c + 1) * kStep;
    if (s.bump[c] + size > s.bump_end[c] || !s.bump[c]) {
        if (g_top + kSlab > g_base + kRange) return false;
        if (!VirtualAlloc(g_top, kSlab, MEM_COMMIT, PAGE_READWRITE)) return false;
        g_class[(size_t)(g_top - g_base) / kSlab] = (uint8_t)c;
        s.bump[c] = g_top;
        s.bump_end[c] = g_top + kSlab;
        g_top += kSlab;
    }
    for (; got < kBatch && s.bump[c] + size <= s.bump_end[c]; got++) {
        Block* b = (Block*)s.bump[c];
        s.bump[c] += size;
        b->next = t.list[c];
        t.list[c] = b;
    }
    t.count[c] += got;
    return true;
}

void give_back(Cache& t, int c) {
    std::lock_guard<std::mutex> l(*g_mu);
    for (int i = 0; i < kBatch && t.list[c]; i++) {
        Block* b = t.list[c];
        t.list[c] = b->next;
        b->next = g_shared.list[c];
        g_shared.list[c] = b;
        t.count[c]--;
    }
}

inline void* alloc(size_t n) {
    if (n <= kMax) {
        if (g_state == 0) set_up();
        if (g_state > 0) {
            int c = class_of(n);
            Cache& t = t_cache;
            Block* b = t.list[c];
            if (!b && refill(t, c)) b = t.list[c];
            if (b) {
                t.list[c] = b->next;
                t.count[c]--;
                return b;
            }
        }
    }
    return malloc(n ? n : 1);
}

inline void release(void* p) {
    if (!p) return;
    char* q = (char*)p;
    if (g_base && q >= g_base && q < g_base + kRange) {   // (the whole range: a slab another thread just made is in it)
        int c = g_class[(size_t)(q - g_base) / kSlab];
        Cache& t = t_cache;
        Block* b = (Block*)p;
        b->next = t.list[c];
        t.list[c] = b;
        if (++t.count[c] > kKeep) give_back(t, c);
        return;
    }
    free(p);
}

void* alloc_or_throw(size_t n) {
    for (;;) {
        if (void* p = alloc(n)) return p;
        std::new_handler h = std::get_new_handler();
        if (!h) throw std::bad_alloc();
        h();
    }
}

}  // namespace

void* operator new(size_t n) { return alloc_or_throw(n); }
void* operator new[](size_t n) { return alloc_or_throw(n); }
void* operator new(size_t n, const std::nothrow_t&) noexcept { return alloc(n); }
void* operator new[](size_t n, const std::nothrow_t&) noexcept { return alloc(n); }
void operator delete(void* p) noexcept { release(p); }
void operator delete[](void* p) noexcept { release(p); }
void operator delete(void* p, size_t) noexcept { release(p); }
void operator delete[](void* p, size_t) noexcept { release(p); }
void operator delete(void* p, const std::nothrow_t&) noexcept { release(p); }
void operator delete[](void* p, const std::nothrow_t&) noexcept { release(p); }
