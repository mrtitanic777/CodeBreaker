// J2X-ALWAYS-LINK
// J2xGc.cpp -- the converted program's garbage collector (PLATFORM COMPANION, PC: Windows x86 and x64; MinGW GCC, Clang, MSVC).
//
// Java frees an object when nothing reaches it; the converted program keeps Java's raw-pointer object model (a field read is a
// load, a call is a call), so the collector works underneath it: it REPLACES the program's global operator new/delete, every
// C++ allocation (objects, container storage, strings, boxes, array blocks) comes from its heap, and a block that nothing
// reaches is reused. Conservative (any word that looks like a pointer into a block keeps it, interior pointers included),
// non-moving, mark-sweep, stop-the-world. Cycles are collected like anything else. Explicit deletes still free at once.
// The design, the alternatives and the measurements: docs/java/RUNTIME_MEMORY.md.
//
// Roots: the program image's writable sections (statics), and the registers and stack of every attached thread. A thread
// attaches on its first allocation, when a Thread starts it, or through the attach hook (JArray.h's j2xHeapHooks); it is
// detached once it has ended. While a Thread is being started, collections wait (its object is reachable only through the new
// OS thread's start argument until the thread runs).
//
// Switches (environment): J2X_GC=0 never collects; J2X_GC_LOG=1 prints each collection (its pause, the live bytes before and
// after, what reached them, what the heap holds from the OS) to stderr; J2X_GC_LOG=2 also where the process's private memory
// is, at each collection and at each new high.
#include "JArray.h"

#if defined(_WIN32) && !defined(_XBOX)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <setjmp.h>
#if defined(_MSC_VER)
#include <intrin.h>
#define J2XGC_NOINLINE __declspec(noinline)
#else
#define J2XGC_NOINLINE __attribute__((noinline))
#endif

namespace j2xgc {

typedef unsigned long long U64;
typedef std::size_t Size;
typedef ULONG_PTR Word;                                  // a pointer-sized unsigned integer

const Size kPage = (Size)1 << 16;                        // the page map's granule: Windows' allocation granularity
const Size kMaxSmall = (Size)128 << 10;                  // larger blocks are regions of their own
const int kSizes = 48;                                   // 16..128 by 16, then four per power of two up to 128 KB
const int kClasses = 2 * kSizes;                         // each size twice: blocks that may hold pointers, and atomic ones
const int kWordBits = (int)(sizeof(Word) * 8);
const Size kL1 = sizeof(void*) == 8 ? (Size)1 << 16 : 1; // address bits 32..47 (a 32-bit program has one entry)
const Size kL2 = (Size)1 << 16;                          // address bits 16..31
const Size kClaimBytes = (Size)16 << 10;                 // a thread takes about this much of a class at a time

// A segment: one class's blocks, or one large block. Its header sits at the start of its region, the two bitmaps after it,
// the blocks after them.
struct Seg {
	char* blocks;
	Size bsize;               // the block size (a large region: its object's size)
	Size bytes;               // the region's size
	U64 recip;                // 2^32 / bsize + 1: offset -> block index by a multiply
	unsigned nblocks, nwords, index;
	unsigned used;            // blocks allocated, as the last sweep counted them
	int cls;                  // -1: a large region
	bool atomic;              // its blocks hold no pointers: never scanned
	Word tail;                // the last bitmap word's bits past nblocks, kept set in `alloc`
	Word* alloc;              // set: allocated (or held in a thread's cache)
	Word* mark;
	Seg* next;                // large regions: a list
	Seg* prev;
	Word largeBits[2];        // a large region's alloc and mark words
};

struct Cls {
	Size size;
	bool atomic;
	Seg** segs;
	unsigned n, cap;
	unsigned cur, curWord;    // where the next claim looks first
	volatile LONG freed;      // a block was freed since the last claim scan reached the end
};

// A thread's blocks in hand, per class: some free bits of one bitmap word it has claimed (set in `alloc`).
struct Local {
	Word mask;
	char* base;               // the block of bit 0 of that word
	Seg* seg;
	unsigned word;
};
struct Cache {
	Local local[kClasses];
	long long delta;          // bytes this thread freed, not yet taken off g_live
};

struct Thr {
	DWORD id;
	HANDLE h;
	Word base;                // the stack's base (highest address)
	Cache* cache;
	bool live;
	bool suspended;
};

struct Range { Word lo, hi; };
struct Item { char* p; Size n; };

// ---- state (g_cs guards all of it, apart from the bitmap words a free clears atomically) ----
CRITICAL_SECTION g_cs;
volatile LONG g_init = 0;                                // 0 none, 1 initializing, 2 ready
Cls g_cls[kClasses];
Seg*** g_l1 = 0;
Word g_lo = ~(Word)0, g_hi = 0;                          // the heap's address bounds
Seg* g_large = 0;
Seg* g_largeFree = 0;                                    // freed large regions kept for reuse (alloc bit clear)
Size g_largeFreeBytes = 0;
const Size kLargeKeep = (Size)16 << 20;                  // at most this much of them, none over a quarter of it
long long g_live = 0;                                    // bytes allocated or held in caches (frees arrive late)
Size g_threshold = 0, g_minThreshold = 0;
bool g_enabled = true, g_log = false, g_collecting = false;
volatile LONG g_starting = 0;                            // Threads being started
Thr* g_thr = 0;
unsigned g_nthr = 0, g_capthr = 0;
Range g_roots[32];
unsigned g_nroots = 0;
Item* g_ms = 0;                                          // the mark stack
Size g_msn = 0, g_mscap = 0;
bool g_markOverflow = false;
Size g_marked = 0;                                       // bytes marked so far in this collection
Size g_markedBy[4];                                      // ... reached from the caches, statics, other threads, this thread
// statistics for the log line, printed after the lock is released
unsigned g_count = 0;
bool g_pendingLog = false;
double g_lastMs = 0, g_totalMs = 0, g_stopMs = 0, g_markMs = 0, g_sweepMs = 0;
long long g_lastBefore = 0, g_lastAfter = 0;

#if defined(_MSC_VER)
__declspec(thread) Cache* t_cache = 0;
#else
__thread Cache* t_cache = 0;
#endif

// ---- small helpers ----
Size g_osBytes = 0, g_osPeak = 0;                       // what the heap holds from the OS, and the most it has held
void* osAlloc(Size n) {
	void* p = VirtualAlloc(0, n, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (p) { g_osBytes += (n + 4095) & ~(Size)4095; if (g_osBytes > g_osPeak) g_osPeak = g_osBytes; }
	return p;
}
void osFree(void* p) {
	if (!p) return;
	MEMORY_BASIC_INFORMATION mb;
	if (VirtualQuery(p, &mb, sizeof mb)) g_osBytes -= mb.RegionSize;
	VirtualFree(p, 0, MEM_RELEASE);
}
Size roundUp(Size n, Size a) { return (n + a - 1) & ~(a - 1); }

inline int lowestSet(Word x) {                           // x != 0
#if defined(_MSC_VER)
	unsigned long i;
#if defined(_WIN64)
	_BitScanForward64(&i, x);
#else
	_BitScanForward(&i, x);
#endif
	return (int)i;
#else
	return __builtin_ctzll((unsigned long long)x);
#endif
}
inline unsigned popCount(Word w) {
	U64 x = (U64)w;
	x = x - ((x >> 1) & 0x5555555555555555ULL);
	x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
	x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
	return (unsigned)((x * 0x0101010101010101ULL) >> 56);
}
inline Word atomicAnd(volatile Word* p, Word m) {        // the old value
#if defined(_MSC_VER) && defined(_WIN64)
	return (Word)_InterlockedAnd64((volatile __int64*)p, (__int64)m);
#elif defined(_MSC_VER)
	return (Word)_InterlockedAnd((volatile long*)p, (long)m);
#else
	return __atomic_fetch_and(p, m, __ATOMIC_ACQ_REL);
#endif
}
inline Word atomicOr(volatile Word* p, Word m) {
#if defined(_MSC_VER) && defined(_WIN64)
	return (Word)_InterlockedOr64((volatile __int64*)p, (__int64)m);
#elif defined(_MSC_VER)
	return (Word)_InterlockedOr((volatile long*)p, (long)m);
#else
	return __atomic_fetch_or(p, m, __ATOMIC_ACQ_REL);
#endif
}

Size sizeOf(int c) {
	if (c < 8) return (Size)(c + 1) * 16;
	const int b = 7 + (c - 8) / 4, k = (c - 8) % 4 + 1;
	return ((Size)1 << b) + (Size)k * ((Size)1 << (b - 2));
}
inline int sizeClassOf(Size n) {                         // 1 <= n <= kMaxSmall
	if (n <= 128) return (int)((n - 1) >> 4);
	const Size m = n - 1;
#if defined(_MSC_VER)
	unsigned long hb;
	_BitScanReverse(&hb, (unsigned long)m);              // (m < 2^17)
	const int b = (int)hb;                               // 2^b <= m < 2^(b+1)
#else
	const int b = 31 - __builtin_clz((unsigned)m);
#endif
	return 8 + (b - 7) * 4 + (int)((m - ((Size)1 << b)) >> (b - 2));
}

// ---- the page map ----
inline Seg* segOf(Word a) {
	if (a < g_lo || a >= g_hi) return 0;
	Seg** l2 = g_l1[(Size)((U64)a >> 32)];
	return l2 ? l2[(a >> 16) & (kL2 - 1)] : 0;
}
bool mapSet(char* base, Size bytes, Seg* to) {
	for (Word a = (Word)base; a < (Word)base + bytes; a += kPage) {
		Seg**& l2 = g_l1[(Size)((U64)a >> 32)];
		if (!l2) {
			if (!to) continue;
			l2 = (Seg**)osAlloc(kL2 * sizeof(Seg*));
			if (!l2) return false;
		}
		l2[(a >> 16) & (kL2 - 1)] = to;
	}
	if (to) {
		if ((Word)base < g_lo) g_lo = (Word)base;
		if ((Word)base + bytes > g_hi) g_hi = (Word)base + bytes;
	}
	return true;
}

inline unsigned blockIndex(const Seg* s, Size off) {
	U64 q = ((U64)off * s->recip) >> 32;
	if (q * s->bsize > off) --q;
	return (unsigned)q;
}

// ---- segments ----
Seg* newSeg(int c) {
	Cls& k = g_cls[c];
	const Size bs = k.size;
	Size bytes = roundUp(bs * 32 + 4096, kPage);
	if (bytes < ((Size)256 << 10)) bytes = (Size)256 << 10;
	unsigned nb = (unsigned)((bytes - sizeof(Seg)) / bs);
	Size hdr;
	for (;;) {                                           // the header and bitmaps, then as many blocks as fit
		const unsigned nw = (nb + kWordBits - 1) / kWordBits;
		hdr = roundUp(sizeof(Seg) + 2 * nw * sizeof(Word), 64);
		if (hdr + (Size)nb * bs <= bytes) break;
		--nb;
	}
	if (k.n == k.cap) {
		const unsigned cap = k.cap ? k.cap * 2 : 64;
		Seg** a = (Seg**)osAlloc(cap * sizeof(Seg*));
		if (!a) return 0;
		if (k.segs) { std::memcpy(a, k.segs, k.n * sizeof(Seg*)); osFree(k.segs); }
		k.segs = a;
		k.cap = cap;
	}
	char* base = (char*)osAlloc(bytes);
	if (!base) return 0;
	Seg* s = (Seg*)base;
	s->bytes = bytes;
	s->bsize = bs;
	s->nblocks = nb;
	s->nwords = (nb + kWordBits - 1) / kWordBits;
	s->recip = ((U64)1 << 32) / bs + 1;
	s->cls = c;
	s->atomic = k.atomic;
	s->alloc = (Word*)(base + sizeof(Seg));
	s->mark = s->alloc + s->nwords;
	s->blocks = base + hdr;
	s->next = s->prev = 0;
	const unsigned rem = nb % kWordBits;
	s->tail = rem ? ~(((Word)1 << rem) - 1) : 0;
	s->alloc[s->nwords - 1] = s->tail;                   // (VirtualAlloc'd memory is zero: the rest is clear)
	if (!mapSet(base, bytes, s)) { mapSet(base, bytes, 0); osFree(base); return 0; }
	s->index = k.n;
	k.segs[k.n++] = s;
	return s;
}

// Claim free blocks of class c for a thread's cache: some clear bits of one bitmap word. The lock is held.
bool claim(int c, Local& L) {
	Cls& k = g_cls[c];
	const unsigned want = (unsigned)(k.size >= kClaimBytes ? 1 : kClaimBytes / k.size);
	bool rescanned = false;
	for (;;) {
		while (k.cur < k.n) {
			Seg* s = k.segs[k.cur];
			for (unsigned w = k.curWord; w < s->nwords; ++w) {
				const Word a = s->alloc[w];
				if (a == ~(Word)0) continue;
				Word take = ~a;
				if (popCount(take) > want) {                  // the lowest `want` clear bits
					Word t = 0;
					for (unsigned i = 0; i < want; ++i) { const Word low = take & (0 - take); t |= low; take &= take - 1; }
					take = t;
				}
				atomicOr(&s->alloc[w], take);                 // (a free only clears bits: all of `take` is still clear)
				k.curWord = w;
				L.mask = take;
				L.base = s->blocks + (Size)w * kWordBits * s->bsize;
				L.seg = s;
				L.word = w;
				g_live += (long long)(popCount(take) * s->bsize);
				return true;
			}
			++k.cur;
			k.curWord = 0;
		}
		if (k.freed && !rescanned) {                      // blocks freed behind the cursor: reuse them before growing
			k.freed = 0;
			rescanned = true;
			k.cur = 0;
			k.curWord = 0;
			continue;
		}
		if (!newSeg(c)) return false;
	}
}

// Give a cache's unused blocks back (a thread ends). The lock is held.
void returnCache(Cache* c) {
	for (int i = 0; i < kClasses; ++i) {
		Local& L = c->local[i];
		if (!L.mask) continue;
		atomicAnd(&L.seg->alloc[L.word], ~L.mask);
		g_live -= (long long)(popCount(L.mask) * L.seg->bsize);
		g_cls[i].freed = 1;
		L.mask = 0;
	}
	g_live -= c->delta;
	c->delta = 0;
}

// ---- large blocks ----
void releaseRegion(Seg* s) {
	mapSet((char*)s, roundUp(s->bytes, kPage), 0);
	osFree(s);
}

void linkLarge(Seg* s) {
	s->prev = 0;
	s->next = g_large;
	if (g_large) g_large->prev = s;
	g_large = s;
	g_live += (long long)s->bytes;
}

// A large block: a region of its own; `align` (a power of two up to 64 KB) for aligned new. A region freed lately is reused
// when it fits (a buffer the program makes and drops every frame costs no VirtualAlloc, as the C heap's would not).
void* allocLarge(Size n, bool atomic, Size align) {
	if (align < 64) align = 64;
	const Size hdr = roundUp(sizeof(Seg), align);
	const Size bytes = roundUp(hdr + n, 4096);
	if (bytes < n) return 0;                             // overflow
	for (Seg* f = g_largeFree; f; f = f->next) {
		if (f->bytes < bytes || f->bytes - bytes > bytes / 4 + kPage) continue;
		if (f->prev) f->prev->next = f->next; else g_largeFree = f->next;
		if (f->next) f->next->prev = f->prev;
		g_largeFreeBytes -= f->bytes;
		f->bsize = roundUp(n, sizeof(Word));
		f->blocks = (char*)f + hdr;
		f->atomic = atomic;
		f->mark[0] = 0;
		if (!atomic) std::memset(f->blocks, 0, f->bsize);
		f->alloc[0] = 1;
		linkLarge(f);
		return f->blocks;
	}
	char* base = (char*)osAlloc(bytes);
	if (!base) return 0;
	Seg* s = (Seg*)base;
	s->bytes = bytes;
	s->bsize = roundUp(n, sizeof(Word));
	s->nblocks = 1;
	s->nwords = 1;
	s->recip = 0;
	s->cls = -1;
	s->index = 0;
	s->atomic = atomic;
	s->alloc = &s->largeBits[0];
	s->mark = &s->largeBits[1];
	s->tail = 0;
	s->alloc[0] = 1;
	s->mark[0] = 0;
	s->blocks = base + hdr;
	if (!mapSet(base, roundUp(bytes, kPage), s)) { mapSet(base, roundUp(bytes, kPage), 0); osFree(base); return 0; }
	linkLarge(s);
	return s->blocks;                                    // (zero: VirtualAlloc's pages are)
}

void freeLarge(Seg* s) {                                 // the lock is held
	if (s->prev) s->prev->next = s->next; else g_large = s->next;
	if (s->next) s->next->prev = s->prev;
	g_live -= (long long)s->bytes;
	s->alloc[0] = 0;                                     // no longer a block
	if (s->bytes > kLargeKeep / 4) { releaseRegion(s); return; }   // (a big one is not worth keeping)
	s->prev = 0;
	s->next = g_largeFree;
	if (g_largeFree) g_largeFree->prev = s;
	g_largeFree = s;
	g_largeFreeBytes += s->bytes;
	while (g_largeFreeBytes > kLargeKeep) {              // drop the oldest
		Seg* last = g_largeFree;
		while (last->next) last = last->next;
		if (last->prev) last->prev->next = 0; else g_largeFree = 0;
		g_largeFreeBytes -= last->bytes;
		releaseRegion(last);
	}
}

// ---- marking ----
bool pushItem(char* p, Size n) {
	if (g_msn == g_mscap) {
		const Size cap = g_mscap ? g_mscap * 2 : (Size)1 << 16;
		Item* a = (Item*)osAlloc(cap * sizeof(Item));
		if (!a) return false;
		if (g_ms) { std::memcpy(a, g_ms, g_msn * sizeof(Item)); osFree(g_ms); }
		g_ms = a;
		g_mscap = cap;
	}
	g_ms[g_msn].p = p;
	g_ms[g_msn].n = n;
	++g_msn;
	return true;
}

inline void markWord(Word v) {
	if (v < g_lo || v >= g_hi) return;
	Seg* s = segOf(v);
	if (!s || v < (Word)s->blocks) return;
	const Size off = (Size)(v - (Word)s->blocks);
	unsigned i;
	if (s->cls < 0) {
		if (off >= s->bsize) return;
		i = 0;
	} else {
		i = blockIndex(s, off);
		if (i >= s->nblocks) return;
	}
	const unsigned w = i / kWordBits;
	const Word m = (Word)1 << (i % kWordBits);
	if (!(s->alloc[w] & m) || (s->mark[w] & m)) return;
	s->mark[w] |= m;
	g_marked += s->bsize;
	if (!s->atomic && !pushItem(s->blocks + (Size)i * s->bsize, s->bsize)) g_markOverflow = true;
}

void scanRange(Word lo, Word hi) {
	lo = (lo + sizeof(Word) - 1) & ~(Word)(sizeof(Word) - 1);
	for (const Word* p = (const Word*)lo; (Word)(p + 1) <= hi; ++p) markWord(*p);
}

void drain() {
	while (g_msn) {
		const Item it = g_ms[--g_msn];
		scanRange((Word)it.p, (Word)it.p + it.n);
	}
}

// Without room to grow the mark stack: rescan every marked, scannable block until nothing new is marked.
void recoverOverflow() {
	while (g_markOverflow) {
		g_markOverflow = false;
		for (int c = 0; c < kClasses; ++c) {
			if (g_cls[c].atomic) continue;
			for (unsigned j = 0; j < g_cls[c].n; ++j) {
				Seg* s = g_cls[c].segs[j];
				for (unsigned i = 0; i < s->nblocks; ++i)
					if (s->mark[i / kWordBits] & ((Word)1 << (i % kWordBits))) {
						const Word b = (Word)(s->blocks + (Size)i * s->bsize);
						scanRange(b, b + s->bsize);
						drain();
					}
			}
		}
		for (Seg* s = g_large; s; s = s->next)
			if ((s->mark[0] & 1) && !s->atomic) { scanRange((Word)s->blocks, (Word)s->blocks + s->bsize); drain(); }
	}
}

// ---- threads ----
Word stackBase() { return (Word)((NT_TIB*)NtCurrentTeb())->StackBase; }

// A thread that has ended (its handle is signalled) is detached at the next collection: its cache goes back, its entry is
// reused. Not earlier: a thread runs destructors, and may allocate, after any exit callback it could be given.
void detachEnded() {                                     // the lock is held
	for (unsigned i = 0; i < g_nthr; ++i) {
		Thr& t = g_thr[i];
		if (!t.live || WaitForSingleObject(t.h, 0) != WAIT_OBJECT_0) continue;
		t.live = false;
		CloseHandle(t.h);
		t.h = 0;
		if (t.cache) { returnCache(t.cache); osFree(t.cache); t.cache = 0; }
	}
}

Cache* attachLocked() {                                  // the lock is held
	if (t_cache) return t_cache;
	unsigned i = 0;
	while (i < g_nthr && g_thr[i].live) ++i;
	if (i == g_nthr) {
		if (g_nthr == g_capthr) {
			const unsigned cap = g_capthr ? g_capthr * 2 : 64;
			Thr* a = (Thr*)osAlloc(cap * sizeof(Thr));
			if (!a) return 0;
			if (g_thr) { std::memcpy(a, g_thr, g_nthr * sizeof(Thr)); osFree(g_thr); }
			g_thr = a;
			g_capthr = cap;
		}
		++g_nthr;
	}
	HANDLE h = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION | SYNCHRONIZE, FALSE, GetCurrentThreadId());
	Cache* c = (Cache*)osAlloc(sizeof(Cache));
	if (!h || !c) { if (h) CloseHandle(h); osFree(c); return 0; }
	g_thr[i].id = GetCurrentThreadId();
	g_thr[i].h = h;
	g_thr[i].base = stackBase();
	g_thr[i].cache = c;
	g_thr[i].live = true;
	g_thr[i].suspended = false;
	t_cache = c;
	return c;
}

void stopWorld() {
	const DWORD me = GetCurrentThreadId();
	for (unsigned i = 0; i < g_nthr; ++i) {
		Thr& t = g_thr[i];
		t.suspended = false;
		if (!t.live || t.id == me) continue;
		if (SuspendThread(t.h) == (DWORD)-1) continue;   // (it has ended: the next collection detaches it)
		t.suspended = true;
	}
}

void scanThreads() {
	const DWORD me = GetCurrentThreadId();
	for (unsigned i = 0; i < g_nthr; ++i) {
		Thr& t = g_thr[i];
		if (!t.live || t.id == me || !t.suspended) continue;
		CONTEXT ctx;
		std::memset(&ctx, 0, sizeof ctx);
#if defined(_WIN64)
		ctx.ContextFlags = CONTEXT_FULL;                 // the integer, control and XMM registers
#else
		ctx.ContextFlags = CONTEXT_FULL | CONTEXT_EXTENDED_REGISTERS;
#endif
		if (!GetThreadContext(t.h, &ctx)) continue;      // (waits for the suspension to take effect)
		DWORD code = 0;
		if (!GetExitCodeThread(t.h, &code) || code != STILL_ACTIVE) continue;
		scanRange((Word)&ctx, (Word)(&ctx + 1));          // its registers
#if defined(_WIN64)
		const Word sp = (Word)ctx.Rsp;
#else
		const Word sp = (Word)ctx.Esp;
#endif
		MEMORY_BASIC_INFORMATION mb;                     // a stack that is really there (committed)
		if (sp >= t.base || t.base - sp >= ((Word)1 << 30) || !VirtualQuery((void*)sp, &mb, sizeof mb) || mb.State != MEM_COMMIT) continue;
		scanRange(sp, t.base);
	}
}

void startWorld() {
	for (unsigned i = 0; i < g_nthr; ++i)
		if (g_thr[i].suspended) { ResumeThread(g_thr[i].h); g_thr[i].suspended = false; }
}

J2XGC_NOINLINE void scanSelf() {
	// The callee-saved registers, onto this frame. (RtlCaptureContext on x86 follows EBP to its caller's frame, and an
	// optimized x86 build has no frame pointer: it read through whatever EBP held. setjmp saves exactly those registers.)
#if defined(_WIN64)
	CONTEXT regs;
	RtlCaptureContext(&regs);
#else
	jmp_buf regs;
	setjmp(regs);
#endif
	scanRange((Word)&regs, (Word)(&regs + 1));
	volatile Word here = 0;
	const Word lo = (Word)&here < (Word)&regs ? (Word)&here : (Word)&regs;
	scanRange(lo, stackBase());
}

// The blocks in the threads' caches stay claimed: marked, never scanned (nothing is in them yet). Their pending frees count.
void markCaches() {
	for (unsigned i = 0; i < g_nthr; ++i) {
		Cache* c = g_thr[i].live ? g_thr[i].cache : 0;
		if (!c) continue;
		for (int k = 0; k < kClasses; ++k) {
			Local& L = c->local[k];
			if (L.mask) L.seg->mark[L.word] |= L.mask;
		}
		c->delta = 0;                                    // (the sweep counts what is live)
	}
}

// ---- sweeping ----
// What is not marked is free. The next collection comes when the live bytes have grown by half (by g_minThreshold at least);
// empty segments are kept up to what the program will fill again by then, the rest go back to the OS.
void sweep() {
	long long live = 0;
	for (int c = 0; c < kClasses; ++c)
		for (unsigned j = 0; j < g_cls[c].n; ++j) {
			Seg* s = g_cls[c].segs[j];
			unsigned used = 0;
			for (unsigned w = 0; w < s->nwords; ++w) {
				Word a = s->alloc[w] & s->mark[w];
				s->mark[w] = 0;
				if (w == s->nwords - 1) a |= s->tail;
				s->alloc[w] = a;
				used += popCount(a);
			}
			s->used = used - popCount(s->tail);
			live += (long long)s->used * (long long)s->bsize;
		}
	for (Seg* s = g_large; s;) {
		Seg* next = s->next;
		if (s->mark[0] & 1) { s->mark[0] = 0; live += (long long)s->bytes; } else freeLarge(s);
		s = next;
	}
	g_live = live;
	const Size half = (Size)live / 2;
	const Size next = (Size)live + (half > g_minThreshold ? half : g_minThreshold);
	g_threshold = next;
	long long keep = (long long)next - live;
	for (int c = 0; c < kClasses; ++c) {
		Cls& k = g_cls[c];
		unsigned out = 0;
		for (unsigned j = 0; j < k.n; ++j) {
			Seg* s = k.segs[j];
			if (s->used == 0) {
				if (keep < (long long)s->bytes) { mapSet((char*)s, s->bytes, 0); osFree(s); continue; }
				keep -= (long long)s->bytes;
			}
			s->index = out;
			k.segs[out++] = s;
		}
		k.n = out;
		k.cur = 0;
		k.curWord = 0;
		k.freed = 0;
	}
}

double msSince(const LARGE_INTEGER& t0, const LARGE_INTEGER& f) {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return (double)(t.QuadPart - t0.QuadPart) * 1000.0 / (double)f.QuadPart;
}

void heapStats();

void collectLocked() {
	if (g_collecting || g_starting > 0) return;
	g_collecting = true;
	LARGE_INTEGER f, t0;
	QueryPerformanceFrequency(&f);
	QueryPerformanceCounter(&t0);
	const long long before = g_live;
	detachEnded();
	stopWorld();
	g_stopMs = msSince(t0, f);
	g_marked = 0;
	markCaches();
	g_markedBy[0] = g_marked;
	for (unsigned r = 0; r < g_nroots; ++r) { scanRange(g_roots[r].lo, g_roots[r].hi); drain(); }
	g_markedBy[1] = g_marked - g_markedBy[0];
	scanThreads();
	drain();
	g_markedBy[2] = g_marked - g_markedBy[1] - g_markedBy[0];
	scanSelf();
	drain();
	recoverOverflow();
	g_markedBy[3] = g_marked - g_markedBy[2] - g_markedBy[1] - g_markedBy[0];
	startWorld();
	g_markMs = msSince(t0, f) - g_stopMs;
	sweep();
	g_lastMs = msSince(t0, f);
	g_sweepMs = g_lastMs - g_markMs - g_stopMs;
	g_totalMs += g_lastMs;
	g_lastBefore = before;
	g_lastAfter = g_live;
	++g_count;
	g_pendingLog = g_log;
	if (g_log) heapStats();
	g_collecting = false;
}

Size g_segBytes = 0, g_largeBytes = 0, g_cachedBytes = 0;
unsigned g_nsegs = 0, g_nlarge = 0;
void heapStats() {                                       // what the heap holds from the OS (the lock is held)
	g_segBytes = g_largeBytes = g_cachedBytes = 0;
	g_nsegs = g_nlarge = 0;
	for (int c = 0; c < kClasses; ++c)
		for (unsigned j = 0; j < g_cls[c].n; ++j) { g_segBytes += g_cls[c].segs[j]->bytes; ++g_nsegs; }
	for (Seg* s = g_large; s; s = s->next) { g_largeBytes += s->bytes; ++g_nlarge; }
	for (Seg* s = g_largeFree; s; s = s->next) g_cachedBytes += s->bytes;
}

// J2X_GC_LOG=2: where the process's private memory is, the heap's and the rest (the five largest other allocations, a thread's
// stack marked), and the process's threads.
bool g_logMap = false;
void printMap() {
	Size total = 0, other = 0;
	struct Big { Word base; Size n; DWORD type; } big[5] = {};
	DWORD curGuard = 0;
	Word a = 0, curBase = 0;
	Size curN = 0;
	MEMORY_BASIC_INFORMATION mb;
	for (;;) {
		if (!VirtualQuery((void*)a, &mb, sizeof mb)) break;
		const Word base = (Word)mb.AllocationBase;
		if (base != curBase) {
			if (curN) {
				int k = 0;
				for (int i = 1; i < 5; ++i) if (big[i].n < big[k].n) k = i;
				if (curN > big[k].n) { big[k].base = curBase; big[k].n = curN; big[k].type = curGuard; }
			}
			curBase = base;
			curN = 0;
			curGuard = 0;
		}
		if (mb.State == MEM_COMMIT && (mb.Protect & PAGE_GUARD)) curGuard = 1;
		if (mb.State == MEM_COMMIT && mb.Type == MEM_PRIVATE) {
			total += mb.RegionSize;
			if (!segOf((Word)mb.BaseAddress)) { other += mb.RegionSize; curN += mb.RegionSize; }
		}
		const Word next = (Word)mb.BaseAddress + mb.RegionSize;
		if (next <= a) break;
		a = next;
	}
	std::fprintf(stderr, "[gc]   private %d MB: the heap %d MB, other %d MB; largest other:", (int)(total >> 20), (int)((total - other) >> 20),
	             (int)(other >> 20));
	for (int i = 0; i < 5; ++i) if (big[i].n) std::fprintf(stderr, " %p %d MB%s", (void*)big[i].base, (int)(big[i].n >> 20), big[i].type ? " (a stack)" : "");
	unsigned threads = 0;
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (snap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 te;
		te.dwSize = sizeof te;
		for (BOOL ok = Thread32First(snap, &te); ok; ok = Thread32Next(snap, &te)) if (te.th32OwnerProcessID == GetCurrentProcessId()) ++threads;
		CloseHandle(snap);
	}
	std::fprintf(stderr, "; %u threads (%u attached entries)\n", threads, g_nthr);
}

// J2X_GC_LOG=2 also watches the process: each new high of its private memory (by 32 MB) is printed with what the heap holds.
DWORD WINAPI watchMemory(void*) {
	Size high = 0;
	for (;;) {
		Sleep(100);
		PROCESS_MEMORY_COUNTERS_EX c;
		c.cb = sizeof c;
		if (!K32GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&c, sizeof c)) continue;
		if (c.PrivateUsage < high + ((Size)32 << 20)) continue;
		high = c.PrivateUsage;
		std::fprintf(stderr, "[gc] watch: private %d MB, the heap holds %d MB from the OS (live about %d MB, next collection at %d MB)\n",
		             (int)(high >> 20), (int)(g_osBytes >> 20), (int)(g_live >> 20), (int)(g_threshold >> 20));
		printMap();
	}
}

void printLog() {                                        // the lock is NOT held (stderr may allocate, may block)
	if (!g_pendingLog) return;
	g_pendingLog = false;
	std::fprintf(stderr, "[gc] #%u %.1f ms (stop %.1f, mark %.1f, sweep %.1f), live %d -> %d MB (from statics %d, other threads %d, "
	             "this thread %d), total pause %.0f ms\n", g_count, g_lastMs, g_stopMs, g_markMs, g_sweepMs, (int)(g_lastBefore >> 20),
	             (int)(g_lastAfter >> 20), (int)(g_markedBy[1] >> 20), (int)(g_markedBy[2] >> 20), (int)(g_markedBy[3] >> 20), g_totalMs);
	std::fprintf(stderr, "[gc]   heap: %u segments %d MB, %u large blocks %d MB, %d MB of freed large blocks kept; from the OS %d MB "
	             "(at most %d)\n", g_nsegs, (int)(g_segBytes >> 20), g_nlarge, (int)(g_largeBytes >> 20), (int)(g_cachedBytes >> 20),
	             (int)(g_osBytes >> 20), (int)(g_osPeak >> 20));
	if (g_logMap) printMap();
}

bool envIs(const char* name, char want) {
	char v[8];
	const DWORD n = GetEnvironmentVariableA(name, v, sizeof v);
	return n > 0 && n < sizeof v && v[0] == want;
}

void addImageRoots() {
	char* m = (char*)GetModuleHandleA(0);
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)m;
	IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(m + dos->e_lfanew);
	IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt);
	for (unsigned i = 0; i < nt->FileHeader.NumberOfSections && g_nroots < 32; ++i) {
		if (!(sec[i].Characteristics & IMAGE_SCN_MEM_WRITE)) continue;
		const Size n = sec[i].Misc.VirtualSize ? sec[i].Misc.VirtualSize : sec[i].SizeOfRawData;
		g_roots[g_nroots].lo = (Word)(m + sec[i].VirtualAddress);
		g_roots[g_nroots].hi = (Word)(m + sec[i].VirtualAddress + n);
		++g_nroots;
	}
}

void* hookAllocAtomic(std::size_t n);
void hookCollect();
void hookAttach();
void hookStarting(int d);

void init() {
	if (g_init == 2) return;
	if (InterlockedCompareExchange(&g_init, 1, 0) != 0) {
		while (g_init != 2) Sleep(0);
		return;
	}
	InitializeCriticalSectionAndSpinCount(&g_cs, 4000);
	for (int c = 0; c < kClasses; ++c) {
		g_cls[c].size = sizeOf(c % kSizes);
		g_cls[c].atomic = c >= kSizes;
	}
	g_l1 = (Seg***)osAlloc(kL1 * sizeof(Seg**));
	g_minThreshold = sizeof(void*) == 8 ? (Size)64 << 20 : (Size)32 << 20;
	g_threshold = g_minThreshold;
	g_enabled = !envIs("J2X_GC", '0');
	g_log = envIs("J2X_GC_LOG", '1') || envIs("J2X_GC_LOG", '2');
	g_logMap = envIs("J2X_GC_LOG", '2');
	if (g_logMap) CloseHandle(CreateThread(0, 0, watchMemory, 0, 0, 0));
	addImageRoots();
	EnterCriticalSection(&g_cs);
	attachLocked();                                      // the thread that runs the static initializers: the main thread
	LeaveCriticalSection(&g_cs);
	J2xHeapHooks& h = j2xHeapHooks();
	h.allocAtomic = hookAllocAtomic;
	h.collect = hookCollect;
	h.attachThread = hookAttach;
	h.threadStarting = hookStarting;
	g_init = 2;
}

// The slow path: large or aligned blocks, a thread's first allocation, an empty cache.
J2XGC_NOINLINE void* allocSlow(Size n, bool atomic, Size align) {
	init();
	void* p = 0;
	EnterCriticalSection(&g_cs);
	Cache* c = attachLocked();
	if (c && c->delta) { g_live -= c->delta; c->delta = 0; if (g_live < 0) g_live = 0; }
	if (g_enabled && g_live + (long long)n > (long long)g_threshold) collectLocked();
	for (int attempt = 0; attempt < 2 && !p; ++attempt) {
		if (attempt) { if (!g_enabled) break; collectLocked(); }   // out of memory: collect and try once more
		if (n > kMaxSmall || align > 16 || !c) {
			p = allocLarge(n, atomic, align);
		} else {
			const int k = sizeClassOf(n) + (atomic ? kSizes : 0);
			Local& L = c->local[k];
			if (L.mask || claim(k, L)) {
				const int bit = lowestSet(L.mask);
				L.mask &= L.mask - 1;
				p = L.base + (Size)bit * g_cls[k].size;
				if (!atomic) std::memset(p, 0, g_cls[k].size);
			}
		}
	}
	LeaveCriticalSection(&g_cs);
	if (g_pendingLog) printLog();
	return p;
}

inline void* alloc(Size n, bool atomic, Size align) {
	Cache* c = t_cache;
	if (c && n - 1 < kMaxSmall && align <= 16) {          // the fast path: a block from the thread's own cache, no lock
		const int k = sizeClassOf(n) + (atomic ? kSizes : 0);
		Local& L = c->local[k];
		if (L.mask) {
			const int bit = lowestSet(L.mask);
			L.mask &= L.mask - 1;
			const Size bs = g_cls[k].size;
			char* p = L.base + (Size)bit * bs;
			if (!atomic) std::memset(p, 0, bs);           // no stale pointers from the block's previous life
			return p;
		}
	}
	return allocSlow(n ? n : 1, atomic, align);
}

void release(void* p) {
	if (!p) return;
	Seg* s = segOf((Word)p);
	if (!s) { std::free(p); return; }                    // a pointer a DLL's operator new (malloc) made
	if (s->cls < 0) {
		EnterCriticalSection(&g_cs);
		if ((char*)p == s->blocks && (s->alloc[0] & 1)) freeLarge(s);
		LeaveCriticalSection(&g_cs);
		return;
	}
	if ((char*)p < s->blocks) return;
	const Size off = (Size)((char*)p - s->blocks);
	const unsigned i = blockIndex(s, off);
	if (i >= s->nblocks || (Size)i * s->bsize != off) return;   // not a block's start: ignore
	const Word m = (Word)1 << (i % kWordBits);
	if (!(atomicAnd(&s->alloc[i / kWordBits], ~m) & m)) return; // already free
	g_cls[s->cls].freed = 1;
	if (Cache* c = t_cache) c->delta += (long long)s->bsize;
}

void* hookAllocAtomic(std::size_t n) {
	void* p = alloc(n, true, 0);
	if (!p) throw std::bad_alloc();
	return p;
}
void hookCollect() {
	init();
	EnterCriticalSection(&g_cs);
	attachLocked();
	if (g_enabled) collectLocked();
	LeaveCriticalSection(&g_cs);
	printLog();
}
void hookAttach() {
	if (t_cache) return;
	init();
	EnterCriticalSection(&g_cs);
	attachLocked();
	LeaveCriticalSection(&g_cs);
}
void hookStarting(int d) {
	if (d > 0) InterlockedIncrement(&g_starting); else InterlockedDecrement(&g_starting);
}

inline void* allocOrThrow(Size n, Size align) {
	void* p = alloc(n, false, align);
	if (!p) throw std::bad_alloc();
	return p;
}

}  // namespace j2xgc

// ---- the replaced global operators ----
void* operator new(std::size_t n) { return j2xgc::allocOrThrow(n, 0); }
void* operator new[](std::size_t n) { return j2xgc::allocOrThrow(n, 0); }
void* operator new(std::size_t n, const std::nothrow_t&) noexcept { return j2xgc::alloc(n, false, 0); }
void* operator new[](std::size_t n, const std::nothrow_t&) noexcept { return j2xgc::alloc(n, false, 0); }
void operator delete(void* p) noexcept { j2xgc::release(p); }
void operator delete[](void* p) noexcept { j2xgc::release(p); }
void operator delete(void* p, const std::nothrow_t&) noexcept { j2xgc::release(p); }
void operator delete[](void* p, const std::nothrow_t&) noexcept { j2xgc::release(p); }
void operator delete(void* p, std::size_t) noexcept { j2xgc::release(p); }
void operator delete[](void* p, std::size_t) noexcept { j2xgc::release(p); }
#if defined(__cpp_aligned_new)
void* operator new(std::size_t n, std::align_val_t a) { return j2xgc::allocOrThrow(n, (std::size_t)a); }
void* operator new[](std::size_t n, std::align_val_t a) { return j2xgc::allocOrThrow(n, (std::size_t)a); }
void* operator new(std::size_t n, std::align_val_t a, const std::nothrow_t&) noexcept { return j2xgc::alloc(n, false, (std::size_t)a); }
void* operator new[](std::size_t n, std::align_val_t a, const std::nothrow_t&) noexcept { return j2xgc::alloc(n, false, (std::size_t)a); }
void operator delete(void* p, std::align_val_t) noexcept { j2xgc::release(p); }
void operator delete[](void* p, std::align_val_t) noexcept { j2xgc::release(p); }
void operator delete(void* p, std::size_t, std::align_val_t) noexcept { j2xgc::release(p); }
void operator delete[](void* p, std::size_t, std::align_val_t) noexcept { j2xgc::release(p); }
void operator delete(void* p, std::align_val_t, const std::nothrow_t&) noexcept { j2xgc::release(p); }
void operator delete[](void* p, std::align_val_t, const std::nothrow_t&) noexcept { j2xgc::release(p); }
#endif

#endif // _WIN32 && !_XBOX
