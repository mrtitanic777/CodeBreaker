// codebreaker/source/vxd/tree.hpp -- typed C++ of codebreaker.source.vxd.tree.
//
// tree.py -- emit a VxD as a proper multi-file MASM source tree.
//
// Real Win95 VxD source was never one monolithic file: it was organised per segment (_LTEXT locked code, _ITEXT init, _PTEXT
// pageable, message tables, data) and split further by function. This emits that structure -- one directory per driver, one file
// per segment, large segments split at function boundaries into numbered files -- reusing the disassembler's DDK-recovered names
// (services, control handlers, and the 12 core VxDs' internal names). Each file carries a header noting the segment class and,
// honestly, that the ORIGINAL file/function names are not present in a stripped LE binary, so files are named by segment/role
// rather than fabricated Microsoft filenames.
//
//     from codebreaker.formats import le
//     from codebreaker.source.vxd import tree as vxd_tree
//     vxd_tree.emit_tree(le.LEFile.from_file("vkd.vxd"), "VKD", "recovered/vxd_src")
//
// Typing notes:
//   * A probe (a function from a file path to (return code, error text, the pattern of an error line)) is a std::function; the
//     Python's `probe_nasm` / `probe_wasm` arguments are the optional paths of the assemblers.
//   * force_raw_map (None, or {object: {offsets}}) is std::optional<link::ForceRaw>; extra_labels {offset: name} is a py::dict.
//   * _entrytab_ddb's (obj, rva) or None is std::optional<std::tuple>.
//   * The files are written with newline="" (the Python's text-mode write made each "\r\n" "\r\r\n"; the Python was changed alike).
//   * main(argv, out, err): the Python's `__main__` block (it reads sys.argv); argv is sys.argv[1:]; what it prints goes to `out`;
//     the return is the exit status. A missing file argument is the IndexError of the Python's sys.argv[1].
#pragma once

#include <cstdint>
#include <functional>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/link.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::tree {

using codebreaker::source::vxd::disassembly::VxDDisasm;
namespace le = codebreaker::formats::le;

extern const py::str _ERRLINE;        // the pattern (compiled with re.I): nasm / gas style
extern const py::str _ERRLINE_WASM;   // the pattern (compiled with re.I): wasm: file(NN): Error!

// (return code, the error text, the pattern an error line is read with)
using ProbeResult = std::tuple<int64_t, py::str, py::re::Pattern>;
using Probe = std::function<ProbeResult(const py::str& path)>;

// probe using `nasm --masm -f bin`
Probe _nasm_probe(const py::str& nasm);
// probe using OpenWatcom `wasm -zcm=masm`
Probe _wasm_probe(const py::str& wasm);

// Emit body, assemble via `probe` (a nasm/wasm assembler callable), byte-preserve (force_raw -> db) every instruction the assembler
// rejects, and repeat until it assembles clean. db == the exact bytes, so this only trades an un-assemblable disassembly (data walked
// as code, a mnemonic/operand the assembler won't spell) for its raw bytes -- byte-exact preserved and now buildable. Returns the body
// lines.
py::list<py::str> _converge_buildable(const VxDDisasm& D, int64_t oi, int64_t lo, int64_t hi, const py::dict<int64_t, py::str>& extra_labels,
                                      const py::list<py::str>& head, const py::list<py::str>& tail, const Probe& probe, int64_t maxit = 20);

// LE object flags -> DDK segment class / name (inferred; the class strings are the DDK's own).
std::tuple<py::str, py::str> seg_class(int64_t flags);

// (obj, rva) of the DDB from the LE entry table's ordinal-1 export -- the authoritative location every VxD publishes, used when the
// heuristic scan (D.ddb) misses it (e.g. VMM itself). Nothing if unparseable.
std::optional<std::tuple<int64_t, int64_t>> _entrytab_ddb(const le::LEFile& v);

// Write the multi-file source tree for one VxD; returns the list of files written. With probe_nasm (a `nasm --masm` binary) or
// probe_wasm (an OpenWatcom `wasm` binary), each file is converged to assemble clean under that assembler by byte-preserving (db)
// the instructions it rejects.
py::list<py::str> emit_tree(const le::LEFile& v, const py::str& name, const py::str& outdir, int64_t max_ins = 6000,
                            const std::optional<py::str>& probe_nasm = std::nullopt, const std::optional<py::str>& probe_wasm = std::nullopt,
                            const std::optional<link::ForceRaw>& force_raw_map = std::nullopt);

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::vxd::tree
