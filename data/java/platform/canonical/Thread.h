#ifndef THREAD_H
#define THREAD_H
// java.lang.Thread -- SELF-CONTAINED STUB for the converter's syntax gate (out, no platform
// headers). start() is a no-op here so output compiles standalone. The REAL build uses the platform
// version docs/platform/reference/Thread.reference.h, which spawns REAL Xbox 360 threads (CreateThread) and a
// REAL Thread::sleep -- the swap copies it over this stub. (Do NOT run run() inline: ThreadSleepForever
// is `while(mc->running) sleep()`, an infinite loop on the main thread.)
#include <string>
#include "Runnable.h"

class Thread : public Runnable {
public:
	Thread() {}
	Thread(const std::string&) {}
	template<class R> Thread(R*) {}                       // new Thread(runnable)
	template<class R> Thread(R*, const std::string&) {}   // new Thread(runnable, name)
	virtual ~Thread() {}
	virtual void run() {}
	void start() {}                                       // NO-OP (see header note) -- do not run inline
	void setDaemon(bool) {}
	void setName(const std::string&) {}
	void setPriority(int) {}
	void interrupt() {}
	void join(...) {}                                    // join() / join(millis) -- no-op (start() is a no-op)
	static void dumpStack() {}
	void stop() {}
	void suspend() {}
	void resume() {}
	bool isAlive() { return false; }
	static void sleep(long long) {}
	static void yield() {}
};
#endif // THREAD_H
