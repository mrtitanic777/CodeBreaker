// codebreaker/emulate/powerpc.hpp -- typed C++ port of codebreaker.emulate.powerpc.
//
// powerpc.py -- run a PowerPC function and record what it did.
//
//     Out r = run(regions, entry, args, options);
//     r.exit        "ret" | "exception:<why>" | "none" (the step limit was reached)
//     r.ret         r3 on return
//     r.calls       [(name, [r3..r10])]: calls that left the code given, in order
//     r.writes      {address: (byte before, byte after)} outside the stack
//     r.path        addresses inside `span` that ran
//     r.saved       whether r14-r31 and the stack pointer came back as they went in
//
// The emulator library used for x86 cannot run 64-bit PowerPC code (its one selectable 64-bit
// processor model faults on the first instruction fetch), so this is an interpreter of our own:
// the 64-bit PowerPC user instruction set with integer, branch, condition-register and
// floating-point instructions, as compilers for 32-bit-address PowerPC targets use it -- 64-bit
// registers and arithmetic, effective addresses cut to 32 bits, big-endian memory.
//
// What it does not do, and says so: vector instructions (the run ends "exception:unsupported"),
// overflow recording (XER[OV] is never set), floating-point status and rounding modes other than
// round-to-nearest, and anything privileged. tests/test_powerpc.py checks it against functions
// compiled from C whose results are known.
//
// Memory is pages created on first touch (zero-filled), with a limit on how many, so code that
// reaches into memory it was not given keeps running the same way on both sides of a comparison.
//
// Notes on the port:
//   * A register is a 64-bit pattern: Python keeps it as an int in 0..2**64-1, here it is uint64_t
//     (every wrap is exact: the unsigned arithmetic gives the same low 64 bits Python's `& M64`
//     keeps, and the places Python's ints grow past 64 bits -- mulhd, mulhdu, the carry of an add,
//     divd of the most negative number, the rotates -- are done with exact unsigned/wider steps).
//     CR, LR, the pc and the instruction word are at most 32 bits and are int64_t.
//   * run() and its keywords: the keywords are the struct RunOptions; the dict run() returns (fixed
//     keys) is the struct Out. `shadow` (emulate/shadow.py's ShadowPPC) has no port yet: it is the
//     interface Shadow, taken as a pointer (null: None).
//   * record() is the one run record (emulate/run.hpp), whose integer slots are int64_t: a register
//     value of 2**63 or more (r3, f1's raw bits, a preserved register) is stored there as the
//     int64_t of the same 64 bits.
//   * Out.path is a py::set<int64_t> and Run.path a py::list<int64_t> in the order the addresses were
//     first reached (CPython's set order is its hash layout).
//   * Python's `raise Stop("...")` is the exception class Stop.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "codebreaker/emulate/run.hpp"

namespace codebreaker::emulate::powerpc {

namespace emu_run = ::codebreaker::emulate::run;   // (this namespace has a function run(), which hides the name)

inline constexpr uint64_t M64 = 0xFFFFFFFFFFFFFFFFULL;
inline constexpr int64_t M32 = 0xFFFFFFFF;
inline constexpr int64_t RETURN = 0x7FFF0000;          // where the function returns to
inline constexpr int64_t STACK_TOP = 0x70100000;       // stack: the megabyte below this
inline constexpr int64_t STACK_LOW = 0x70000000;
inline constexpr int64_t ARGS_BASE = 0x60000000;       // memory pointer-shaped inputs can point into
inline constexpr int64_t PAGE = 0x1000;
inline constexpr int64_t MAX_PAGES = 2048;
inline constexpr int64_t MAX_STEPS = 20000;
inline constexpr int64_t MAX_CALLS = 400;

class Stop : public py::Exception {
  public:
    Stop() : py::Exception() {}
    explicit Stop(const py::str& why) : py::Exception(why) {}
    explicit Stop(const char* why) : py::Exception(why) {}
    const char* type() const override { return "Stop"; }
};

int64_t _s64(uint64_t v);
int64_t _s32(uint64_t v);
uint64_t _sext(uint64_t v, int64_t bits);
// Bits mb..me set, bit 0 the most significant of 64 (wrapping when mb > me).
uint64_t _mask(int64_t mb, int64_t me);
uint64_t _rotl64(uint64_t v, int64_t n);
uint64_t _rotl32(uint64_t v, int64_t n);
double _f(uint64_t bits);
uint64_t _b(double x);
// A double rounded to single precision (still held as a double).
double _single(double x);

using Region = std::tuple<int64_t, py::bytes>;         // (address, bytes)
using Pages = py::dict<int64_t, py::bytes>;            // {page number: bytes}

// {page number: bytes} for [(address, bytes)] -- an image prepared once and shared by every
// run of it (a run copies a page only when it writes to it).
Pages pages(const py::list<Region>& regions);

class Machine {
  public:
    py::dict<int64_t, py::bytearray> pages;            // pages this run has written or created
    Pages shared;                                      // pages it only reads so far
    py::list<uint64_t> r;
    py::list<uint64_t> f;                              // floating registers, as 64 raw bits
    int64_t lr = 0;
    uint64_t ctr = 0;
    int64_t cr = 0;
    int64_t ca = 0;
    int64_t pc = 0;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> writes;   // address -> (first old byte, last new byte)
    std::optional<int64_t> reserve;

    explicit Machine(const py::list<Region>& regions, const std::optional<Pages>& shared_ = std::nullopt);

    // -- memory
    void load(int64_t va, const py::bytes& data);
    // Python's _page(a, write): the page's bytes to change (a shared page is copied in first) ...
    std::string& _page(int64_t a);
    // ... and _page(a, False): the page's bytes to read (a shared page is not copied)
    const std::string& _page_read(int64_t a);
    uint64_t read(int64_t a, int64_t n);
    void write(int64_t a, int64_t n, uint64_t v);

    // -- condition register
    void set_cr(int64_t field, int64_t bits);
    // CR0 from a result, as a signed 64-bit number.
    void record(uint64_t v);
    void compare(int64_t field, int64_t a, int64_t b);
    void compare(int64_t field, uint64_t a, uint64_t b);
    int64_t bit(int64_t i);
};

// (the sum mod 2**64, the carry out)
std::tuple<uint64_t, int64_t> _add(Machine& m, uint64_t a, uint64_t b, int64_t carry = 0);

// Execute one instruction word at m.pc; set m.pc for the next.
void _step(Machine& m, int64_t w);

// Does a conditional branch with these BO and BI fields go? (decrements CTR when BO says so)
bool _taken(Machine& m, int64_t bo, int64_t bi);

struct LoadSpec {      // (size, signed, update)
    int64_t size;
    int64_t signed_;
    int64_t update;
};
struct StoreSpec {     // (size, update)
    int64_t size;
    int64_t update;
};
struct XLoadSpec {     // xo -> (size, signed, update, byte-reversed)
    int64_t size;
    int64_t signed_;
    int64_t update;
    int64_t swap;
};
struct XStoreSpec {    // xo -> (size, update, byte-reversed)
    int64_t size;
    int64_t update;
    int64_t swap;
};

extern const py::dict<int64_t, std::function<int64_t(int64_t, int64_t)>> _CR_OPS;
extern const py::dict<int64_t, LoadSpec> _LOADS;
extern const py::dict<int64_t, StoreSpec> _STORES;
extern const py::dict<int64_t, XLoadSpec> _XLOAD;
extern const py::dict<int64_t, XStoreSpec> _XSTORE;
extern const py::dict<int64_t, std::function<uint64_t(uint64_t, uint64_t)>> _LOGIC;

void _memory(Machine& m, int64_t op, int64_t w, int64_t rt, int64_t ra, uint64_t simm);
py::bytes _pack_single(double x);
void _extended(Machine& m, int64_t w, int64_t rt, int64_t ra, int64_t rb);
void _arithmetic(Machine& m, int64_t w, int64_t xo, int64_t rt, int64_t ra, int64_t rb, int64_t rc);
void _float(Machine& m, int64_t op, int64_t w, int64_t rt, int64_t ra, int64_t rb);
void _float_other(Machine& m, int64_t w, int64_t xo, int64_t rt, int64_t ra, int64_t rb);
int64_t _to_int(double x, int64_t bits, bool toward_zero);

// Contents for the memory pointer-shaped inputs point into (ARGS_BASE upward), so that what
// a function reads through a pointer is not always zero:
//   "pointers"  every word holds the address 0x100 past itself: any chain of pointers leads on
//   "counts"    words 1, 2, 3, ...
//   "text"      a run of letters ending in a zero every 24 bytes
py::bytes filled(const py::str& kind, int64_t size = 0x10000);

// What the last compare into a condition field compared: ("cmp", size, a, b, where a was loaded
// from or None, where b was)
struct Compare {
    int64_t size = 0;
    uint64_t a = 0;
    uint64_t b = 0;
    std::optional<int64_t> a_from;
    std::optional<int64_t> b_from;
};
// (address, taken, compare)
using BranchEvent = std::tuple<int64_t, bool, std::optional<Compare>>;

// The branch log of run(branches=True), for an input generator (verify/guided.py): each
// conditional branch -- (address, taken, compare) -- where compare is what the last compare
// into the condition field it tests compared, or None when that field was last set some other
// way. A register's "where" is the memory it was last loaded from, if nothing has written it since.
class _Branches {
  public:
    static constexpr int64_t CAP = 4096;
    py::list<BranchEvent> events;
    py::dict<int64_t, std::optional<Compare>> field;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> loaded;

    _Branches() = default;
    void step(Machine& m, int64_t pc, int64_t w);
    std::optional<int64_t> _where(int64_t reg, int64_t size);
};

// emulate/shadow.py's ShadowPPC, kept up beside the run (the path explorer's): stepped before each
// instruction, told of each call out. (No port of it yet: this is the interface run() needs.)
class Shadow {
  public:
    virtual ~Shadow() = default;
    virtual void step(Machine& m, int64_t pc) = 0;
    virtual void after_call(Machine& m, bool known, uint64_t ret) = 0;
};

// run's keywords after `args`
struct RunOptions {
    // {address: name}: functions outside what was given: a call to one is recorded with its eight
    // argument registers and returns `stub_ret`
    std::optional<py::dict<int64_t, py::str>> externs;
    std::optional<std::tuple<int64_t, int64_t>> span;   // (lo, hi): the addresses whose execution is recorded
    int64_t stub_ret = 0;
    int64_t steps = MAX_STEPS;
    py::list<double> fargs;                             // floats, in f1..
    std::optional<Pages> shared;                        // memory prepared with pages()
    std::optional<py::str> fill;                        // what the memory behind pointer-shaped inputs holds
    bool branches = false;                              // log each conditional branch inside `span`
    // (low, high): where code the image calls may be missing -- a call that lands on nothing
    // there is a call out, recorded as "@<address>"
    std::optional<std::tuple<int64_t, int64_t>> absent;
    Shadow* shadow = nullptr;                           // None
};

// run's record: {exit, truncated, calls, ret, fret, steps, path, pc, writes, saved, machine[, branches]}
struct Out {
    py::str exit = py::str("none");
    bool truncated = false;
    py::list<std::tuple<py::str, py::list<uint64_t>>> calls;
    uint64_t ret = 0;
    uint64_t fret = 0;
    int64_t steps = 0;
    py::set<int64_t> path;
    int64_t pc = 0;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> writes;
    bool saved = false;
    py::ref<Machine> machine;
    std::optional<py::list<BranchEvent>> branches;      // present when run(branches=True)
};

// Run the function at `entry`. `regions` is [(address, bytes)] to place in memory; `args`
// go in r3..r10 and `fargs` (floats) in f1..; `externs` {address: name} are functions outside
// what was given: a call to one is recorded with its eight argument registers and returns
// `stub_ret`. `shared` is memory prepared with pages(), used instead of (or under) `regions`;
// `fill` names what the memory behind pointer-shaped inputs holds (see filled()). With
// `branches`, each conditional branch inside `span` is logged in out.branches (see _Branches).
// `absent` (low, high): where code the image calls may be missing -- a call that lands on nothing
// there is a call out, recorded as "@<address>" (a partial image calls into the rest of its title).
// `shadow`: an emulate/shadow.py ShadowPPC kept up beside the run (the path explorer's): stepped
// before each instruction, told of each call out. The run is the same run with it or without.
// (A page of `regions` is made from `shared` alone: one that lies in the `fill` holds only what
// the regions put there.)
Out run(const py::list<Region>& regions, int64_t entry, const py::list<uint64_t>& args = py::list<uint64_t>(),
        const RunOptions& kw = RunOptions());

// run's record as the one run record (emulate/run.py): r3 and f1 the results, each call out
// with its eight argument registers, the bytes it left changed (outside the stack), r14-r31 and
// the stack pointer as they went in and came out, the addresses of `span` that ran.
emu_run::Run record(const Out& out);

// The platform's runner (platforms/xbox360.py emulator.run): run(...) as a Run.
emu_run::Run recorded(const py::list<Region>& regions, int64_t entry,
                      const py::list<uint64_t>& args = py::list<uint64_t>(), const RunOptions& kw = RunOptions());

}  // namespace codebreaker::emulate::powerpc
