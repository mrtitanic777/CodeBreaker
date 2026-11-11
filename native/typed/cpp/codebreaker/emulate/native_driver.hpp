// codebreaker/emulate/native_driver.hpp -- C++ port of codebreaker.emulate.native_driver.
//
// native_driver.py -- driver.py run()'s hooks in C++ (native/cb/emulate/tracer: cbtrace.dll), through ctypes.
//
// A run of a VxD entry point used to call back into Python on every instruction and every memory
// access. Here those hooks are C++: emulate/driver.run still lays the regions out, sets the
// registers, starts the run (uc.emu_start to _RET) and reads the result; this module attaches the
// DLL's driver hooks to the same Unicorn engine (uc._uch), in driver.run's order, and turns what
// they recorded back into the same rec dict. During a run Python is asked one thing: which register
// a service answers in (driver_services + the knowledge base), once per distinct service.
//
//     run = native_driver.Run(uc, ...)   # the hooks attached (None when the DLL cannot be used here)
//     uc.emu_start(entry, RET, count=cap + 1)
//     run.collect(rec)                   # the rec fields the Python hooks would have left
//     run.detach()
//
// The Python hooks this replaced are gone (docs/NATIVE.md, step 3): without the DLL a run raises
// (require()). Build: tools\build_cb.cmd (native/cb/CMakeLists.txt, target cbtrace).
//
// Typing notes (where the C++ has to say what the Python leaves to ctypes and dicts):
//   ctypes structures  _Config and _Result are the C structures of native/cb/emulate/tracer/cbtrace.h (cbt_drv_config,
//                      cbt_drv_result), field for field; Lib holds the DLL's cbt_drv_* entry points (what
//                      lib.cbt_drv_* are on the ctypes CDLL). native_trace::_load() loads the DLL (and keeps
//                      its entry points in native_trace::Lib); `_lib()` binds the driver ones from the same
//                      DLL, once, as the Python's _READY guards.
//   the Unicorn engine `uc` is native_trace::Engine (its handle uc._uch); driver.run starts the run itself.
//   the record         the dict driver.run's rec is, restricted to the keys collect() sets: DriverRecord,
//                      each key an std::optional that is empty when the key is left as it was.
//   the callback       the DLL's `answers` has no context argument (a ctypes closure carries its Run); a
//                      fixed pool of C++ entry points, one per live Run, stands in for the closure.
//   BaseException      the exception `answers` raises is kept as a std::exception_ptr (any exception).
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "py.hpp"
#include "codebreaker/emulate/native_trace.hpp"

namespace codebreaker::emulate::native_driver {

inline constexpr int64_t _CALL_WORDS = 15, _PORT_WORDS = 4, _MEM_WORDS = 8, _LAZY_WORDS = 5;

// The DLL calls this when a service is called: (device, service, out) -> 0, or nonzero when the answer raised.
using ANSWERS = int (*)(int32_t, int32_t, int32_t*);

// ---- native/cb/emulate/tracer/cbtrace.h: cbt_drv_config and cbt_drv_result, field for field ------------------
struct _Config {
    void* uch = nullptr;
    int32_t reg_ids[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int32_t ins_in = 0, ins_out = 0;
    uint64_t vm_addr = 0, ret = 0;
    uint64_t scratch_lo = 0, scratch_hi = 0;
    const uint64_t* spans = nullptr;
    int64_t nspans = 0;
    int64_t cap = 0, service_eax = 0, service_carry = 0;
    int64_t gov_n = 0, gov_ceil = 0;
    ANSWERS answers = nullptr;
    void* shadow = nullptr;
};

struct _Result {
    int64_t exit_code, exit_int, insns, truncated, governed, outside, memops, writes_seen, stuck, gov_n, hook_error, uc_err;
    const void* cover;
    int64_t ncover;
    const void* calls;
    int64_t ncalls;
    const void* ports;
    int64_t nports;
    const void* reads;
    int64_t nreads;
    const void* writes;
    int64_t nwrites;
    const void* lazy;
    int64_t nlazy;
};

// The DLL's driver entry points (the ctypes CDLL's cbt_drv_* functions).
struct Lib {
    void* (*cbt_drv_begin)(const _Config*) = nullptr;
    int (*cbt_drv_attach)(void*) = nullptr;
    int (*cbt_drv_detach)(void*) = nullptr;
    void (*cbt_drv_free)(void*) = nullptr;
    void (*cbt_drv_state)(void*, int64_t*) = nullptr;
    const _Result* (*cbt_drv_result_get)(void*) = nullptr;
};

// [bound]: the driver entry points, once they are prepared on the DLL
extern py::list<std::shared_ptr<Lib>> _READY;

// The cbtrace.dll native_trace loaded, with the driver entry points bound, or None (empty).
std::shared_ptr<Lib> _lib();
// The driver recorder for this process: the DLL loads here (the Python hooks it replaced are gone;
// require() raises with why_not() when it does not).
bool enabled();
std::optional<py::str> why_not();

// (the array ctypes makes, at least one long, and the count)
std::tuple<std::shared_ptr<std::vector<uint64_t>>, int64_t> _u64s(const std::vector<uint64_t>& values);
// ctypes.string_at(ptr, ...) read as `n` records of `count` words each (struct.iter_unpack("<%dQ")).
py::list<std::vector<uint64_t>> _words(const void* ptr, int64_t n, int64_t count);

// driver.REGISTERS and emulator._RET (exit computed in driver.run after the run).
extern const py::dict<int64_t, py::str> _EXITS;

// ---- the record (the keys collect() sets) ---------------------------------------------------------
// A "calls" entry: (first, second, regs, words); first is None when the call went to a pointer.
using Call = std::tuple<std::optional<uint64_t>, uint64_t, py::list<uint64_t>, py::list<uint64_t>>;
// A "ports" entry: ("in" | "out", port, size, value).
using Port = std::tuple<py::str, uint64_t, uint64_t, uint64_t>;

struct DriverRecord {
    std::optional<int64_t> insns;
    std::optional<bool> truncated;
    std::optional<bool> governed;
    std::optional<int64_t> outside;
    std::optional<py::str> exit;
    std::optional<py::set<uint64_t>> cover;
    std::optional<py::list<Call>> calls;
    std::optional<py::list<Port>> ports;
    std::optional<py::list<native_trace::MemRead>> mem_reads;
    std::optional<py::list<native_trace::MemWrite>> mem_writes;
    std::optional<py::list<native_trace::LazyMap>> lazy_maps;
    std::optional<int64_t> memops;
    std::optional<int64_t> writes_seen;
    std::optional<int64_t> _stuck;
};

// The arguments of Run (its keyword arguments). `reg_ids` the Unicorn ids of eax,ebx,ecx,edx,esi,edi,ebp,esp,
// eip,eflags (at most ten); `spans` the image's [(lo, hi)]; `scratch` the scratch region (lo, hi); `gov`
// emulator._GOV; `answers(device, service)` -> the place in REGISTERS the service answers in (or -1).
struct RunArgs {
    py::list<int64_t> reg_ids;
    int64_t ins_in = 0, ins_out = 0;
    int64_t vm_addr = 0, ret = 0;
    std::tuple<int64_t, int64_t> scratch;
    py::list<std::tuple<int64_t, int64_t>> spans;
    int64_t cap = 0;
    int64_t service_eax = 0, service_carry = 0;
    native_trace::Gov* gov = nullptr;
    std::function<int64_t(int32_t, int32_t)> answers;
    void* shadow = nullptr;
};

// One driver run's native hooks on `uc`. Raises nothing itself; `enabled()` gates its use.
class Run {
  public:
    std::shared_ptr<Lib> lib;
    native_trace::Engine uc;
    native_trace::Gov* gov = nullptr;
    std::function<int64_t(int32_t, int32_t)> answers_fn;
    std::exception_ptr exc;
    _Config config;
    void* handle = nullptr;

    Run(const native_trace::Engine& uc, const RunArgs& a);
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;
    ~Run();

    // Which register the service answers in (driver.answers_in), or -1.
    int _resolve(int32_t device, int32_t service, int32_t* out);
    // The exception the resolver raised, if any (driver.run has no try around the hook, so a
    // resolver that raised comes out of emu_start as a UcError; this is the original).
    std::exception_ptr raised();
    // The rec fields the Python hooks would have left, as data.
    void collect(DriverRecord& rec);
    void detach();

  private:
    int slot_ = -1;
    std::shared_ptr<std::vector<uint64_t>> keep_spans_;
    int64_t state_[3] = {0, 0, 0};
};

// The recorder is C++ only (the Python hooks it replaced are gone): raise when the DLL cannot
// be used here, saying why.
void require();

}  // namespace codebreaker::emulate::native_driver
