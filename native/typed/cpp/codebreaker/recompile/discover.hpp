// codebreaker/recompile/discover.hpp -- typed C++ port of codebreaker.recompile.discover.
//
// discover.py -- every function of an Xbox 360 image, for the recompiler.
//
//     auto [funcs, sources, img] = discover::discover(path, true, extra);
//     funcs       {address: size, 0 when unknown}
//     sources     how many came from each place: pdata, entry, calls, pointers, constants, seed
//     img         the image (formats/powerpc_image.Image)
//
//     python -m codebreaker.recompile.discover <image or .xex> [--list]
//
// A recompiled program calls a function only if it was found: a missed one is a call that does
// nothing at run time, which is how a recompiled title crashes. So more is found here than the
// decompiler needs (formats/powerpc_image.py finds the .pdata functions, the entry point and every
// `bl` target):
//
//   * pointers -- methods reached only through a pointer (virtual tables, function tables): every
//     big-endian word of the data sections that is a four-byte-aligned code address, not inside a
//     known function, where a function's first instruction plausibly stands;
//   * constants -- the same test for addresses the code builds (a `lis` and then an `addi` or
//     `ori` on that register): the CRT hands its own leaf functions, which have no .pdata, to
//     the run time that way (FlsAlloc's stand-in, the callbacks it registers);
//   * seeds -- addresses the caller already knows are called indirectly (from a run), whose own
//     callees are then followed like any other's.
//
// Typing notes:
//   * `extra` (any iterable of addresses, None by default) is an optional list.
//   * from_pointers / from_constants return the Python's set; it is iterated in the order the runtime's set keeps
//     (Python's is its hash order), which only changes the order of discover()'s dict, never its contents.
//   * main(argv) is the Python's main with `argv` the arguments after the program's name and what it prints
//     written to `out`; the __main__ block (sys.exit(main(sys.argv[1:]))) is the caller's.
#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>

#include "codebreaker/formats/powerpc_image.hpp"
#include "py.hpp"

namespace codebreaker::recompile::discover {

inline constexpr int64_t WORD = 4;

// the instructions a function's first plausibly is (_PROLOGUE)
const py::set<py::str>& _PROLOGUE();
// bclr, bcctr, rfid
const py::set<int64_t>& _XL_ENDERS();

// the image of a path: a .xex's basefile, or the PowerPC PE itself
py::ref<formats::powerpc_image::Image> _image(const py::str& path);

// A function's first instruction plausibly stands at `a`: it is a prologue-like instruction, or any instruction
// (not a data word) right after the end of a function (blr, bctr, a plain branch, padding).
bool _plausible_start(const py::bytes& code, int64_t lo, int64_t a);

// Function starts found as pointers in the data sections (see the module's notes).
py::set<int64_t> from_pointers(const formats::powerpc_image::Image& img, const py::dict<int64_t, int64_t>& known);

// Function starts found as addresses built in the code: `lis rD, hi` and then `addi rX, rD, lo` / `ori rX, rD, lo`
// (raw opcode bits; see the module's notes).
py::set<int64_t> from_constants(const formats::powerpc_image::Image& img, const py::dict<int64_t, int64_t>& known);

using Result = std::tuple<py::dict<int64_t, int64_t>, py::dict<py::str, int64_t>, py::ref<formats::powerpc_image::Image>>;

Result discover(const py::str& path, bool pointer_scan = true, const std::optional<py::list<int64_t>>& extra = std::nullopt);

int64_t main(const py::list<py::str>& argv, std::ostream& out = std::cout);

}  // namespace codebreaker::recompile::discover
