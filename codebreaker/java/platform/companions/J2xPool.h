// J2xPool.h -- size-bucketed slab pool for high-churn small objects (PLATFORM COMPANION, header-only).
//
// WHY: the no-GC 360 port allocates/frees huge numbers of tiny objects per second (scheduled ticks =
// NextTickListEntry + their two std::set nodes; ~120k live, churning). On the Xbox 360 heap (BaseHeap,
// size-class pages) that churn shreds the small-size-class pages into fragments, so eventually a modest
// CONTIGUOUS allocation -- e.g. a chunk/region SAVE buffer -- can't be placed and the alloc returns NULL
// -> NULL-deref FREEZE (observed: game hangs the instant it autosaves). Routing those objects through a
// pool keeps them in big contiguous slabs with per-size free-lists: reused, never returned to the general
// heap, so they neither churn nor fragment it.
//
// USE:
//   * STL container nodes (std::set/std::map):  std::set<T, Cmp, J2xPoolAlloc<T> >   (dealloc knows the size)
//   * a hot heap class:                         put  J2X_POOLED_NEW_DELETE(ClassName)  in its body
// Thread-safe (a CRITICAL_SECTION); first use is single-threaded (world create) so the lazy init is safe.
#ifndef J2X_POOL_H
#define J2X_POOL_H
#if defined(_XBOX)
#include <xtl.h>        // Xbox 360 XDK: CRITICAL_SECTION / Interlocked* / Sleep (there is no <windows.h>)
#else
#include <windows.h>    // host / PC build
#endif
#include <cstdlib>

namespace J2xPool {
	enum { MAXSZ = 160, ALIGN = 8, NBUCKETS = (MAXSZ / ALIGN) + 1, SLAB = 1 << 18 };  // slabs of 256KB

	struct Pool {
		void* freelist[NBUCKETS];
		char* slab;
		size_t slabLeft;
		CRITICAL_SECTION cs;
		volatile LONG init;    // 0=uninit 1=initializing 2=ready
		volatile LONGLONG live, slabs;
	};

	inline Pool& P() { static Pool p = { {0}, 0, 0, {0}, 0, 0, 0 }; return p; }

	inline void ensureReady(Pool& p) {
		if (p.init == 2) return;
		if (InterlockedCompareExchange(&p.init, 1, 0) == 0) {
			InitializeCriticalSection(&p.cs);
			p.init = 2;
		} else {
			while (p.init != 2) Sleep(0);
		}
	}

	inline int bucketOf(size_t n) { return (int)((n + (ALIGN - 1)) / ALIGN); }

	inline void* alloc(size_t n) {
		if (n == 0) n = 1;
		if (n > (size_t)MAXSZ) return malloc(n);          // large: straight to the general heap
		Pool& p = P(); ensureReady(p);
		int b = bucketOf(n); size_t bs = (size_t)b * ALIGN;
		EnterCriticalSection(&p.cs);
		void* r = p.freelist[b];
		if (r) {
			p.freelist[b] = *(void**)r;                   // pop the free-list
		} else {
			if (p.slabLeft < bs) { p.slab = (char*)malloc(SLAB); p.slabLeft = p.slab ? SLAB : 0; if (p.slab) ++p.slabs; }
			if (p.slabLeft >= bs) { r = p.slab; p.slab += bs; p.slabLeft -= bs; }
			else { LeaveCriticalSection(&p.cs); return malloc(bs); }   // slab alloc failed: fall back
		}
		++p.live;
		LeaveCriticalSection(&p.cs);
		return r;
	}

	inline void freeSized(void* q, size_t n) {
		if (!q) return;
		if (n == 0) n = 1;
		if (n > (size_t)MAXSZ) { free(q); return; }
		Pool& p = P();
		int b = bucketOf(n);
		EnterCriticalSection(&p.cs);
		*(void**)q = p.freelist[b];                       // push onto the free-list (reused, not returned to heap)
		p.freelist[b] = q;
		--p.live;
		LeaveCriticalSection(&p.cs);
	}
}

// std::allocator adapter -- deallocate() gets the element count, so the size is always known.
template<class T> struct J2xPoolAlloc {
	typedef T value_type; typedef T* pointer; typedef const T* const_pointer;
	typedef T& reference; typedef const T& const_reference;
	typedef size_t size_type; typedef ptrdiff_t difference_type;
	template<class U> struct rebind { typedef J2xPoolAlloc<U> other; };
	J2xPoolAlloc() {}
	J2xPoolAlloc(const J2xPoolAlloc&) {}
	template<class U> J2xPoolAlloc(const J2xPoolAlloc<U>&) {}
	pointer address(reference x) const { return &x; }
	const_pointer address(const_reference x) const { return &x; }
	pointer allocate(size_type n, const void* = 0) { return (pointer)J2xPool::alloc(n * sizeof(T)); }
	void deallocate(pointer p, size_type n) { J2xPool::freeSized(p, n * sizeof(T)); }
	void construct(pointer p, const T& v) { ::new((void*)p) T(v); }
	void destroy(pointer p) { p->~T(); (void)p; }
	size_type max_size() const { return (size_type)(-1) / sizeof(T); }
	bool operator==(const J2xPoolAlloc&) const { return true; }
	bool operator!=(const J2xPoolAlloc&) const { return false; }
};

// Per-class fixed-size pooling. operator delete has no size in C++03, so we hardcode sizeof(Class).
#define J2X_POOLED_NEW_DELETE(Class) \
	static void* operator new(size_t n)   { return J2xPool::alloc(n); } \
	static void  operator delete(void* p) { J2xPool::freeSized(p, sizeof(Class)); } \
	static void* operator new(size_t, void* p) { return p; } \
	static void  operator delete(void*, void*) {}

#endif // J2X_POOL_H
