#ifndef THREAD_H
#define THREAD_H
// java.lang.Thread. start() is a NO-OP BY DEFAULT: on the console the java daemon threads (StatsSyncher,
// ThreadDownload*, the timer hack) are DEAD -- they do no useful work (no HTTP/socket) and flipping the global
// start() to a real OS thread turned them on and FROZE the first menu render frame. The real game-parallelism
// pool is j2x_pool (ThreadPool.h) -- unaffected by this.
//
// FIXED BUG mp-threads-never-run: multiplayer's connect/reader/writer threads are REAL java Threads, so with a
// no-op start() ThreadConnectToServer::run() (socket + handshake) and the NetworkManager reader/writer loops
// never executed -- MP hung forever on "Connecting to the server..." The controlled/gated fix: start() is
// VIRTUAL, and ONLY the three net-thread classes override it (in the overlay) to call j2x_startReal() (a real
// CreateThread). Every other Thread keeps the no-op, so the dead daemons stay dead and the menu-freeze does not
// return. [360] equivalent: CreateThread / ExCreateThread + Sleep / WaitForSingleObject (xboxkrnl.exe).
#include <string>
#include "Common.h"          // xtl.h: HANDLE/DWORD/WINAPI/LPVOID, CreateThread/Sleep
#include "Runnable.h"

class Thread : public Runnable {
protected:
	HANDLE _handle;
	static DWORD WINAPI _entry(LPVOID p) { ((Thread*)p)->run(); return 0; }
	// Spawn a REAL OS thread that runs run(). Called ONLY from the net-thread start() overrides (see header note).
	void j2x_startReal() { if (!_handle) _handle = CreateThread(0, 0, _entry, this, 0, 0); }
public:
	Thread() : _handle(0) {}
	Thread(const std::string&) : _handle(0) {}
	template<class R> Thread(R*) : _handle(0) {}                       // new Thread(runnable)
	template<class R> Thread(R*, const std::string&) : _handle(0) {}   // new Thread(runnable, name)
	virtual ~Thread() {}
	virtual void run() {}
	virtual void start() {}                               // NO-OP by default; net threads override -> j2x_startReal()
	void setDaemon(bool) {}
	void setName(const std::string&) {}
	void setPriority(int) {}
	void interrupt() {}
	void join(...) {}                                     // no-op (join semantics not needed here)
	static void dumpStack() {}
	void stop() {}
	void suspend() {}
	void resume() {}
	bool isAlive() { return _handle != 0; }
	static void sleep(long long ms) { Sleep((DWORD)(ms < 0 ? 0 : ms)); }   // REAL sleep (net loops idle without busy-spin)
	static void yield() { Sleep(0); }
};
#endif // THREAD_H
