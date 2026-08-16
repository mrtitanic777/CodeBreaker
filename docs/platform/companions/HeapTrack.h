// HeapTrack.h -- global heap-leak watchdog for the j2ctool port (PLATFORM COMPANION).
//
// The no-GC port leaks any object Java would have collected; enough churn eventually exhausts the
// heap ("BaseHeap::Alloc failed to find contiguous range") and the game hangs. This watchdog makes
// that VISIBLE and ATTRIBUTABLE: it intercepts EVERY global operator new/delete, tags each live
// allocation with its CALL SITE (return address) + a size bucket, and a background thread logs the
// live total, the leak RATE, and the top-growing call sites every couple of seconds -- so even when
// the main thread wedges on an allocation, the LAST few [HEAP] reports name what ran the heap dry.
//
// Turn the printed `0x........` call-site addresses into function names with the linker MAP file
// (MinecraftBeta.map) or the .pdb -- find the address in the map's "Address" column.
//
// Toggle with J2X_HEAPTRACK in HeapTrack.cpp (default ON). When OFF the whole thing compiles out and
// the default new/delete are used.
#ifndef HEAPTRACK_H
#define HEAPTRACK_H

namespace HeapTrack {
	void start();                    // spawn the monitor thread (call once, early in main())
	void report(const char* tag);    // emit a [HEAP] snapshot NOW (the monitor thread calls this too)
	long long liveBytes();           // current outstanding bytes
	long long liveCount();           // current outstanding allocations
	long long peakBytes();
}

#endif // HEAPTRACK_H
