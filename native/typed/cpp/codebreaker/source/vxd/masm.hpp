// codebreaker/source/vxd/masm.hpp -- typed C++ of codebreaker.source.vxd.masm
//
// masm.py -- assemble-and-compare for a recovered VxD object.
//
// The disassembler (disassembly) proves the listing is a faithful VIEW of the bytes. This closes the
// loop the other way: re-emit an executable object as MASM, assemble it with the period DDK ML.EXE (or the
// MASM-compatible nasm), and compare the assembled code bytes to the oracle object -- the byte-exact test.
//
// Emission rule that keeps the comparison honest:
//   * an instruction with NO fixup is emitted symbolically (mnemonic + operands) and re-encoded by ML --
//     this is what the test measures: does ML reproduce the original compiler's encoding?
//   * an instruction WITH a fixup, a VxD service call, and every data byte are emitted as raw `db` -- exact
//     by construction, since an object-level assembly has no linker to resolve fixups.
//
//     LEFile v = LEFile::from_file("vkd.vxd");
//     CompareObject r = compare_object(v, 1);      // r.match, r.oracle_len, r.symbolic_insns ...
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * The row dicts of D.insns are disassembly::Row; the label of a place that is absent or "" is read as "no
//     label" (Python's `D.labels.get(k)` is falsy for both).
//   * compare_object's result dict is the struct CompareObject (the dict's "D" is a shared VxDDisasm); the
//     per-object dicts of compare_module are ObjectReport, the module's ModuleReport.  _converge_range's dict is
//     ConvergeResult.
//   * `force_raw` / `crange` / `extra_labels` that may be None are std::optional.
//   * The assemblers' results (obj, log) are std::tuple<std::optional<py::bytes>, py::str>.  subprocess.run's
//     `timeout=` is not applied (the runtime's subprocess::run has none): a hung tool is not killed.
//   * `_nasm_cache` / `_native_ml_cache` are function-local statics.  The Python's `if __name__ == "__main__"` block is
//     not ported.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/source/vxd/disassembly.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::masm {

using codebreaker::source::vxd::disassembly::Row;
using codebreaker::source::vxd::disassembly::VxDDisasm;
namespace le = codebreaker::formats::le;

using Asm = std::tuple<std::optional<py::bytes>, py::str>;   // (object file or None, the assembler's log)

// ML.EXE (DDK 6.11c) is a Phar Lap TNT executable -- it needs DOSXNT.EXE, and the mount path must be space-free
// (DOSBox `mount` breaks on spaces). All three are staged into one cached space-free dir.
py::str _TC();          // os.path.join(os.environ.get("TEMP", "/tmp"), "vxdtc")
extern const py::list<py::str> _ML_REF;   // inside the reference SDK tree (paths.reference)
extern const py::str _DOSXNT_SRC;

// Locate the 'Microsoft Windows 95 DDK' dir (holds INC32 / MSVC20 / MASM611C), robust to a design_root() that points
// at the checkouts' parent instead of the Win95 design arena.
py::str _ddk_root();
// Path to a NATIVE (Win32) ML.EXE -- MASM 6.11d from the NT 3.51 DDK -- or the Win95 DDK's ML.
py::str native_ml();
py::str _stage_toolchain(const py::str& root);

// ---- MASM text for a single instruction
extern const py::dict<py::str, py::str> _MNEM;
extern const py::set<py::str> _STRING_OPS;
extern const py::set<py::str> _SHORTABLE;
py::str _masm_ops(const py::str& ops);
// MASM text for one instruction, or None if it must be emitted as raw bytes instead.
std::optional<py::str> _masm_line(const VxDDisasm& D, int64_t oi, const Row& row);
py::str _db(const py::bytes& bs);
extern const py::str _ABS_MEM;   // the pattern: [number] -- no base/index register
bool has_abs_mem(const Row& row);
extern const py::set<int64_t> _SEG_PREFIX;
bool has_seg_prefix(const Row& row);

using LineMap = py::dict<int64_t, int64_t>;
// The body lines of one object, and the map from a source line (base_line + index, 1-based) to the instruction offset.
std::tuple<py::list<py::str>, LineMap> object_body(const VxDDisasm& D, int64_t oi, const std::optional<py::set<int64_t>>& force_raw = std::nullopt,
                                                   int64_t base_line = 0, const std::optional<py::dict<int64_t, py::str>>& extra_labels = std::nullopt,
                                                   bool probe = false, const std::optional<std::tuple<int64_t, int64_t>>& crange = std::nullopt);
std::tuple<py::str, LineMap> emit_object_masm(const VxDDisasm& D, int64_t oi, const py::str& segname = py::str("_LTEXT"),
                                              const std::optional<py::set<int64_t>>& force_raw = std::nullopt, bool probe = false,
                                              const std::optional<std::tuple<int64_t, int64_t>>& crange = std::nullopt);
py::list<std::tuple<int64_t, int64_t>> split_ranges(const VxDDisasm& D, int64_t oi, int64_t max_ins = 6000);
py::set<int64_t> cross_range_force(const VxDDisasm& D, int64_t oi, int64_t lo, int64_t hi);

// ---- assemble + compare
// The Phar Lap TNT ML.EXE on CodeBreaker's own DOS executive, or None to use DOSBox.
std::optional<Asm> _tnt_ml(const py::str& args, const py::str& masm_text, const py::str& root);
Asm assemble(const py::str& masm_text, const py::str& root);

extern const py::str _NASM_PROBE;
// Path to a `--masm`-capable, public-case-preserving nasm, or None.
std::optional<py::str> find_nasm();
Asm _assemble_coff_nasm(const py::str& masm_text, const py::str& nasm);
bool _native_ml_ok();
Asm _assemble_coff_native_ml(const py::str& masm_text, const py::str& root);
Asm assemble_coff(const py::str& masm_text, const py::str& root);
py::list<Asm> assemble_coff_batch(const py::list<py::str>& masm_list, const py::str& root, int64_t budget = 300000, int64_t maxfiles = 32);
// From a COFF object with one code section, (section_bytes, {symbol_name: offset}).
std::tuple<py::bytes, py::dict<py::str, int64_t>> coff_text_syms(const py::bytes& obj);

extern const py::str _ERRLINE;   // the pattern (compiled with re.I)

// _converge_range's {sym, raw, match, force, text}
struct ConvergeResult {
    int64_t sym = 0, raw = 0;
    bool match = false;
    py::set<int64_t> force;
    py::bytes text;
};
// `first` seeds iteration 0 with an already-assembled first pass: (obj, log, linemap).
ConvergeResult _converge_range(const VxDDisasm& D, int64_t oi, const py::str& root, int64_t lo, int64_t hi, const py::bytes& oracle,
                               int64_t max_repair, const std::optional<std::tuple<std::optional<py::bytes>, py::str, LineMap>>& first = std::nullopt);

// compare_object's dict
struct CompareObject {
    bool ok = true, match = false;
    int64_t oracle_len = 0, asm_len = 0, n_diffs = 0, symbolic_insns = 0, forced_raw = 0;
    py::set<int64_t> force_raw_set;
    bool raw_fallback = false;
    int64_t n_files = 0;
    py::bytes asm_bytes, oracle_bytes;
    std::shared_ptr<VxDDisasm> D;
};
// Assemble-and-compare object oi against the oracle, maximising GENUINE re-encodes.
CompareObject compare_object(const le::LEFile& v, int64_t oi, const py::str& root = py::str("."), int64_t max_repair = 8, int64_t max_ins = 6000);

struct ObjectReport {
    bool ok = true;
    int64_t obj = 0;
    bool match = false;
    int64_t oracle_len = 0, asm_len = 0, n_diffs = 0, symbolic_insns = 0, forced_raw = 0;
    py::set<int64_t> force_raw_set;
    int64_t n_files = 0;
    bool raw_fallback = false;
    py::bytes asm_bytes, oracle_bytes;
};
struct ModuleReport {
    bool byte_exact = false;
    py::list<ObjectReport> objects;
    int64_t symbolic = 0, forced_raw = 0, raw_fallback_objs = 0;
};
// Assemble-and-compare every executable object of a VxD.
ModuleReport compare_module(const le::LEFile& v, const py::str& root = py::str("."), int64_t max_repair = 8, int64_t max_ins = 6000);

}  // namespace codebreaker::source::vxd::masm
