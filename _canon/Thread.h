#ifndef THREAD_H
#define THREAD_H
// java.lang.Thread (stub: runs inline; real threading is a platform concern)
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
	void start() { run(); }
	void setDaemon(bool) {}
	void setName(const std::string&) {}
	void setPriority(int) {}
	void interrupt() {}
	void join() {}
	void stop() {}
	void suspend() {}
	void resume() {}
	bool isAlive() { return false; }
	static void sleep(long long) {}
	static void yield() {}
};
#endif // THREAD_H
