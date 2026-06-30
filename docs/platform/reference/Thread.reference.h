#ifndef THREAD_H
#define THREAD_H
// java.lang.Thread -> REAL Xbox 360 threads. The 360 has 3 cores / 6 hardware threads -- use them.
// start() spawns an OS thread that runs run() (NOT inline). sleep() REALLY sleeps, so a
// `while(running) Thread::sleep(...)` daemon loop (e.g. ThreadSleepForever) parks instead of
// busy-spinning a core or (when run inline) hanging the main thread. join() waits.
//
// The main game loop is the MAIN thread (Main.cpp g_minecraft->run()); these are the daemon/timer +
// the (network) download/netcode workers. CAVEAT: D3D9 and most game state are NOT thread-safe --
// only the main thread touches the render device. Anything a worker shares with the main thread must
// be locked; today the only worker that does real work on the menu/single-player path is the timer
// daemon (pure sleep). Use Win32/XDK threading (CreateThread/Sleep/WaitForSingleObject via xtl.h).
#include <string>
#include "Common.h"          // xtl.h: HANDLE/DWORD/WINAPI/LPVOID, CreateThread/Sleep/WaitForSingleObject
#include "Runnable.h"

class Thread : public Runnable {
	Runnable* _target;
	std::string _name;
	HANDLE _handle;
	static DWORD WINAPI _entry(LPVOID p) { ((Thread*)p)->run(); return 0; }
public:
	Thread() : _target(0), _handle(0) {}
	Thread(const std::string& n) : _target(0), _name(n), _handle(0) {}
	template<class R> Thread(R* r) : _target(r), _handle(0) {}                       // new Thread(runnable)
	template<class R> Thread(R* r, const std::string& n) : _target(r), _name(n), _handle(0) {}
	virtual ~Thread() {}
	virtual void run() { if (_target) _target->run(); }   // Thread(Runnable) form delegates to the target
	void start() { if (!_handle) _handle = CreateThread(0, 0, _entry, this, 0, 0); }   // real OS thread
	void setDaemon(bool) {}
	void setName(const std::string& n) { _name = n; }
	void setPriority(int) {}
	void interrupt() {}
	void join() { if (_handle) WaitForSingleObject(_handle, INFINITE); }
	void stop() {}
	void suspend() {}
	void resume() {}
	bool isAlive() { return _handle != 0; }
	static void sleep(long long ms) { Sleep(ms > 0 ? (DWORD)ms : 1); }   // REAL sleep -- never busy-spin
	static void yield() { Sleep(0); }
};
#endif // THREAD_H
