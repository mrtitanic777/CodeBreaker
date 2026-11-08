// codebreaker/exact/habits/cmpp.hpp -- C++ from codebreaker.exact.habits.cmpp.
//
// cmpp.py - a Pascal procedure compiled by the original compiler, compared with the executable.
//
//   python cmpp.py M.EXE 0x00001 m100/p00001.pas [--proc NAME] [--asm] [--cod]
//
// The file is a small complete Pascal program: the declarations the procedure needs, stubs for the
// procedures it calls, and the procedure itself, named P<address> (P00001 for the one at image
// 0x00001) unless --proc says otherwise. It is compiled with the MS-Pascal compiler of 1981
// (tools/pcc.py) and the procedure's bytes are compared with the executable's. Not compared, because
// they depend on where things are placed: the bytes a linker fills in (addresses of variables and
// constants, far call targets) and the displacement of a near call to another procedure.
// Prints IDENTICAL, or the instructions that differ. --asm prints the original procedure and
// compiles nothing; --cod prints the compiler's own listing of the compiled procedure.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/exact/x86.hpp"
#include "py.hpp"

namespace codebreaker::exact::habits::cmpp {

// A raised ImportError (Python's `import pcc` failing)
using py::ImportError;

// What cmpp.py takes from outside, none of it with a typed port yet:
//   MD = platforms.decoder(16)  -- the 16-bit x86 decoder; the interface x86.hpp declares (first instruction
//                                  of the bytes at an address, or nullopt)
//   import pcc                  -- tools/pcc.py, the MS-Pascal compiler driver: image(exe) and compile_pas(path)
//   HERE                        -- the folder of cmpp.py (the ledger is HERE/../ledger/<EXE>.json)
void set_decoder(x86::Decoder d);

// what pcc.compile_pas(path) returns (its .code, .procs, .fix, .lst, .console, .cod)
struct CompileResult {
    std::optional<py::bytes> code;
    py::dict<py::str, std::tuple<int64_t, int64_t>> procs;
    py::list<int64_t> fix;
    py::str lst;
    py::str console;
    py::str cod;
};
struct Pcc {
    std::function<py::bytes(const py::str&)> image;
    std::function<CompileResult(const py::str&)> compile_pas;
};
void set_pcc(Pcc p);

extern py::str HERE;

// what follows a procedure's call of the entry helper: a byte and a word (build.py sets 2 for MASM 3.00)
extern int64_t HDRLEN;

using WalkItem = std::tuple<int64_t, int64_t, py::str, py::str>;

// -> [(offset, size, text, mnemonic)]; the db/dw after the entry call are data
py::list<WalkItem> walk(const py::bytes& b, int64_t base = 0, bool skip_hdr = true);

// offsets not compared: fixups, near call displacements to outside, far pointers
py::set<int64_t> loose_bytes(const py::bytes& b, const py::set<int64_t>& fx = py::set<int64_t>());

// main(): Python reads sys.argv; the caller hands it in (program name first). Returns the exit status
// (0, or what Python passes to sys.exit); exceptions propagate as Python's would.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::exact::habits::cmpp
