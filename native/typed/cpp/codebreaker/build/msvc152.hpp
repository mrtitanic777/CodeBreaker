// codebreaker/build/msvc152.hpp -- typed C++ port of codebreaker.build.msvc152.
//
// cc16.py -- compile 16-bit C with the PERIOD compiler (MSVC 1.52c) under DOSBox-X.
//
// The foundation of byte-exact C matching (matching decompilation): to prove a reconstructed C function is
// right, compile it with the SAME compiler+flags that built retail Win98 and check the emitted bytes against the
// shipped bytes. That requires the real compiler -- Microsoft Visual C++ 1.52c, the Win16 (MS C8) toolchain --
// which is a 16-bit DOS program (no NTVDM on x64), so it runs under the vendored DOSBox-X, exactly like
// `masmlift`/`link16` drive MASM.
//
// This module just RUNS the compiler: given C source (+ flags), it produces the .OBJ and the `/Fc` assembly
// listing. Extracting the code bytes from the OBJ, diffing against the target, and the permuter that searches
// flags/source for a byte-match are the layers built on top (see docs / COMPILER_RE.md's codegen oracle). First
// job: confirm MSVC 1.52c runs here and emits code.
//
//     python cc16.py --selftest              # compile a trivial fn, show the asm
//     python cc16.py foo.c --flags "/AM /Ox" # compile foo.c, dump obj + listing
//
// Typing notes:
//   * compile_c()'s dict is the struct Compiled: "work" (None when the emulator did not run), and "obj", "cod",
//     "log" only when that file was made (an empty optional is a missing key).
//   * sys.exit("...") is py::SystemExit with the text; argparse's exits are SystemExit(0) (-h) and SystemExit(2)
//     (usage and the error text written to `err`).
//   * main(argv): the Python's main() reads sys.argv, here argv[0] is the program name as sys.argv[0] is (the
//     usage line shows its basename); what it prints goes to `out`, argparse's usage and errors to `err`.
//     The command line is parsed as argparse parses this parser (-h/--help, --flags FLAGS, --selftest, [csrc];
//     long options by any unambiguous prefix, --flags=VALUE, a value or word with a space is not an option);
//     not covered: attached short options (-hX), the "--" separator is honoured, nothing else of argparse.
//   * DBX and MSVC are the module's values, read from the settings when the program starts; they may be assigned.
#pragma once

#include <cstdint>
#include <iostream>
#include <optional>

#include "py.hpp"

namespace codebreaker::build::msvc152 {

extern py::str DBX;    // settings.json: dosbox_x ("dosbox-x" when not set)
extern py::str MSVC;   // settings.json: msvc152 (Microsoft C/C++ 8.00c)

extern const py::str DOC;   // the module's docstring

struct Compiled {
    std::optional<py::str> work;   // the folder it ran in (kept for inspection; the caller cleans it up)
    std::optional<py::bytes> obj;
    std::optional<py::str> cod;    // the /Fc listing
    std::optional<py::str> log;    // the raw compiler log
};

// Compile csrc with MSVC 1.52c under DOSBox-X (build/dos_tools.py). Returns the object bytes, the /Fc listing
// text, and the raw compiler log.
Compiled compile_c(const py::str& csrc, const py::str& flags = py::str("/AM /c"), const py::str& stem = py::str("t"),
                   double timeout = 180);

// The command line (what the Python's main() does; the SystemExit it raises is the Python's, and the result is 0 when it returns)
int64_t main(const py::list<py::str>& argv, std::ostream& out = std::cout, std::ostream& err = std::cerr);

}  // namespace codebreaker::build::msvc152
