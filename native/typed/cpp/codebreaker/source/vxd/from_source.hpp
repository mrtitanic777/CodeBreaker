// codebreaker/source/vxd/from_source.hpp -- typed C++ of codebreaker.source.vxd.from_source.
//
// from_source.py -- build a LOADABLE Win95 VxD from reconstructed SOURCE, with NO byte transplant.
//
// This is the path that `build_loadable_idiomatic` could not take: it emits the recovered VxD as idiomatic DDK-macro assembly with each
// object in its REAL named segment, assembles it with the period ML.EXE, links it with the DDK's OWN LINK 2.60, and embeds the version
// resource with the DDK's OWN RC + ADRC2VXD. Every byte comes from an authentic period tool run over recovered source -- no oracle object
// bytes are overlaid and no container field is hand-patched. A VKD.VXD built this way boots real Windows 95 to the logon (and the
// keyboard is live at the logon prompt).
//
// The four requirements, all pinned by real boots (see docs/VXD_FROM_SOURCE.md):
//
//   1. SEGMENT MODEL. The DDB object's recovered code goes in `_LTEXT` (VxD_LOCKED_CODE_SEG) WITH the control dispatch; the service table +
//      `Declare_Virtual_Device` land in `_LDATA` via the macros; and the DDB object's post-DDB TAIL is DATA -- emitted into `_LDATA` after
//      the DDB, not left in `_LTEXT` where a disassembler's over-read would insert it ahead of the service table and shove the DDB off its
//      true offset. This yields the on-object order [dispatch][code][service][DDB][data].
//   2. FLAT MODEL. Every non-DDK segment is declared `<NAME> SEGMENT DWORD PUBLIC _FLAT '<NAME>CODE'`. `_FLAT` (=FLAT under MASM6) makes it
//      part of the flat group; `USE32` does not, and every cross-segment near call then fails to assemble (A2107). This is what the DDK's
//      MakeCodeSeg expands to.
//   3. THE RIGHT LINKER. The VxD device id (LE 0xc0) and the Windows version resource (0xb8/0xbc) are loader-REQUIRED. The VC++ 2.0 linker
//      (LINK 2.50) leaves 0xc0 blank and permutes the page map; the DDK shipped LINK 2.60.5046 (\MSVC20\LINK.EXE) SPECIFICALLY to fix VxD
//      linking (its README says so), and it stamps 0xc0 from the exported <NAME>_DDB and writes an identity map. The resource is then added
//      by the DDK's own RC + ADRC2VXD -- see version_rc / embed_version_resource, not a hand-copy. LINK386 (the OS/2 linker) is never needed:
//      it targets OS/2 LX and would not stamp a Win95 device id.
//   4. IDENTITY PAGE MAP + OBJECT ORDER. realseg_def marks every segment PRELOAD NONDISCARDABLE. This keeps the linker from grouping
//      discardable pages at end-of-file (which would permute the page map the Win95 static loader reads by LOGICAL slot -> objects load as
//      garbage -> init fault) AND keeps the objects in .def order == the oracle's object order. Result: an identity map and a matching object
//      table.
//
//     from codebreaker.source.vxd import from_source as F
//     r = F.build_from_source(v)          # v = engine.format_loaders.le.LEFile
//     open("vkd.vxd","wb").write(r["vxd"])
//
// NOTE ON PROVENANCE: the code, DDB, service table, fixups and object bytes are 100% from the recovered source (only commutative
// re-encodings differ from the original -- SIB base/index order, test/xchg operand order). The version resource is likewise rebuilt from
// recovered source: version_rc reconstructs the .rc (device id + internal name from the DDB; file description + copyright years recovered
// from the oracle's version-resource STRINGS -- metadata, not code) and the DDK's real RC + ADRC2VXD recompile and embed it. Nothing is
// transplanted and no MS tool is reimplemented; the toolchain is ML 6.11 + LINK 2.60.5046 + RC 3.11 + ADRC2VXD, all period-authentic (see
// docs/VXD_FROM_SOURCE.md).
//
// Typing notes:
//   * build_from_source / _result's dict is the struct Built: `ok` is always true there, `asm`/`def` are set by the source lane only (none
//     for the 512-byte-page fallback and for _result called alone), compile_ok / link_ok exist only for the source lane (the Python's keys are
//     absent otherwise), `object_flags` (always None) is not kept. ddb_rva / oracle_ddb_rva are `dd and dd["rva"]`: none when there is no DDB.
//   * embed_version_resource returns (the bytes or None, the log); with no version resource in the oracle the bytes are the ones given.
//   * The Python left the temp folders of the DOS tools behind (build_from_source's compile/link work folder, embed_version_resource's); they are
//     removed here and in the Python.
//   * main(argv, out, err): the Python's `__main__` block; argv is sys.argv[1:]; the return is the exit status; a missing file is the
//     IndexError of sys.argv[1].
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "codebreaker/source/vxd/disassembly.hpp"
#include "py.hpp"

namespace codebreaker::source::vxd::from_source {

using codebreaker::source::vxd::disassembly::VxDDisasm;
namespace le = codebreaker::formats::le;

extern const py::str _RC_DIR;                   // RC 3.11 driver + RCPP (DOS)
extern const py::list<py::str> _ADRC_REF;       // DDK version-resource embedder
extern const py::list<py::str> _INC16_REF;      // version.h / common.ver / ver.h
extern const py::set<py::str> _MASM_RESERVED;   // MASM reserved words a recovered 4-char segment tag can collide with

// The MASM SEGMENT name for a 4-char class tag: the tag itself, or a safe alias when it is a reserved word. Only the segment NAME changes;
// the class stays '<tag>CODE' so the object tag is preserved.
py::str _seg_name(const py::str& tag);

// Map each NON-DDB object index -> (segment_name, class) for the _FLAT custom segments, guaranteeing a UNIQUE, VALID MASM name per object.
// A tag that is blank, NUL-filled, a reserved word, or a DUPLICATE of another object's tag would otherwise yield an unnamed or colliding
// segment (A2034 `must be in segment block' / a silent LINK merge) -- give those a synthetic `SEG<idx>' name. The class tracks the name so
// LINK keeps every object distinct; a plain unique tag keeps its `<tag>CODE'.
py::dict<int64_t, std::tuple<py::str, py::str>> _object_segs(const py::list<py::str>& names, int64_t ddb_obj);

// The 4-char segment class tag stored in each LE object-table entry's reserved field (offset 20). LINK /VXD writes the segment's CLASS name
// truncated to 4 chars there (e.g. 'LCODE' -> 'LCOD'), which is how the original's segment structure survives in a stripped binary.
py::list<py::str> object_names(const le::LEFile& v);

// The DDB object's bytes AFTER the DDB are data (locked data + tables + BSS) that a recursive-descent disassembler over-reads as code
// (procedures straddling the DDB). Mark [ddb_end, vsize) of the DDB object as DATA so the emitter renders it as `db`/`dd` and it can be placed
// in _LDATA. Returns the tail start offset (or none if there is no DDB / no tail).
std::optional<int64_t> _reclassify_ddb_tail_as_data(VxDDisasm& D, const le::LEFile& v);

// Re-emit vxd_idiom's loadable source with each object in its REAL segment: the DDB object's code in `VxD_LOCKED_CODE_SEG` (_LTEXT) and its
// post-DDB tail split out into `_LDATA`; every other object in a `_FLAT`-model custom segment named for its class. (A transform over
// emit_idiomatic's output -- which emits synthetic per-object VSEGnn -- rather than a second emitter.)
py::str realseg_source(VxDDisasm& D, const le::LEFile& v, const py::list<py::str>& names, const std::optional<int64_t>& tail0);

// The .DEF SEGMENTS block: _LTEXT/_LDATA (the DDB object) plus one entry per custom segment, ALL `PRELOAD NONDISCARDABLE` -> the linker writes
// pages in logical order (identity page map). Names and classes come from _object_segs so they match realseg_source.
py::str realseg_def(const py::str& name, const py::list<py::str>& names, int64_t ddb_obj = 1);

// 16-bit (ANSI) VER resource: 'key\0value\0' pairs with WORD-align padding. Extract wanted keys. (A VxD's version resource is VOS_DOS_WINDOWS16,
// so its strings are ANSI, not the UTF-16 of a Win32 VS_VERSION_INFO.)
py::dict<py::str, py::str> ver_strings(const py::bytes& res);

// Reconstruct the VxD version-resource .rc as recovered SOURCE. The device id and internal name come from the recovered DDB; the human-authored
// strings (file description, copyright years) are recovered from the oracle's version-resource block `res`. Those strings are metadata the real
// RC recompiles into a fresh resource -- NOT a byte transplant. This is the .rc Microsoft fed to RC + ADRC2VXD (its shape is the DDK's own
// BASE\VXDRES\SAMPLE.RC).
py::str version_rc(const VxDDisasm& D, const py::bytes& res);

// Copy RC 3.11 (+ its RCPP preprocessor) and the DDK ADRC2VXD into a cached space-free tool dir, and the INC16 version headers into a cached
// space-free include dir (the DDK's own path has spaces DOSBox cannot mount). Returns (tooldir, incdir).
std::tuple<py::str, py::str> _stage_res_tools(const py::str& root);

// Add the Windows version resource with the DDK's OWN tools -- the BASE\VXDRES\MAKEFILE recipe, `rc -r <name>.rc` (RC 3.11) then
// `adrc2vxd <name>.vxd <name>.res` (DDK) -- replacing every hand-rolled container patch. Reconstructs the .rc (version_rc), compiles + embeds it
// under DOSBox, and returns (bytes, log) where bytes is the resource-carrying .vxd (hdr 0xb8/0xbc now reference a freshly compiled resource) or
// none if DOSBox is unconfigured / the tools error.
std::tuple<std::optional<py::bytes>, py::str> embed_version_resource(const py::bytes& vxd_bytes, const VxDDisasm& D, const py::str& root);

struct Built {
    bool ok = true;
    py::bytes vxd;
    int64_t vxd_len = 0, oracle_len = 0;
    std::optional<py::str> asm_, def_;
    int64_t objects = 0;
    bool page_map_identity = false;
    std::optional<int64_t> ddb_rva, oracle_ddb_rva;
    int64_t device_id = 0;
    bool device_id_ok = false;
    int64_t resource_len = 0, obj1_diffs = 0;
    py::str link_log, res_log;
    std::optional<bool> compile_ok, link_ok;
};

// Assemble the build_from_source return dict + container validation from the finished LE bytes.
Built _result(const py::bytes& final_, const le::LEFile& v, const VxDDisasm& D, const std::optional<py::str>& asm_ = std::nullopt,
              const std::optional<py::str>& deftext = std::nullopt, const py::str& llog = py::str(""), const py::str& rlog = py::str(""));

// Build a loadable from-source VxD (no transplant).
Built build_from_source(const le::LEFile& v, const std::optional<py::str>& name = std::nullopt, const py::str& root = py::str("."));

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::vxd::from_source
