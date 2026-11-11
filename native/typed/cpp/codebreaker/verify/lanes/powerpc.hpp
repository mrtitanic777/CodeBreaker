// codebreaker/verify/lanes/powerpc.hpp -- typed C++ of codebreaker.verify.lanes.powerpc.
//
// powerpc.py -- what is particular to reconstructing a PowerPC image.
//
// One of the kinds reconstruct.py works with (see there for what a kind provides). The image is
// decompiled and made compilable by decompile/c_text.py, every function is compiled with a
// PowerPC compiler and one image linked by build/functions.py, and each function is run beside
// the original in the PowerPC interpreter (emulate/powerpc.py).
//
// Both sides get the same eight argument registers. The rebuilt image is placed beside the
// original in the same memory, so a global is the same storage for both and a call through a
// pointer the original holds still lands in the original. Compared: how the run ends, the calls
// to functions outside the image in order, the result in r3 (not for a function that returns
// nothing), the memory left changed outside the stack, and whether r14-r31 and the stack pointer
// were handed back.
//
// How many arguments a function outside the image takes is not stated anywhere in a PowerPC
// image. Where every call to it in the module passes the same number, that many are compared;
// otherwise the call is compared by name only and a match involving it is marked weak. An
// argument that points into either image's data is compared by what it points at, since the
// rebuilt image keeps its own copy of a constant.
//
// The interpreter, the convention, the compiler's options and the build of the rebuilt image are
// the platform's (codebreaker/platforms/xbox360.py): its emulator, convention, compiler, packager.
//
// Typing notes:
//   * The Python module IS a Kind. Here the free functions below are the module's and `PowerPCKind` is the reconstruct::Kind
//     that forwards to them (register it under "powerpc"; kind()).
//   * `P = platforms.of("powerpc")` is not declared (the platforms registry has no typed port): its slots are what they hold:
//     ARGS() = convention.arguments (8), the emulator emulate::powerpc::recorded, the compiler's options
//     build::functions::CFLAGS_POWERPC, the packager build::functions::build_powerpc.
//   * An input (the Python's dict {args, stub_ret, fill, mem_init}) is `Vector`: a compare::Input (args, stub_ret) plus `fill`
//     and `mem_init`. Arguments are 64-bit registers held as int64_t (the same bits). verify::guided takes guided::Input: to_guided()
//     / from_guided() convert (`fill` travels in `extra["fill"]` as 1 pointers / 2 counts / 3 text).
//   * What prepare() hands reconstruct is reconstruct::Prepared whose `lane` is a std::shared_ptr<decompile::c_text::ModulePPC> (the
//     Python's m: key, all_keys and sig are the Prepared's own members).
//   * An item of build() (the Python's tuple (key, address, span, instruction count, rebuilt address, returns nothing)) is `Item`;
//     the judge's context is `Context`.  Both travel to a worker process as text (encode / decode).
//   * `_DIRECTED` and `_SHARED` are the Python's module-level caches (private).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/emulate/powerpc.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/verify/compare.hpp"
#include "codebreaker/verify/concolic.hpp"
#include "codebreaker/verify/guided.hpp"
#include "codebreaker/verify/lane_common.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::powerpc {

extern const py::str KIND;                  // "powerpc"
inline constexpr bool PER_FUNCTION = true;  // a verdict holds while the function and what it reaches are unchanged
// 4: branch-guided inputs (verify/guided.py); calls into absent code;
// 5: those found path by path (explore_vectors: ShadowPPC, verify/concolic.py), a verdict's limits saying paths=N / paths-all
inline constexpr int64_t REV = 5;
int64_t ARGS();                             // P.convention.arguments: r3-r10
inline constexpr int64_t M32 = 0xFFFFFFFF;
inline constexpr int64_t _OFFSET = 0x01000000;   // the rebuilt image sits this far above the original
// A question over 64-bit registers is a bigger one for the solver than the 16-bit lane's: those it cannot answer cost it four times
// as long before it gives up. A quarter of concolic.SOLVER_STEPS -- still z3's own steps, the same on any machine.
inline constexpr int64_t SOLVER_STEPS = 500000;

struct Vector : compare::Input {
    std::optional<py::str> fill;                                         // "pointers" | "counts" | "text"
    std::optional<py::list<std::tuple<int64_t, py::bytes>>> mem_init;
};

// The inputs every function is run on: eight argument registers and what calls outside the image return. Memory an argument points
// at reads as zeros unless the image put something there, on both sides alike.
py::list<Vector> vectors();

// Inputs aimed at what the function's own code tests: the constants it compares values with, given in each argument register, as what
// calls outside return, and as what the memory behind a pointer holds.  (Remembered per span, as the Python's _DIRECTED.)
py::list<Vector> directed(const formats::powerpc_image::Image& image, const std::tuple<int64_t, int64_t>& span, int64_t most = 6);

guided::Input to_guided(const Vector& v);
Vector from_guided(const guided::Input& v);

using Externs = py::dict<int64_t, py::str>;
using Span = std::tuple<int64_t, int64_t>;
using Pages = emulate::powerpc::Pages;

// Inputs that take branches of the original no seed took (verify/guided.py).
py::list<Vector> guided_vectors(const Pages& shared, int64_t va, const Span& span, const Externs& externs, const py::list<Vector>& seeds,
                                const std::optional<Span>& absent = std::nullopt);

// What the exploration is left in (the Python's `found` dict, ["got"]).
struct Found {
    std::shared_ptr<concolic::Exploration<Vector>> got;
};

// The directed inputs, then those found path by path (or guessed).
py::list<Vector> directed_and_guided(const py::str& module_path, const Pages& shared, int64_t va, const Span& span, const Externs& externs,
                                     const std::optional<Span>& absent = std::nullopt, Found* found = nullptr);

// The lift mode of the platform's processor (emulate/shadow.py mode: its cpu slot).
std::optional<py::str> _mode();
bool concolic_ready();

// Input `v` with the named inputs (shadow.vars) set to `values`: an argument register, what a call out returns, bytes of memory -- behind
// the pointers given as whole pages, the input's `fill` under them.  nullopt: nothing changed.
std::optional<Vector> _derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names);

// [(low, high)] of the image's writable sections: its globals, which an input may set.
py::list<Span> _writable(const py::str& module_path);

// (inputs, exploration): inputs taking paths of the original no seed took, found path by path.
std::tuple<py::list<Vector>, std::shared_ptr<concolic::Exploration<Vector>>> explore_vectors(
    const Pages& shared, int64_t va, const Span& span, const Externs& externs, const py::list<Vector>& seeds,
    const std::optional<Span>& absent = std::nullopt, const py::list<Span>& data = py::list<Span>());

// One argument of a recorded call, written as what it is rather than its bit pattern where the two builds would legitimately differ.
py::str _argument(emulate::powerpc::Machine& machine, int64_t v, const py::list<Span>& images);

// What one run of one function did, in a form two builds can be compared on.
compare::Fingerprint fingerprint(const Pages& shared, int64_t entry, const Vector& v, const Externs& externs, const py::dict<py::str, int64_t>& argc,
                                 const py::list<Span>& images, const std::optional<Span>& span = std::nullopt, bool novalue = false);

// (pages of the original alone, pages of original + rebuilt, [(lo, hi)] of each image); the last pair asked for is kept
std::tuple<Pages, Pages, py::list<Span>> _memory(const py::str& module_path, const py::str& dll);

// What judge() is handed once for every function.
struct Context {
    py::str module_path;
    py::str dll;
    Externs ext_original, ext_rebuilt;
    py::dict<py::str, int64_t> argc;
};
// One function to judge: (key, address, span, instruction count, rebuilt address, returns nothing)
struct Item {
    py::str key;
    int64_t va = 0;
    Span span;
    int64_t insns = 0;
    int64_t rebuilt_va = 0;
    bool novalue = false;
};

// Worker: judge one function.
reconstruct::Verdict judge(const Context& context, const Item& item);

// (digest, settings) of everything other than the source that a verdict depends on.
std::tuple<py::str, py::str> profile(const py::str& module_path);

// The decompiled module made compilable: reconstruct::Prepared with the ModulePPC in `lane`.
reconstruct::Prepared prepare(const py::str& module_path, const py::str& work);

// -> (context for judge, {name: item}, {name: why it is not in the build})
reconstruct::Built build(const reconstruct::Prepared& m, const py::str& module_path, const py::str& work);

const std::shared_ptr<reconstruct::Kind>& kind();
void register_lane();

}  // namespace codebreaker::verify::lanes::powerpc
