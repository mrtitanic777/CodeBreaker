// codebreaker/base/parallel.cpp -- C++17 port of codebreaker.base.parallel (see parallel.hpp).

#include "codebreaker/base/parallel.hpp"

#include <algorithm>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <variant>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <sys/locking.h>
#include <sys/stat.h>
#else
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::base::parallel {

namespace {

// A setting as Python's settings.get hands it back: settings.json holds low_load, workers and min_free_gb as
// numbers, booleans or text, and the callers below take each as Python does. (std::variant: the one place
// this module's values are of several types; monostate is None.)
using Setting = py::settings::Value;

// `os.environ.get(name) or settings.get(key)`: the variable if it is set and not empty, else the setting
Setting env_or_setting(const char* name, const char* key) {
    std::optional<py::str> v = py::os::environ_::get(py::str(name));
    if (v && !v->u32().empty()) return *v;
    return py::settings::get(py::str(key));
}

// str(v)
py::str setting_text(const Setting& v) {
    if (auto* s = std::get_if<py::str>(&v)) return *s;
    if (auto* i = std::get_if<int64_t>(&v)) return py::to_str(*i);
    if (auto* d = std::get_if<double>(&v)) return py::to_str(*d);
    if (auto* b = std::get_if<bool>(&v)) return py::str(*b ? "True" : "False");
    return py::str("None");
}

// settings.state("slots", ""): a path inside the state folder, which is created if it is not there.
py::str state_slots() {
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), py::str("slots"), py::str(""));
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

}  // namespace

int64_t cpu_count() {
    // os.cpu_count() or 4
#ifdef _WIN32
    int64_t n = static_cast<int64_t>(GetActiveProcessorCount(ALL_PROCESSOR_GROUPS));
#else
    int64_t n = static_cast<int64_t>(std::thread::hardware_concurrency());
#endif
    return n ? n : 4;
}

const int64_t LOW_LOAD = 4;                    // what the setting low_load holds everything to

bool low_load() {
    // settings.json "low_load" (or CB_LOW_LOAD=1): the machine is someone's to work on while
    // CodeBreaker runs -- every count below is held to LOW_LOAD
    py::str t = setting_text(env_or_setting("CB_LOW_LOAD", "low_load")).strip().lower();
    return !(t == py::str("") || t == py::str("0") || t == py::str("false") || t == py::str("no") ||
             t == py::str("off") || t == py::str("none"));
}

int64_t workers() {
    // How many things run side by side, everywhere: the one number every part of the workbench
    // asks for (compiles, judging, sweeps). settings.json "workers", or CB_JOBS; otherwise every
    // processor but two (the two left for the machine's owner and for the process handing the work
    // out). With low_load, never more than LOW_LOAD.
    Setting v = env_or_setting("CB_JOBS", "workers");
    int64_t n = 0;                                     // int(v), or 0 when it is not a number (TypeError, ValueError)
    if (auto* text = std::get_if<py::str>(&v)) {
        try {
            n = py::int_from_str(*text);
        } catch (const py::ValueError&) {
            n = 0;
        } catch (const py::OverflowError&) {
            // beyond int64: a huge number (kept as the largest) or a huge negative one (not above 0)
            n = text->strip().startswith(py::str("-")) ? 0 : INT64_MAX;
        }
    } else if (auto* i = std::get_if<int64_t>(&v)) {
        n = *i;
    } else if (auto* d = std::get_if<double>(&v)) {
        n = py::int_from_float(*d);
    } else if (auto* b = std::get_if<bool>(&v)) {
        n = *b ? 1 : 0;
    }
    n = n > 0 ? n : std::max<int64_t>(1, cpu_count() - 2);
    return low_load() ? std::min(n, LOW_LOAD) : n;
}

// ---------------------------------------------------------------------------
// Background work: below normal priority, on a bounded number of cores.
// ---------------------------------------------------------------------------
bool throttle(std::optional<int64_t> cores, bool below_normal) {
    // Lower this process's priority and/or pin it to the first `cores` CPUs, so a background
    // sweep leaves the machine usable. Child processes inherit both. Windows only; a no-op (False)
    // elsewhere or on failure.
#ifdef _WIN32
    // OpenProcess on our own pid: the GetCurrentProcess pseudo-handle overflows a c_void_p.
    HANDLE h = OpenProcess(0x1F0FFF, FALSE, GetCurrentProcessId());
    if (!h) return false;
    // (the ctypes call hands the handle back as a C int)
    HANDLE ih = reinterpret_cast<HANDLE>(static_cast<intptr_t>(static_cast<int32_t>(reinterpret_cast<uintptr_t>(h))));
    if (below_normal) SetPriorityClass(ih, 0x00004000);                // BELOW_NORMAL_PRIORITY_CLASS
    if (cores && *cores != 0) {
        int64_t n = std::min(*cores, cpu_count());
        // (1 << n) - 1: a negative count is the Python's ValueError, and a mask that does not fit the
        // 32-bit C int ctypes passes is its ArgumentError -- both end in `return False` (handle left open)
        if (n < 0 || n > 32) return false;
        uint64_t mask = (static_cast<uint64_t>(1) << n) - 1;
        SetProcessAffinityMask(ih, static_cast<DWORD_PTR>(static_cast<int64_t>(static_cast<int32_t>(static_cast<uint32_t>(mask)))));
    }
    CloseHandle(ih);
    return true;
#else
    (void)cores; (void)below_normal;
    return false;
#endif
}

std::optional<double> available_gb() {
    // the memory the system can still give out without paging, in GB (None where not known)
#ifdef _WIN32
    MEMORYSTATUSEX m;
    m.dwLength = sizeof(m);
    if (GlobalMemoryStatusEx(&m))
        return static_cast<double>(std::min(m.ullAvailPhys, m.ullAvailPageFile)) / 1073741824.0;   // (RAM free, and commit left)
    return std::nullopt;
#else
    std::ifstream f("/proc/meminfo");
    std::string line;
    while (std::getline(f, line))
        if (line.rfind("MemAvailable:", 0) == 0) return static_cast<double>(std::stoll(line.substr(13))) / 1048576.0;
    return std::nullopt;
#endif
}

double min_free_gb() {
    // settings.json "min_free_gb" (or CB_MIN_FREE_GB): below this much free memory no new heavy job starts
    Setting v = env_or_setting("CB_MIN_FREE_GB", "min_free_gb");
    if (auto* text = std::get_if<py::str>(&v)) {
        try {
            return py::float_from_str(*text);
        } catch (const py::ValueError&) {
            return 6.0;
        }
    }
    if (auto* i = std::get_if<int64_t>(&v)) return static_cast<double>(*i);
    if (auto* d = std::get_if<double>(&v)) return *d;
    if (auto* b = std::get_if<bool>(&v)) return *b ? 1.0 : 0.0;
    return 6.0;                                        // None: float(None) is a TypeError
}

double wait_for_memory(const py::str& label, double poll, double limit) {
    // before a heavy job: wait while the system has less than min_free_gb free - the machine first, the work
    // after. Returns the seconds waited.
    double need = min_free_gb(), waited = 0.0;
    for (;;) {
        std::optional<double> a = available_gb();
        if (!a || *a >= need || waited >= limit) return waited;
        if (waited == 0) {
            py::str text = py::percent(py::str("(waiting: %.1f GB free, %.1f wanted%s)"), *a, need,
                                       !label.u32().empty() ? py::str(" - ") + label : py::str(""));
            std::string out = text.utf8() + "\n";
            std::fwrite(out.data(), 1, out.size(), stderr);
            std::fflush(stderr);
        }
        py::time::sleep(poll);
        waited += poll;
    }
}

std::optional<double> total_gb() {
    // the machine's memory in GB (None where not known)
#ifdef _WIN32
    ULONGLONG kb = 0;
    if (GetPhysicallyInstalledSystemMemory(&kb)) return static_cast<double>(kb) / 1048576.0;   // (KB)
    return std::nullopt;
#else
    long page = sysconf(_SC_PAGE_SIZE), pages = sysconf(_SC_PHYS_PAGES);
    if (page < 0 || pages < 0) return std::nullopt;
    return static_cast<double>(page) * static_cast<double>(pages) / 1073741824.0;
#endif
}

// ---------------------------------------------------------------------------
// The governor: one budget of heavy jobs for the whole machine.
// ---------------------------------------------------------------------------
Slot_::~Slot_() {
    try {
        give_back();
    } catch (...) {
    }
}

bool Slot_::take() {
    std::string p = path.utf8();
#ifdef _WIN32
    std::wstring w = std::filesystem::u8path(p).wstring();
    int fd = _wopen(w.c_str(), _O_RDWR | _O_CREAT | _O_APPEND | _O_BINARY | _O_NOINHERIT, _S_IREAD | _S_IWRITE);   // open(path, "a+b")
    if (fd < 0) throw py::OSError("cannot open " + p);
    _lseek(fd, 0, SEEK_SET);
    if (_locking(fd, _LK_NBLCK, 1) != 0) {
        _close(fd);
        return false;
    }
#else
    int fd = ::open(p.c_str(), O_RDWR | O_CREAT | O_APPEND, 0666);
    if (fd < 0) throw py::OSError("cannot open " + p);
    if (flock(fd, LOCK_EX | LOCK_NB) != 0) {
        close(fd);
        return false;
    }
#endif
    fd_ = fd;
    return true;
}

void Slot_::give_back() {
    if (fd_ < 0) return;
#ifdef _WIN32
    _lseek(fd_, 0, SEEK_SET);
    _locking(fd_, _LK_UNLCK, 1);   // (an error here is passed over)
    _close(fd_);
#else
    close(fd_);
#endif
    fd_ = -1;
}

double reserve_gb() {
    // memory the governor leaves to the system: min_free_gb, but no more than a quarter of the machine
    std::optional<double> t = total_gb();
    return (t && *t != 0.0) ? std::min(min_free_gb(), *t / 4) : min_free_gb();
}

std::optional<double> idle_cores(double sample) {
    // processors nothing is using, measured over `sample` seconds (None where not known): what
    // other programs keep busy -- a build beside us -- is not ours to take
#ifdef _WIN32
    try {
        auto times = [](uint64_t& idle, uint64_t& total) {
            FILETIME i, k, u;
            GetSystemTimes(&i, &k, &u);
            auto v = [](const FILETIME& f) { return (static_cast<uint64_t>(f.dwHighDateTime) << 32) | f.dwLowDateTime; };
            idle = v(i);
            total = v(k) + v(u);                                 // (kernel time includes idle)
        };
        uint64_t i0, t0, i1, t1;
        times(i0, t0);
        py::time::sleep(sample);
        times(i1, t1);
        if (t1 > t0) return static_cast<double>(static_cast<uint64_t>(cpu_count()) * (i1 - i0)) / static_cast<double>(t1 - t0);
        return std::nullopt;
    } catch (const py::Exception&) {
        return std::nullopt;
    }
#else
    (void)sample;
    double load[1];
    if (getloadavg(load, 1) < 1) return std::nullopt;
    return std::max(0.0, static_cast<double>(cpu_count()) - load[0]);
#endif
}

std::optional<int64_t> memory_allows(int64_t mb_each) {
    // how many pieces of `mb_each` MB the free memory carries past the reserve (None: not known)
    std::optional<double> a = available_gb();
    if (!a) return std::nullopt;
    return py::int_from_float(py::floordiv(std::max(0.0, *a - reserve_gb()) * 1024, static_cast<double>(std::max<int64_t>(1, mb_each))));
}

slots::slots(int64_t want, int64_t mb_each, const py::str& label, double limit, double poll)
    : want(std::max<int64_t>(1, want)), mb_each(mb_each), label(label), limit(limit), poll(poll) {}

slots::~slots() {
    exit();
}

int64_t slots::enter() {
    int64_t cap = workers();
    py::str folder = state_slots();
    std::optional<int64_t> mem = memory_allows(mb_each);
    int64_t want_now = std::min(std::min(want, cap), mem ? std::max<int64_t>(1, *mem) : cap);
    if (want_now > 1) {                                          // (and no more than the processors free)
        std::optional<double> idle = idle_cores();
        if (idle) want_now = std::min(want_now, std::max<int64_t>(1, py::int_from_float(*idle + 0.5)));
    }
    double waited = 0.0;
    for (;;) {
        for (int64_t i = 0; i < cap; ++i) {
            if (static_cast<int64_t>(held.size()) >= want_now) break;
            auto s = std::make_shared<Slot_>(py::os::path::join(folder, py::str("slot-") + py::to_str(i)));
            if (s->take()) held.push_back(s);
        }
        if (!held.empty() || waited >= limit) return std::max<int64_t>(1, static_cast<int64_t>(held.size()));
        if (waited == 0) {
            py::str text = py::str("(waiting for a free slot") + (!label.u32().empty() ? py::str(" - ") + label : py::str("")) + ")";
            std::string out = text.utf8() + "\n";
            std::fwrite(out.data(), 1, out.size(), stderr);
            std::fflush(stderr);
        }
        py::time::sleep(poll);
        waited += poll;
    }
}

bool slots::exit() {
    for (auto& s : held) s->give_back();
    held.clear();
    return false;
}

}  // namespace codebreaker::base::parallel
