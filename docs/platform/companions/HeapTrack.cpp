// HeapTrack.cpp -- global heap-leak watchdog (PLATFORM COMPANION). See HeapTrack.h.
//
// Overrides global operator new/delete so EVERY C++ allocation is tagged with a 16-byte header and
// attributed to a CALL-STACK. Attribution uses a CONSERVATIVE STACK SCAN (pure memory reads -- NO kernel
// call like RtlCaptureStackBackTrace, which the emulator doesn't implement): it scans this thread's own
// stack for words that land in the title code range and treats them as the return-address chain. This
// blames the GAME function that grew a std::vector<JObject>/std::map node, not just the std-lib internals.
// The scan stops at the next 64KB boundary (>= the 64KB-aligned stack base) so it can never over-read.
// A monitor thread reports every 2.5s: live bytes, leak RATE, and the top callstacks. The tracker uses
// malloc/free + static tables + Interlocked ONLY (never new) -> no recursion. Map each printed 0x........
// frame via MinecraftBeta.map; skip the first 1-2 (operator new / _Allocate) to find the game caller.
#include "stdafx.h"
#include "HeapTrack.h"
#include <cstdlib>
#include <new>

// TOGGLE: heap-leak watchdog. Default ON. Override to 0 from the build (add J2X_HEAPTRACK=0 to the
// vcxproj PreprocessorDefinitions) or edit here. Reports via DbgPrint (cheap kernel channel, NO
// filesystem I/O) -- independent of J2X_DEBUG_LOG. 0 compiles it out entirely (no new/delete override).
#ifndef J2X_HEAPTRACK
#define J2X_HEAPTRACK 1
#endif

#ifndef HT_CODE_LO
// TIGHT title CODE bounds from MinecraftBeta.map (.text ends ~0x82587020). Keeping HI just past the
// last function EXCLUDES the RTTI/static-field data that lives right above code (0x82590000, 0x8266xxxx)
// -- those were the noise words the scan mistook for return addresses and burned backtrace slots on.
#define HT_CODE_LO 0x82110000u
#define HT_CODE_HI 0x82588000u
#endif

// Allocations smaller than this skip the stack scan (they get a shared "small" bucket) -- the tiny
// per-tick allocs are the most frequent, and scanning on every one is what dropped us to 8 fps. The
// leaks that matter (JList/JMap buffers, chunk arrays, vertex buffers) are all far larger than this.
#define HT_MINTRACE 64u
#define HT_SCAN_WORDS 2048          // stack words to scan (8KB) -- deep enough for the full chain, cheap

// SKIP the std/runtime template cluster (basic_string, JObject, JList<JObject> push_back/reserve/allocate,
// ostringstream, locale, JCat/StringBuilder). Confirmed via the map: 0x82110000..0x82116000 is ALL machinery,
// no game code. Skipping it means the captured frames are the GAME callers, not the container internals we
// already know from the type. (The alloc's type is still obvious from the site's byte size.)
#define HT_SKIP_LO 0x82110000u
#define HT_SKIP_HI 0x82116000u

#if J2X_HEAPTRACK

extern "C" unsigned long __cdecl DbgPrint(const char* Format, ...);

namespace {
	const unsigned HT_MAGIC = 0x48545241u;                 // 'HTRA'
	enum { HT_DEPTH = 10 };                                 // return addresses captured per allocation (deep
	                                                        // enough that game callers survive past the alloc
	                                                        // machinery; the resolver filters std/CRT frames)

	struct Site { void* volatile bt[HT_DEPTH]; volatile LONGLONG bytes, count, total; };
	struct Hdr  { size_t size; Site* site; unsigned magic; unsigned pad; };
	const size_t HT_HDR = (sizeof(Hdr) + 15) & ~(size_t)15;

	volatile LONGLONG g_liveBytes = 0, g_liveCount = 0, g_peakBytes = 0, g_totAlloc = 0, g_totFree = 0;
	volatile LONG g_started = 0;

	enum { NSITES = 16384 };
	Site g_sites[NSITES];

	void htCapture(void** bt) {
		for (int i = 0; i < HT_DEPTH; ++i) bt[i] = 0;
#if defined(HT_HOST_TEST)
		bt[0] = (void*)0x1000; bt[1] = (void*)0x1004;      // deterministic fake site for the host counting test
#else
		volatile int probe = 0;
		unsigned int* p = (unsigned int*)(((size_t)&probe) & ~(size_t)3);
		// safe upper bound: next 64KB boundary (stacks are 64KB-aligned, so the base is at a 64KB boundary
		// -> never read past mapped stack), capped so a very shallow frame doesn't over-scan.
		unsigned int* stop = (unsigned int*)((((size_t)p) + 0x10000) & ~(size_t)0xFFFF);
		if ((size_t)(stop - p) > HT_SCAN_WORDS) stop = p + HT_SCAN_WORDS;
		int found = 0; unsigned int last = 0;
		for (; p < stop && found < HT_DEPTH; ++p) {
			unsigned int v = *p;
			if (v < HT_CODE_LO || v >= HT_CODE_HI) continue;      // not a title-code return address
			if (v >= HT_SKIP_LO && v < HT_SKIP_HI) continue;      // skip the std/JList/JObject machinery cluster
			if (v == last) continue;
			bt[found++] = (void*)(size_t)v; last = v;
		}
#endif
	}

	Site* siteFor(void** bt) {
		unsigned h = 0;
		for (int k = 0; k < HT_DEPTH; ++k) h = h * 131 + (unsigned)((size_t)bt[k] >> 2);
		for (int i = 0; i < NSITES; ++i) {
			Site& s = g_sites[(h + i) & (NSITES - 1)];
			int m = 1; for (int k = 0; k < HT_DEPTH; ++k) if (s.bt[k] != bt[k]) { m = 0; break; }
			if (m) return &s;
			if (s.bt[0] == NULL) {
				if (InterlockedCompareExchangePointer(&s.bt[0], bt[0], NULL) == NULL) {
					for (int k = 1; k < HT_DEPTH; ++k) s.bt[k] = bt[k];
					return &s;
				}
			}
		}
		return &g_sites[h & (NSITES - 1)];
	}

	void onAlloc(size_t size, Site* s) {
		LONGLONG live = InterlockedExchangeAdd64(&g_liveBytes, (LONGLONG)size) + (LONGLONG)size;
		InterlockedIncrement64(&g_liveCount); InterlockedIncrement64(&g_totAlloc);
		if (live > g_peakBytes) g_peakBytes = live;
		InterlockedExchangeAdd64(&s->bytes, (LONGLONG)size);
		InterlockedIncrement64(&s->count); InterlockedIncrement64(&s->total);
	}
	void onFree(size_t size, Site* s) {
		InterlockedExchangeAdd64(&g_liveBytes, -(LONGLONG)size);
		InterlockedDecrement64(&g_liveCount); InterlockedIncrement64(&g_totFree);
		if (s) { InterlockedExchangeAdd64(&s->bytes, -(LONGLONG)size); InterlockedDecrement64(&s->count); }
	}

	void* htAlloc(size_t n) {
		void* base = malloc(n + HT_HDR);
		if (!base) return 0;
		void* bt[HT_DEPTH];
		if (n >= HT_MINTRACE) {
			htCapture(bt);
		} else {
			// Small allocs are the most frequent; scanning every one tanks fps. But a fast small-object LEAK
			// still needs attribution, so SAMPLE 1/64 with a real backtrace (a 32k/sec leak lands in the
			// sample within a frame) and send the other 63/64 to the cheap shared bucket.
			static volatile LONG smallCtr = 0;
			if ((InterlockedIncrement(&smallCtr) & 63) == 0) {
				htCapture(bt);
			} else {
				for (int i = 0; i < HT_DEPTH; ++i) bt[i] = 0;
				bt[0] = (void*)(size_t)0x82110000u;   // shared "small allocs" bucket -- counted, not scanned
			}
		}
		Site* s = siteFor(bt);
		Hdr* h = (Hdr*)base; h->size = n; h->site = s; h->magic = HT_MAGIC; h->pad = 0;
		onAlloc(n, s);
		return (char*)base + HT_HDR;
	}
	void htFree(void* p) {
		if (!p) return;
		Hdr* h = (Hdr*)((char*)p - HT_HDR);
		if (h->magic != HT_MAGIC) { free(p); return; }
		onFree(h->size, h->site); h->magic = 0; free(h);
	}
}

void* operator new(size_t n)                                   { return htAlloc(n ? n : 1); }
void* operator new[](size_t n)                                 { return htAlloc(n ? n : 1); }
void* operator new(size_t n, const std::nothrow_t&) throw()    { return htAlloc(n ? n : 1); }
void* operator new[](size_t n, const std::nothrow_t&) throw()  { return htAlloc(n ? n : 1); }
void  operator delete(void* p) throw()                         { htFree(p); }
void  operator delete[](void* p) throw()                       { htFree(p); }
void  operator delete(void* p, const std::nothrow_t&) throw()  { htFree(p); }
void  operator delete[](void* p, const std::nothrow_t&) throw(){ htFree(p); }

namespace HeapTrack {

long long liveBytes() { return g_liveBytes; }
long long liveCount() { return g_liveCount; }
long long peakBytes() { return g_peakBytes; }

void report(const char* tag) {
	static volatile LONGLONG pTot = 0, pFre = 0, pLive = 0;
	LONGLONG live = g_liveBytes, cnt = g_liveCount, peak = g_peakBytes, tot = g_totAlloc, fre = g_totFree;
	DbgPrint("[HEAP] %s live=%ldK (%ld objs) peak=%ldK | interval: +%ld alloc -%ld free = %+ldK net\n",
	         tag ? tag : "", (long)(live/1024), (long)cnt, (long)(peak/1024),
	         (long)(tot - pTot), (long)(fre - pFre), (long)((live - pLive)/1024));
	pTot = tot; pFre = fre; pLive = live;

	enum { TOPN = 8 };
	Site* top[TOPN]; LONGLONG topB[TOPN]; int i;
	for (i = 0; i < TOPN; ++i) { top[i] = 0; topB[i] = 0; }
	for (i = 0; i < NSITES; ++i) {
		if (!g_sites[i].bt[0]) continue;
		LONGLONG b = g_sites[i].bytes;
		if (b <= topB[TOPN - 1]) continue;
		int j = TOPN - 1;
		while (j > 0 && b > topB[j - 1]) { top[j]=top[j-1]; topB[j]=topB[j-1]; --j; }
		top[j] = &g_sites[i]; topB[j] = b;
	}
	for (i = 0; i < TOPN; ++i) {
		if (!top[i] || topB[i] <= 0) continue;
		Site* s = top[i];
		char line[256]; int n = 0;
		n += _snprintf(line + n, sizeof(line) - n, "[HEAP]  %ldK %ld objs <-", (long)(topB[i]/1024), (long)s->count);
		for (int k = 0; k < HT_DEPTH && n < (int)sizeof(line) - 12; ++k)
			n += _snprintf(line + n, sizeof(line) - n, " %08X", (unsigned)(size_t)s->bt[k]);
		DbgPrint("%s\n", line);
	}
}

static DWORD WINAPI _monitor(LPVOID) { for (;;) { Sleep(2500); report("watch"); } }

void start() {
	if (InterlockedExchange(&g_started, 1) != 0) return;
	HANDLE t = CreateThread(NULL, 0, _monitor, NULL, 0, NULL);
	if (t) CloseHandle(t);
	DbgPrint("[HEAP] watchdog started (stack-scan backtrace; map the '<- ...' addrs via MinecraftBeta.map)\n");
}

} // namespace HeapTrack

#else
namespace HeapTrack {
	void start() {} void report(const char*) {}
	long long liveBytes() { return 0; } long long liveCount() { return 0; } long long peakBytes() { return 0; }
}
#endif
