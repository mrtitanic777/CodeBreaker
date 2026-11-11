// codebreaker/source/vxd/link.hpp -- typed C++ of codebreaker.source.vxd.link
//
// link.py -- full-file LE rebuild of a VxD via ML (-coff) + LINK /VXD.
//
// The object-level test (masm) proves each object's CODE reassembles byte-exact. This goes the whole way: emit the
// entire driver as a multi-segment MASM source + a .DEF, assemble to COFF with the period DDK ML.EXE, and LINK it
// with `/VXD` (MS LINK 2.60, which runs natively on modern Windows) into a real .vxd (LE) -- then byte-compare the
// whole file to the oracle, region by region.
//
//     BuildAndCompare r = build_and_compare(v, "VKD");
//
// One unit with idiom.hpp (they import each other).
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * The result dicts are structs: BuildAndCompare, BuildFunctional, LoadableIdiomatic (the keys of the dict that
//     are not always present are std::optional; the Python's "asm" / "def" keys are `asm_` / `def_`), CompareObjects
//     (compare_objects' report) and OverlayRow (one entry of `overlay`).
//   * `_source_code_into_container` raises the ValueError of the Python when it cannot match every object.
//   * `force_raw` ({oi: set(offsets)}, or None) is std::optional<py::dict<int64_t, py::set<int64_t>>>.
//   * Python's `round(x, nd)` is correctly rounded (half to even on the exact value); `_round` here is the same.
//   * subprocess.run(..., timeout=) is not applied (the runtime's subprocess::run has none).
//   * The Python's `if __name__ == "__main__"` block is not ported.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/source/vxd/disassembly.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::link {

using codebreaker::source::vxd::disassembly::VxDDisasm;
namespace le = codebreaker::formats::le;
using ForceRaw = py::dict<int64_t, py::set<int64_t>>;

extern const py::str _LINK;
extern const py::list<py::str> _LINK260_FILES;
// standard DDK VxD segment map; LINK ignores segments absent from the object, so one template serves every VxD.
extern const py::str _VXD_DEF_SEGS;

// .DEF segment attributes for an LE object's flags.
py::str _seg_attrs(int64_t flags);
// Give every INTERNAL fixup target a label so the functional emit can reference it.
void _label_fixup_targets(VxDDisasm& D);
// Object body for a FUNCTIONAL (loadable, not byte-exact) rebuild.
py::list<py::str> _link_body(const VxDDisasm& D, int64_t oi, const std::optional<py::dict<int64_t, py::str>>& extra_labels = std::nullopt);
// (asm_text, def_text) for the whole VxD; `functional` emits internal fixups symbolically.
std::tuple<py::str, py::str> emit_source_and_def(VxDDisasm& D, const py::str& vxdname, const std::optional<ForceRaw>& force_raw = std::nullopt,
                                                 bool functional = false);

// -> (obj or None, log, work dir or None)
std::tuple<std::optional<py::bytes>, py::str, std::optional<py::str>> _assemble_coff(const py::str& asm_text, const py::str& root);
// Link one COFF object into a .vxd with LINK /VXD (native). -> (the file or None, the output)
std::tuple<std::optional<py::bytes>, py::str> _link_vxd(const py::bytes& obj_bytes, const py::str& def_text, const py::str& root, const py::str& work);
// Copy LINK 2.60 + its DLLs into a cached SPACE-FREE dir and return the staged LINK.EXE path.
py::str _stage_link260(const py::str& root);
// Link one COFF object into a loadable .vxd with the DDK's LINK 2.60 /VXD.
std::tuple<std::optional<py::bytes>, py::str> link_vxd_260(const py::bytes& obj_bytes, const py::str& def_text, const py::str& root, const py::str& work,
                                                           int64_t align = 0x1000);

// Per exec object, the force-raw offsets that made the object-level byte-exact test exact, and whether it matched.
std::tuple<ForceRaw, py::dict<int64_t, bool>> byte_exact_force_raw(const le::LEFile& v, const py::str& root = py::str("."));

// compare_objects' report
struct ObjectMatch {
    int64_t mine = 0, oracle = 0, equal = 0, n = 0;
    double pct = 0;
};
struct CompareObjects {
    py::list<ObjectMatch> objects;
    int64_t exact_objects = 0, total = 0;
};
// Match each linked object to the oracle object with the same vsize + best byte overlap, report per-object exactness.
CompareObjects compare_objects(const le::LEFile& orc, const le::LEFile& mine);

struct BuildAndCompare {
    bool ok = false;
    std::optional<py::str> stage, log, asm_, def_, link_log;
    std::optional<py::bytes> vxd;
    int64_t vxd_len = 0, oracle_len = 0;
    std::optional<CompareObjects> report;
};
BuildAndCompare build_and_compare(const le::LEFile& v, const py::str& vxdname, const py::str& root = py::str("."),
                                  const std::optional<ForceRaw>& force_raw = std::nullopt);

struct BuildFunctional {
    bool ok = false;
    std::optional<py::str> stage, log, asm_, def_, link_log;
    std::optional<py::bytes> vxd;
    int64_t vxd_len = 0, oracle_len = 0, oracle_fixups = 0, oracle_internal = 0, rebuilt_fixups = 0, rebuilt_internal = 0;
};
// Build a LOADABLE .vxd (relocations present, container NOT byte-exact).
BuildFunctional build_functional(const le::LEFile& v, const py::str& vxdname, const py::str& root = py::str("."));

// Assemble idiomatic DDK-macro source to COFF with the NATIVE ML.EXE. -> (obj or None, log, work dir)
std::tuple<std::optional<py::bytes>, py::str, py::str> _assemble_idiomatic(const py::str& asm_text, const py::str& root);

// one object of the overlay report
struct OverlayRow {
    int64_t obj = 0;
    std::optional<int64_t> asm_obj;
    double score = 0;
    int64_t cmp = 0, expected = 0, asm_len = 0, macro = 0;
    bool ok = false;
};
// Overlay the assembled-from-source CODE onto the recovered object bytes `D.code` and emit through the byte-exact LE
// writer. Raises ValueError when it cannot match every object confidently. D.code is changed.
std::tuple<py::bytes, py::list<OverlayRow>> _source_code_into_container(VxDDisasm& D, const le::LEFile& v, const py::bytes& fs_bytes,
                                                                        double min_score = 0.5);

struct LoadableIdiomatic {
    bool ok = false;
    std::optional<py::str> stage, log, asm_, def_, link_log;
    std::optional<py::bytes> vxd;
    int64_t vxd_len = 0, oracle_len = 0, objects = 0, rebuilt_internal = 0, oracle_internal = 0;
    py::list<OverlayRow> overlay;
    double min_overlay_score = 0;
};
// Build a LOADABLE LE .vxd from the *idiomatic DDK-macro* source and the real toolchain.
LoadableIdiomatic build_loadable_idiomatic(const le::LEFile& v, const std::optional<py::str>& name = std::nullopt, const py::str& root = py::str("."));

}  // namespace codebreaker::source::vxd::link
