// java.lang.Thread on the PC: a real std::thread per start(), as in Java. The program ends when its last
// non-daemon thread does (the generated main waits: j2xThreadsWait). The game creates its window and GL
// context on the thread that calls Display.create(), so rendering stays on that one thread.
#ifndef THREAD_H
#define THREAD_H
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "Runnable.h"

namespace j2xthreads {
	inline std::mutex& lock() { static std::mutex m; return m; }
	inline std::condition_variable& done() { static std::condition_variable c; return c; }
	inline int& live() { static int n = 0; return n; }               // running non-daemon threads
}

class Thread : public Runnable {
protected:
	Runnable* _target;   // `new Thread(runnable)`'s runnable, or 0 for a Thread subclass
	bool _daemon;
	std::atomic<bool> _alive;
	std::thread _t;
	std::string _name;
	static void _entry(Thread* t) {
		t->run();
		t->_alive = false;
		if (!t->_daemon) {
			std::lock_guard<std::mutex> g(j2xthreads::lock());
			--j2xthreads::live();
			j2xthreads::done().notify_all();
		}
	}
public:
	Thread() : _target(0), _daemon(false), _alive(false) {}
	Thread(const std::string& n) : _target(0), _daemon(false), _alive(false), _name(n) {}
	template<class R> Thread(R* r) : _target(r), _daemon(false), _alive(false) {}
	template<class R> Thread(R* r, const std::string& n) : _target(r), _daemon(false), _alive(false), _name(n) {}
	virtual ~Thread() { if (_t.joinable()) _t.detach(); }
	virtual void run() { if (_target) _target->run(); }
	virtual void start() {
		if (_alive || _t.joinable()) return;
		_alive = true;
		if (!_daemon) {
			std::lock_guard<std::mutex> g(j2xthreads::lock());
			++j2xthreads::live();
		}
		_t = std::thread(_entry, this);
	}
	void setDaemon(bool d) { _daemon = d; }
	bool isDaemon() const { return _daemon; }
	void setName(const std::string& n) { _name = n; }
	std::string getName() const { return _name; }
	void setPriority(int) {}
	void interrupt() {}
	void join(...) { if (_t.joinable() && _t.get_id() != std::this_thread::get_id()) _t.join(); }
	static void dumpStack() {}
	void stop() {}
	void suspend() {}
	void resume() {}
	bool isAlive() { return _alive; }
	static void sleep(long long ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms < 0 ? 0 : ms)); }
	static void yield() { std::this_thread::yield(); }
};

// main(): wait for every non-daemon thread, as the JVM does before it exits
inline void j2xThreadsWait() {
	std::unique_lock<std::mutex> g(j2xthreads::lock());
	j2xthreads::done().wait(g, [] { return j2xthreads::live() == 0; });
}
#endif // THREAD_H
