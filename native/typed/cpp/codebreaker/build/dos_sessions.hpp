// codebreaker/build/dos_sessions.hpp -- typed C++ from codebreaker.build.dos_sessions.
//
// dosworker.py -- a POOL of persistent, warm DOSBox+MASM sessions.
//
// The DOSBox BOOT (~1.5-2 s) is the dominant per-assemble cost. `masmlift` amortised
// it by batching many functions into one boot; this eliminates the RE-boot entirely:
// each worker boots DOSBox ONCE and then loops, assembling batch after batch without
// restarting. A batch that took a fresh boot now costs ~0.3 s.
//
// How the warm session works (validated):
//   * a SINGLE small mount (C:) holds MASM.EXE + the work files -- one small mount so
//     DOSBox's `RESCAN` reliably sees host-created files (a big mount does not);
//   * a DOS dispatch loop polls: `rescan` -> if C:\GO exists -> `call C:\WORK.BAT`
//     (the MASM commands the host just wrote) -> write C:\ACK -> loop;
//   * the host drops the .ASM + WORK.BAT + GO, waits for ACK, reads the .OBJ/.LST.
//
// A pool hands out warm workers to the conductor's fan-out (one worker per concurrent
// chunk). Falls back cleanly: a dead worker is dropped and a fresh one boots.
//
// Stand-ins for what the Python takes from its standard library and the runtime lacks:
//   Popen      subprocess.Popen(args, env=..., stdout=DEVNULL, stderr=DEVNULL): poll / kill / wait(timeout)
//   Semaphore  threading.Semaphore
#pragma once

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::dos_sessions {

// WAIT.COM: BIOS wait (INT 15h/86h) ~50ms, then exit. A bounded busy-wait -- used
// to build a short LOW-LATENCY response window before the worker falls back to the
// CPU-cheap (but 1s-coarse) CHOICE throttle.
extern const py::bytes _WAIT_COM;   // mov ah,86h;cx=0;dx=C350h;int15h;int20h

// Hybrid idle loop: a few fast `wait`+rescan checks (~200ms window that catches a
// back-to-back next batch almost instantly), and only THEN a 1s CHOICE keyboard-
// wait that DOSBox idle-throttles to ~7% of a core. A bare `goto loop` busy-spins
// a full core (a pool of those pegs the machine and STARVES MASM); a pure 1s
// CHOICE throttles but adds ~1s latency to every assemble. The hybrid gives both:
// low latency when busy, low CPU when genuinely idle.
extern const py::str _POLL;
extern const py::str _WORKER_BAT;
extern const py::str _CONF;

// A child process: what DosboxWorker uses of subprocess.Popen.
class Popen {
  public:
    // starts args[0] with the other arguments, the parent's environment plus `extra_env`, stdout and stderr to the
    // null device (OSError / FileNotFoundError when it cannot start)
    Popen(const py::list<py::str>& args, const py::dict<py::str, py::str>& extra_env);
    Popen(const Popen&) = delete;
    Popen& operator=(const Popen&) = delete;
    ~Popen();
    // the exit code, or None while it runs
    std::optional<int64_t> poll();
    // stops it (nothing when it has ended)
    void kill();
    // waits up to `timeout` seconds for it to end -> the exit code; TimeoutError past it
    int64_t wait(std::optional<double> timeout = std::nullopt);

  private:
    void* handle_ = nullptr;
    std::optional<int64_t> returncode_;
};

// One file an assembly produced: the keys of the dict the Python hands back, each present only when the file exists.
struct Product {
    std::optional<py::bytes> obj;   // the .OBJ
    std::optional<py::str> lst;     // the .LST (latin1 text)
    std::optional<py::str> log;     // the .LOG (latin1 text)
};

// What assemble_many answers: {'results': {stem: Product}, 'work': dir}, {'aborted': True} or {'dead': True}.
struct Assembled {
    bool aborted = false;
    bool dead = false;
    std::optional<py::dict<py::str, Product>> results;
    std::optional<py::str> work;
};

// One persistent DOSBox session running the MASM dispatch loop.
class DosboxWorker {
  public:
    py::str dbx;
    py::str work;
    py::str conf;
    std::unique_ptr<Popen> proc;

    DosboxWorker(const py::str& masmroot, const py::str& dbx, double boot_timeout = 20);

    py::str _p(const py::str& name) const;
    void _wait_boot(double timeout);

    // Assemble [(stem, asm_src)] in this warm session. Returns
    // {'results': {stem: {obj,lst,log}}} or {'aborted': True}/{'dead': True}.
    Assembled assemble_many(const py::list<std::tuple<py::str, py::str>>& items, std::optional<double> timeout = std::nullopt,
                            const std::function<bool()>& cancelled = nullptr);

    bool alive();
    void close();
};

class Semaphore {
  public:
    explicit Semaphore(int64_t n) : n_(n) {}
    void acquire();
    void release();

  private:
    std::mutex m_;
    std::condition_variable cv_;
    int64_t n_;
};

// Thread-safe pool of warm workers, capped at `maxsize`.
class WorkerPool {
  public:
    py::str masmroot;
    py::str dbx;

    WorkerPool(const py::str& masmroot, const py::str& dbx, int64_t maxsize);

    std::shared_ptr<DosboxWorker> lease();
    void release(const std::shared_ptr<DosboxWorker>& w, bool keep = true);
    void close();

  private:
    Semaphore _sem;
    std::vector<std::shared_ptr<DosboxWorker>> _idle;
    std::vector<std::shared_ptr<DosboxWorker>> _all;
    std::mutex _lock;
};

}  // namespace codebreaker::build::dos_sessions
