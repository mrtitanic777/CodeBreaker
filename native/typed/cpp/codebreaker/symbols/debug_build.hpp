// codebreaker/symbols/debug_build.hpp -- typed C++ port of codebreaker.symbols.debug_build.
//
// Recover real internal function names for a shipped NE module by matching it, function by function, against a
// *debug build of the same version* whose linker .MAP names every function (debug builds declare internals
// PUBLIC).
//
// The retail export table names only ~25% of a system module's functions; the rest carve as anonymous
// ``sub_XXXX``. A debug build of the identical Windows version carries the true names, but at *different
// offsets* (debug adds assertion / RIP code and shifts everything). So we can't align by address -- we align by
// an offset-invariant byte signature:
//
//     norm_sig(): the objdump text per instruction with only the OFFSET-dependent operands masked --
//     branch/call targets, absolute ``[0xNNNN]`` memory, and far ``seg:off`` pointers. IMMEDIATES stay, because
//     they are part of a function's identity; masking them (an earlier attempt) let functions with the same
//     instruction skeleton but different constants collide. A signature match is therefore byte-identity modulo
//     relocation: the two functions are provably the same code, and the debug build's name for it is exact.
//
// Three guards keep the result at zero false positives:
//
//   1. ambiguity drop -- a signature that two *different* debug names share is discarded (can't tell which name
//      is right).
//   2. export validation -- the retail export table is a labelled test set: any signature a retail EXPORT
//      already owns under a different name is discarded as collision-prone (this is what catches e.g. a
//      2-instruction predicate matching the wrong short function). After this filter, every known export in the
//      map agrees with its own signature -- the proof the method is sound.
//   3. min length + uniqueness -- functions under MIN_LEN instructions are too skeleton-like to trust; and a
//      real name claimed by >1 retail offset is dropped entirely (a symbol lives at one address, so a double
//      match is an internal-internal collision, and duplicate labels would break emission).
//
// Only a module with BOTH a debug binary and its .MAP can be recovered this way. The output ``names.json``
// ("seg:off" -> NAME) feeds trace.Seg(name_override=...) which renames the anonymous functions in the emitted
// idiomatic source -- a pure label rename, so byte-exactness is untouched.
//
// Typing notes:
//   * a signature is a tuple of str: Sig, a py::list<py::str> (hashable here: the hasher is specialized below).
//   * Seg keeps a reference to its NEFile, so the NEFiles live as long as the Segs of a function.
//   * json.load's json.JSONDecodeError (a ValueError) is JSONDecodeError here, thrown for the text the JSON reader refuses.
//   * load_override's values must be JSON strings (a name); the Python would keep any JSON value there, and a
//     number or null is a TypeError here.
//   * not ported: the `__name__ == "__main__"` block (it recovers KRNL386 from the Windows 3.1 retail and the
//     debug SDK build and writes state/idiomatic/krnl386/names.json): the caller does that with recover().
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/source/ne_asm/segment.hpp"
#include "py.hpp"

namespace py {
// a signature (a tuple of str) as a dict key
template <> struct hasher<py::list<py::str>> {
    size_t operator()(const py::list<py::str>& l) const {
        size_t h = 0x345678;
        for (const py::str& s : l) h = h * 1000003 ^ hasher<py::str>()(s);
        return h;
    }
};
}  // namespace py

namespace codebreaker::symbols::debug_build {

inline constexpr int64_t MIN_LEN = 5;   // functions under 5 instructions share skeletons -> collision-prone

extern const py::str _BR;   // the branch mnemonics whose target is masked

using Sig = py::list<py::str>;

// json.JSONDecodeError (a ValueError that names itself so)
struct JSONDecodeError : py::ValueError {
    explicit JSONDecodeError(const std::string& m) : py::ValueError(m) {}
    const char* type() const override { return "JSONDecodeError"; }
};

// Offset-invariant signature of the function at `start`: one entry per instruction, OFFSET-dependent operands
// masked, immediates kept.
Sig norm_sig(const source::ne_asm::segment::Seg& sg, int64_t start);

// {signature -> NAME} for every uniquely-signatured named function in a debug build (its .MAP names them;
// ambiguous signatures are dropped).
py::dict<Sig, py::str> build_debug_index(const py::str& debug_exe, const py::str& debug_map,
                                         const py::dict<py::str, py::str>& ordinals);

// ({"seg:off" -> NAME}: the exactly-matching real names for the anonymous internal functions of `retail_exe`,
// with all three false-positive guards applied, (agree, disagree) over the known exports).
std::tuple<py::dict<py::str, py::str>, std::tuple<int64_t, int64_t>> match_module(
    const py::str& retail_exe, const py::dict<Sig, py::str>& sig2name, const py::dict<py::str, py::str>& ordinals,
    int64_t min_len = MIN_LEN);

// names.json ("seg:off" -> NAME)  ->  {seg:int -> {off:int -> NAME}} for trace.Seg(name_override=...).
py::dict<int64_t, py::dict<int64_t, py::str>> load_override(const py::str& names_json);

// End-to-end: build the debug index, match, optionally write names.json.
std::tuple<py::dict<py::str, py::str>, std::tuple<int64_t, int64_t>> recover(
    const py::str& retail_exe, const py::str& debug_exe, const py::str& debug_map, const py::dict<py::str, py::str>& ordinals,
    const std::optional<py::str>& out_json = std::nullopt);

}  // namespace codebreaker::symbols::debug_build
