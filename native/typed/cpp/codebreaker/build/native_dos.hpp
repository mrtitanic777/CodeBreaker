// codebreaker/build/native_dos.hpp -- typed C++17 port of codebreaker.build.native_dos.
//
// native_dos.py -- run the period real-mode DOS tools on CodeBreaker's OWN emulator, in-process,
// instead of launching DOSBox-X.
//
// DOSBox-X boots a whole emulated PC in an external process to run one 16-bit DOS program: it is
// slow, serial, heavyweight and an external dependency. CodeBreaker already has a real-mode DOS
// executive on Unicorn (exact/emu/dos): a PSP, the MCB memory chain, program load and INT 20h/21h over a
// shim filesystem that maps a working directory, all in-process with no disk image. This wraps that
// executive in the shape build/masm wants, so the optional period byte-match can re-assemble with the
// authentic MASM.EXE on our own CPU.
//
//     native_dos::available()                     Unicorn can be opened
//     native_dos::enabled(tool)                   the native runner is on (see the CB_NATIVE_DOS switch)
//     native_dos::assemble(src, stem, masmroot)   -> Assembly   (one source; mirrors masm::assemble)
//     native_dos::assemble_many(items, masmroot)  -> {work, results: {stem: Product}} (mirrors the warm worker)
//
// THE SWITCH -- CB_NATIVE_DOS:
//     unset / "auto"   native for the tools PROVEN byte-identical to DOSBox (_PROVEN: MASM and the DDK's
//                      TNT ML), with DOSBox as the automatic fallback when the native run cannot complete
//     "1" / "on"       force the native runner (no DOSBox)
//     "0" / "off"      force DOSBox (the native runner is not used)
//
// THE DPMI TOOL (TNT ML.EXE): the Win95 DDK's ML.EXE (the VxD/driver assembler) is a Phar Lap TNT bound
// program; ml() runs it with the DPMI host, entirely in memory (a MemFS: nothing written to the disk).
//
// Notes on the port:
//   - available(): Python's "is unicorn importable" is here "can a Unicorn engine be opened" -- the
//     program has handed over unicorn.dll (exact::emu::dpmi::load_unicorn) or it has not.
//   - the dicts the Python returns ({obj, lst, log, work}) are the `Assembly` struct; a part the tool did not
//     make is nullopt (a missing key).
//   - run_extended's `files` values are text (str: written with CRLF line ends) or bytes: a variant, as the
//     Python's isinstance(src, str) tests; `tool_files` values are paths (MemFS.disk reads them).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/build/dos_sessions.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::native_dos {

// The native runner could not complete a run (unimplemented service, CPU fault, limit): the caller falls
// back to DOSBox. A clean run that merely reports tool errors is NOT this.
struct NativeDosError : py::RuntimeError {
    NativeDosError() : py::RuntimeError() {}
    explicit NativeDosError(const std::string& m) : py::RuntimeError(m) {}
    const char* type() const override { return "NativeDosError"; }
};

// what an assembly made: {obj, lst, log, work}; a part not made is nullopt. aborted: {"work": dir, "aborted": True}.
struct Assembly {
    std::optional<py::bytes> obj;
    std::optional<py::str> lst;
    std::optional<py::str> log;
    py::str work;
    bool aborted = false;
};

using Product = dos_sessions::Product;
using Assembled = dos_sessions::Assembled;
using FileData = std::variant<py::str, py::bytes>;   // text (CRLF on "disk") or bytes

bool available();

// Whether the native runner should be used for `tool`. See the CB_NATIVE_DOS switch above.
bool enabled(const py::str& tool = py::str("masm"));

extern const py::set<py::str> _PROVEN;   // tools proven byte-identical to DOSBox (MASM; the DDK TNT ML)

// Run a real-mode DOS program under the executive. -> (exit code, console bytes). Raises NativeDosError if the
// executive could not complete the run.
std::tuple<std::optional<int64_t>, py::bytes> _run_tool(const py::str& exe, const py::str& work, const py::str& cmdline,
                                                       bool fast = true);

// work/<stem><ext>, making any sub-folder the stem names
py::str _stem_path(const py::str& work, const py::str& stem, const py::str& ext);

py::str _masm_exe(const py::str& work, const py::str& masmroot);

Product _read_out(const py::str& work, const py::str& stem);

py::str _cmdline(const py::str& stem);

// One source, the native runner. -> Assembly. Raises NativeDosError if the executive could not complete (the
// caller then uses DOSBox).
Assembly assemble(const py::str& asm_src, const py::str& stem = py::str("t"), const std::optional<py::str>& masmroot = std::nullopt);

// Run a DOS-extended (DPMI) tool -- the Phar Lap TNT ML.EXE -- on the executive with the DPMI host, entirely in
// memory: the tool's own files ({name: path}) and the inputs ({name: text or bytes}) are given, what it writes stays
// in a MemFS, nothing touches the disk. -> ({artifact: bytes} of the ones made, console bytes). Raises
// NativeDosError if the run could not complete.
std::tuple<py::dict<py::str, py::bytes>, py::bytes> run_extended(const py::dict<py::str, py::str>& tool_files, const py::str& exe,
                                                                const py::str& cmdline, const py::dict<py::str, FileData>& files,
                                                                const py::list<py::str>& artifacts,
                                                                const py::bytes& env = py::bytes(std::string("PATH=C:\\\0", 9)));

// The Win95 DDK's Phar Lap TNT ML.EXE, its ML.ERR and the DOSXNT.EXE extender: {name: path}.
py::dict<py::str, py::str> ml_files(const std::optional<py::str>& root = std::nullopt);

// TNT ML.EXE natively: ml("/c /Fom.obj m.asm", {"m.asm": text}, ["m.obj"]) -> ({artifact: bytes}, console text).
// Raises NativeDosError when the run could not complete.
std::tuple<py::dict<py::str, py::bytes>, py::str> ml(const py::str& args, const py::dict<py::str, FileData>& files,
                                                    const py::list<py::str>& artifacts, const std::optional<py::str>& root = std::nullopt);

// [(stem, asm_src)] -> {work: dir, results: {stem: Product}}. One MASM run per item, in a shared work folder (the
// real MASM.EXE copied in once). Raises NativeDosError the moment any item's run could not complete, so the caller
// falls the WHOLE call back to DOSBox.
Assembled assemble_many(const py::list<std::tuple<py::str, py::str>>& items, const std::optional<py::str>& masmroot = std::nullopt);

}  // namespace codebreaker::build::native_dos
