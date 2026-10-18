#ifndef J2X_THREADPOOL_H
#define J2X_THREADPOOL_H
// ============================================================================
// j2x_pool -- a tiny, self-contained worker pool for the Xbox 360 (Xenon: 3 cores x 2 HW
// threads = HW threads 0..5). The main game loop runs on HW0, XACT audio on HW5; HW1..4 are
// idle. This pool spins up workers pinned to the idle HW threads (default: 2 and 4, one per
// free physical core) so the heavy, parallelizable game work (chunk meshing, world gen) can
// run off the main thread.
//
// Header-only on purpose: inline functions + a single function-local static State give ONE
// shared instance across translation units with no .vcxproj/.obj to add and no ODR clash.
//
// VERIFIED vs the XDK (and mirrors the proven XactAudio.cpp thread): CreateThread(CREATE_
// SUSPENDED) -> XSetThreadProcessor(handle, hw) -> ResumeThread; CRITICAL_SECTION queue lock;
// CreateSemaphore job signal; Interlocked* for the lock-free counters.
//
// CONTRACT (thread-safety): start() MUST be called once from the MAIN thread before any
// submit()/wait() and before workers run -- that single-threaded first call constructs the
// State (C++03 function-local statics are not race-safe to initialize). A submitted JobFn runs
// on a worker thread; it must only touch data it exclusively owns or that is otherwise
// synchronized (snapshots, per-job output slots). The pool itself adds no game-data safety.
// ============================================================================
#include "stdafx.h"   // xtl.h: CreateThread / XSetThreadProcessor / CRITICAL_SECTION / CreateSemaphore / Interlocked*

namespace j2x_pool {

typedef void (*JobFn)(void* arg);

enum { CAP = 4096, MAX_WORKERS = 4 };

struct _Job { JobFn fn; void* arg; };

struct _State {
	CRITICAL_SECTION cs;       // guards the ring head/tail
	HANDLE  sem;               // counts queued-but-not-yet-claimed jobs
	_Job    ring[CAP];
	volatile LONG head;        // next slot a worker will claim
	volatile LONG tail;        // next slot submit() will fill
	volatile LONG pending;     // submitted - completed (barrier counter)
	volatile LONG running;     // 1 while workers should keep looping
	HANDLE  workers[MAX_WORKERS];
	int     numWorkers;
	bool    started;
};

// Single shared instance. First touched (single-threaded) from start().
inline _State& _st() { static _State s = { 0 }; return s; }

inline DWORD WINAPI _workerProc(LPVOID) {
	_State& s = _st();
	for (;;) {
		WaitForSingleObject(s.sem, INFINITE);      // sleep until a job is available
		if (!s.running) break;
		_Job job; bool got = false;
		EnterCriticalSection(&s.cs);
		if (s.head != s.tail) { job = s.ring[s.head & (CAP - 1)]; ++s.head; got = true; }
		LeaveCriticalSection(&s.cs);
		if (got) {
			if (job.fn) job.fn(job.arg);
			InterlockedDecrement(&s.pending);
		}
	}
	return 0;
}

// Start the pool. hwThreads[] lists the HW thread to pin each worker to (e.g. {2,4}).
// Idempotent; call once from the main thread.
inline void start(int n, const int* hwThreads) {
	_State& s = _st();
	if (s.started) return;
	if (n < 1) n = 1; if (n > MAX_WORKERS) n = MAX_WORKERS;
	InitializeCriticalSection(&s.cs);
	s.sem     = CreateSemaphore(NULL, 0, CAP, NULL);
	s.head    = 0; s.tail = 0; s.pending = 0; s.running = 1;
	s.numWorkers = n;
	for (int i = 0; i < n; ++i) {
		s.workers[i] = CreateThread(NULL, 0, _workerProc, NULL, CREATE_SUSPENDED, NULL);
		if (s.workers[i]) {
			XSetThreadProcessor(s.workers[i], hwThreads ? hwThreads[i] : (2 + i * 2));
			ResumeThread(s.workers[i]);
		}
	}
	s.started = true;
}

inline bool isStarted() { return _st().started; }
inline int  workerCount() { return _st().numWorkers; }

// Queue a job. If the pool isn't started (or the ring is momentarily full), run it INLINE on
// the caller so work is never dropped -- correctness over parallelism.
inline void submit(JobFn fn, void* arg) {
	_State& s = _st();
	if (!s.started || !fn) { if (fn) fn(arg); return; }
	bool queued = false;
	EnterCriticalSection(&s.cs);
	if ((s.tail - s.head) < CAP) { s.ring[s.tail & (CAP - 1)].fn = fn; s.ring[s.tail & (CAP - 1)].arg = arg; ++s.tail; queued = true; }
	LeaveCriticalSection(&s.cs);
	if (queued) { InterlockedIncrement(&s.pending); ReleaseSemaphore(s.sem, 1, NULL); }
	else        { fn(arg); }   // ring full -> do it here
}

// How many submitted jobs have not finished yet.
inline LONG pending() { return _st().pending; }

// Barrier: block the caller until every submitted job has completed. Yields (Sleep(0)) rather
// than hard-spinning so the worker HW threads aren't starved. Use for one-shot fan-outs (e.g.
// the spawn pre-gen). The per-frame mesh path should instead poll pending()/drain results and
// NOT block, so the main thread keeps rendering.
inline void wait() {
	_State& s = _st();
	while (s.pending > 0) { Sleep(0); }
}

// Shutdown (rarely needed on console; the game runs until power-off).
inline void stop() {
	_State& s = _st();
	if (!s.started) return;
	InterlockedExchange(&s.running, 0);
	if (s.sem) ReleaseSemaphore(s.sem, s.numWorkers, NULL);   // wake workers to observe running==0
	for (int i = 0; i < s.numWorkers; ++i) {
		if (s.workers[i]) { WaitForSingleObject(s.workers[i], 2000); CloseHandle(s.workers[i]); s.workers[i] = NULL; }
	}
	if (s.sem) { CloseHandle(s.sem); s.sem = NULL; }
	DeleteCriticalSection(&s.cs);
	s.started = false;
}

} // namespace j2x_pool

#endif // J2X_THREADPOOL_H
