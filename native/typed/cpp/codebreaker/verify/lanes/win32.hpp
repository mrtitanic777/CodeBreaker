// codebreaker/verify/lanes/win32.hpp -- interface of codebreaker.verify.lanes.win32 (typed C++; implementation pending).
//
// win32.py -- what is particular to reconstructing a 32-bit Windows module.
//
// One of the kinds reconstruct.py works with (see reconstruct.hpp for what a kind provides). Here: the module is
// decompiled by decompile/decompiler.py, built by build/functions.py, and each function is run beside the original in
// the 32-bit emulator (emulate/emulator.py) and compared by verify/compare.py (calls and their arguments, result,
// memory changed, stack, saved registers, coverage).
//
// What runs a function, how it is called, its compiler's options and what builds the rebuild are the platform's
// (codebreaker/platforms/win32.py): its emulator, convention, compiler and packager.
//
// Typing notes:
//   * The Python module IS a Kind (reconstruct.py duck-types it by attribute). Here the free functions below are the
//     module's, and `Win32Kind` is the reconstruct::Kind that forwards to them (register it under "pe").
//   * `P = platforms.of("pe")` (the six platform slots) is not declared: the platforms registry has no typed port; the
//     implementation reaches the slots as the platforms port provides them. ARITY (P.convention.arguments, the
//     arguments every function is given) is a function here, read when first used.
//   * An input vector (the Python's dict {args, mem_init, stub_ret, ecx, edx, given, room, sentinels}) is `Vector`:
//     a compare::Input (args, stub_ret) plus the keys only this lane adds. verify::guided / lane_common::guided take
//     guided::Input (args, mem_init, stub_ret, `extra`): to_guided() / from_guided() convert (ecx, edx, given, room,
//     sentinels travel in `extra`). A key absent in the dict is std::nullopt here.
//   * What prepare() hands reconstruct is reconstruct::Prepared whose `lane` is a std::shared_ptr<decompile::c_text::Module32>
//     (the Python's m: the Made with imports, in_ecx, in_edx ...); `va` (name -> address) is the inverse of its by_va.
//   * `shape` (in_ecx / in_ecx_edx / identity) is a std::function<Vector(const Vector&)>; empty means the identity.
//   * An item of build() (the Python's tuple (key, address, dll, entry, regs)) is `Item`; the judge's context is the
//     module path.  regs: 2 for a function whose first two arguments are in ECX and EDX, 1 for ECX, else 0.
//   * concolic::Vars carries `where` as an integer: for a register input (kind "reg") the Python's where is the register's name;
//     here ECX is 1 and EDX is 2 (what _derive reads back; the explorer never looks at `where`).
//   * `found` of directed_and_guided() (a dict the exploration is left in, ["got"]) is `Found`.
//   * The ctypes-free emulator is called through emulate::emulator (TraceOptions, trace_function / recorded).
//   * Module-level caches (_PROFILES, _IMAGE_SPAN, _INSN_COUNT, _THUNKS, _HARVEST, _FILL, _DATA) are private.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/emulate/emulator.hpp"
#include "codebreaker/emulate/native_trace.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/verify/compare.hpp"
#include "codebreaker/verify/concolic.hpp"
#include "codebreaker/verify/guided.hpp"
#include "codebreaker/verify/lane_common.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::win32 {

extern const py::str KIND;             // "pe"
inline constexpr bool PER_FUNCTION = true;   // a verdict holds while the function and what it reaches are unchanged

// the arguments every function is given (P.convention.arguments; its real count is not known)
int64_t ARITY();

inline constexpr int64_t PIPE_REV = 48;      // what a verdict means (see the comment in the Python for the history)
inline constexpr int64_t ROOM = 25 * emulate::emulator::_MAX_INSNS;   // the steps a rebuild is given then
inline constexpr int64_t MAX_SUPPORT = 1500; // out-of-batch callee bodies pulled in per batch
inline constexpr int64_t _MAX_CALLS = 64;
inline constexpr int64_t MAX_DIRECTED = 16;  // directed inputs kept per body
inline constexpr int64_t _HARVEST_DEPTH = 2; // how far into callees to look for what branches test
inline constexpr int64_t _HARVEST_FUNCS = 12;
inline constexpr int64_t _MAX_REACH = 6000;
inline constexpr int64_t _FILL_MAX = 0x40000;

// verdicts::GOOD: a verdict that is a match (weak or not)
const py::list<py::str>& GOOD();

// The provisioned environment: a region of pointers that all resolve to a safe RET, inside the tracer's scratch window.
inline constexpr int64_t _OBJ = 0x206000, _SAFE = 0x207800;
inline constexpr std::array<int64_t, 2> _PADS = {_SAFE, _SAFE + 0x1000};
extern const bool _PLAIN;              // lane_common::PLAIN: one instruction at a time on a fresh machine (CB_PLAIN)

// The oracle's stack (and the RET landing pad just under it) at the top of the scratch region.
inline constexpr int64_t _STACK_LO = emulate::emulator::_STACK_BASE;
inline constexpr int64_t _STACK_HI = emulate::emulator::_STACK_BASE + emulate::emulator::_STACK_SIZE;

// The saved registers hold these instead when asking which bits of a return value are leftovers.
extern const std::array<int64_t, 4> _ALT_SENTINELS;   // SAVED_SENTINELS xor 0xFFFFFFFF

// ---- inputs ------------------------------------------------------------------------------------------------------
// An input vector: {args, mem_init, stub_ret, ecx, edx, given, room, sentinels}.
struct Vector : compare::Input {
    std::optional<py::list<std::tuple<int64_t, py::bytes>>> mem_init;   // [(address, bytes)] written before the run
    std::optional<int64_t> ecx, edx;                    // the first (two) argument(s) of a member function
    std::optional<py::list<int64_t>> given;             // the arguments as the calls name them (before ecx/edx took some)
    std::optional<int64_t> room;                        // a larger instruction cap for a rebuild stopped at the limit
    std::optional<py::list<int64_t>> sentinels;         // the registers' values at entry (4: EBX ESI EDI EBP)
};
using Shape = std::function<Vector(const Vector&)>;
guided::Input to_guided(const Vector& v);
Vector from_guided(const guided::Input& v);

// The same input for a function that takes its first argument in ECX (a member function's object): the first value
// there, the rest on the stack.
Vector in_ecx(const Vector& v);
// ... and for one that takes its first two in ECX and EDX.
Vector in_ecx_edx(const Vector& v);

// A pretend object whose every field is a pointer to a landing pad of its own.
py::list<std::tuple<int64_t, py::bytes>> _prov_meminit();

// Input vectors to exercise a function past the immediate null-fault; `lane` is "pe" (or "dos"/"ne" for the others).
py::list<Vector> _vectors(int64_t arity, const py::str& lane);
// More inputs, run only for a body that already matched on every basic vector.
py::list<Vector> _confirm_vectors(int64_t arity, const py::str& lane);

// ---- lane ----------------------------------------------------------------------------------------------------------
// (digest, settings): everything OTHER than a body's own source that a verdict depends on (settings is JSON text).
std::tuple<py::str, py::str> settings_profile(const py::str& lane);
std::tuple<py::str, py::str> profile(const py::str& module_path);

// The paths part of the profile: [ready, PATH_BUDGET, MAX_PATHS, GUIDED_REST, the lifter's stamp or ""].
struct PathsProfile {
    bool ready = false;
    int64_t path_budget = 0, max_paths = 0, guided_rest = 0;
    std::optional<std::tuple<py::str, py::str>> stamp;   // pcode::stamp(mode) when ready, else ""
};
PathsProfile _paths_profile();

// 32-bit PE. Bodies are compiled together in batches, so a body that calls a batch-mate is verified against that
// mate's real reconstruction rather than a stub.
class PELane {
  public:
    py::str name = py::str("pe");
    py::list<int64_t> functions(const py::str& path) const;   // sorted(pe_functions.carve(path).funcs)
    py::str key(int64_t va) const;                            // "%x"
};
extern py::dict<py::str, PELane> LANES;   // {"pe": PELane()}

py::list<int64_t> functions(const py::str& lane, const py::str& path);

// The decompiled module: decompile(module_path, addresses, work), with key/va/all_keys/sig added.
reconstruct::Prepared prepare(const py::str& module_path, const py::str& work);

// What build() gives judge() for one function: (key, address, dll, entry, regs).
struct Item {
    py::str key;
    int64_t va = 0;
    py::str dll;
    int64_t entry = 0;
    int64_t regs = 0;
};
// -> (context for judge: the module path, {name: item}, {name: why it is not in the build})
struct Built32 {
    py::str module_path;
    py::dict<py::str, Item> items;
    py::dict<py::str, py::str> dropped;
};
Built32 build(const reconstruct::Prepared& m, const py::str& module_path, const py::str& work);

// Worker: judge one function. -> (verdict, reason, limits, detail)
reconstruct::Verdict judge(const py::str& module_path, const Item& item);

// The reconstruct::Kind for "pe" (forwards to the functions above).
class Win32Kind final : public reconstruct::Kind {
  public:
    py::str KIND() const override;
    std::tuple<py::str, py::str> profile(const py::str& module_path) const override;
    reconstruct::Prepared prepare(const py::str& module_path, const py::str& work) const override;
    reconstruct::Built build(const reconstruct::Prepared& m, const py::str& module_path, const py::str& work) const override;
    reconstruct::Verdict judge(const reconstruct::Context& context, const reconstruct::Item& item) const override;
    bool PER_FUNCTION() const override { return true; }
};

// The one Win32Kind, and its registration under "pe" (as win16::register_lane registers "ne").
const std::shared_ptr<reconstruct::Kind>& kind();
void register_lane();

// ---- the fingerprint -----------------------------------------------------------------------------------------------
// compiler/CRT-runtime imports the toolchain emits are ignored (not the body's own logic).
// Canonicalise an imported-call name so the two builds' import-table spellings of the same import compare equal.
py::str _norm_dep(const py::str& d);
// Toolchain/thunk imports that differ by CODEGEN (OpenWatcom vs MS), not by the body's logic.
bool _is_noise(const std::optional<py::str>& api);

// Argument count of an imported function (apiarity.argc, else the small builtin table); None = unknown.
std::optional<int64_t> _argc(const py::str& dll, const py::str& name);

// The imported-call part of a fingerprint: every call in order, its name as both builds spell it, toolchain noise left
// out, each argument as lane_common::argument writes it. `own`: the address range of a rebuilt image (or None).
py::list<py::str> _call_seq(const py::list<lane_common::Called>& calls, const py::list<int64_t>& ins,
                            const std::optional<std::tuple<int64_t, int64_t>>& own = std::nullopt);

// The fingerprint cache key (r<REV>:<sha>:<digest of the run's inputs>).
py::str _fp_key(const py::str& sha, int64_t func, const Vector& spec);

// (lo, hi) addresses of a PE image as loaded; (0, 0) when it cannot be read.
std::tuple<int64_t, int64_t> _image_span(const py::str& path);

// What the function left in memory outside its own frame: a digest of the final value of every byte it wrote
// (lane_common::left). `own`: the address range of a REBUILT image. None = too many writes to record them all.
std::optional<py::str> _effects(const emulate::run::Run& r, const std::optional<std::tuple<int64_t, int64_t>>& own);

// (own, reach): offsets of the function's own instructions that ran, and the addresses of every instruction that ran
// (sorted, at most _MAX_REACH). Reads func_va / func_end_va from the run's raw trace.
std::tuple<py::list<int64_t>, py::list<int64_t>> _coverage(const emulate::run::Run& r);

// How many instructions the function at `func` has, or 0: those reachable from its entry through its own jumps.
int64_t insn_count(const py::str& path, int64_t func);
int64_t _reachable_insns(const py::bytes& code, int64_t start);

// {address: ("THUNK", name)} and {name: argument count or None} for the stubs a flat thunk's compiler generated.
std::tuple<py::dict<int64_t, std::tuple<py::str, py::str>>, py::dict<py::str, std::optional<int64_t>>> _thunk_stubs(
    const py::str& original);

// How a run enters the stubs: as imports, each with its argument count (TraceOptions::stub_at and sig_resolver).
struct StubOptions {
    std::optional<py::dict<int64_t, std::tuple<py::str, py::str>>> stub_at;   // None when there are no stubs
    emulate::native_trace::SigResolver sig_resolver;
};
StubOptions _stub_options(const py::str& original);

// Fingerprint one run. For a REBUILT image pass `rebuilt` = the path of the original: its own statics are then left
// out of the comparison and the original's code is mapped beside it. Kept across runs (lane_common::remembered).
compare::Fingerprint _fp_pe(const py::str& path, int64_t func, const Vector& spec,
                            const std::optional<py::str>& rebuilt = std::nullopt);
// One uncached run as a fingerprint (an error is a fingerprint with `err` set; an ImportError goes on).
compare::Fingerprint _trace_pe(const py::str& path, int64_t func, const Vector& spec, const std::optional<py::str>& rebuilt);

// ---- inputs aimed at branches --------------------------------------------------------------------------------------
// (constants, globals) read off the function's own instructions: the immediates it compares or masks with, and the
// absolute data addresses it reads.
std::tuple<py::list<int64_t>, py::list<int64_t>> _harvest(const py::str& path, int64_t func);

// mem_init that gives every word of the original's writable sections a value of its own (a landing pad's address).
py::list<std::tuple<int64_t, py::bytes>> _data_filled(const py::str& path);

// Inputs chosen for THIS function: a candidate is kept only if the ORIGINAL then runs instructions no earlier input
// reached; at most MAX_DIRECTED. `shape` (a member function: the first value goes in ECX) may be empty.
py::list<Vector> directed_vectors(const py::str& path, int64_t func, const py::set<int64_t>& covered, int64_t arity = 4,
                                  const py::set<int64_t>& reached = py::set<int64_t>(), const Shape& shape = nullptr);

// The original run on `spec` with its branches logged, or None when the run is not an observation.
std::optional<guided::Observation> _branch_run(const py::str& path, int64_t func, const Vector& spec);

// Inputs that take branches of the original no seed took (verify/guided.py).
py::list<Vector> guided_vectors(const py::str& path, int64_t func, const py::list<Vector>& seeds, const Shape& shape = nullptr);

// The exploration a judge leaves for its verdict's limits (found["got"]).
struct Found {
    std::shared_ptr<concolic::Exploration<Vector>> got;
};
// The directed inputs, then those found path by path (or guessed) seeded with every input run so far.
py::list<Vector> directed_and_guided(const py::str& path, int64_t func, const py::set<int64_t>& covered,
                                     const py::set<int64_t>& reached, const Shape& shape = nullptr, Found* found = nullptr);

// The lift mode of the platform's processor (emulate/shadow.py mode: its cpu slot).
std::optional<py::str> _mode();
// The lifter is there and neither CB_NO_GUIDED nor CB_NO_CONCOLIC is set.
bool concolic_ready();
// [(low, high)] of the module's writable data sections, as loaded.
py::list<std::tuple<int64_t, int64_t>> _writable(const py::str& path);

// Input `v` (as the run is given it: shaped) with the named inputs (shadow.vars) set to `values`; None when nothing
// changes.
std::optional<Vector> _derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names, const Shape& shape);

// (inputs, exploration): inputs taking paths of the original no seed took, found path by path (verify/concolic.py;
// lane_common::explored), and how far the exploration got (null where the function could not be resolved).
std::tuple<py::list<Vector>, std::shared_ptr<concolic::Exploration<Vector>>> explore_vectors(
    const py::str& path, int64_t func, const py::list<Vector>& seeds, const Shape& shape = nullptr);

// Bits of the original's return value that are not a result at all (0, 0xFFFF0000, 0xFFFFFF00 or 0xFFFFFFFF): the
// original run again with the saved registers holding the opposite bits, `fp_retail(v)` being its fingerprint.
int64_t undefined_ret_bits(const std::function<compare::Fingerprint(const Vector&)>& fp_retail, const Vector& v,
                           const compare::Fingerprint& a);

}  // namespace codebreaker::verify::lanes::win32
