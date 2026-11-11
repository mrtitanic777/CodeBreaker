// codebreaker/source/vxd/idiom.hpp -- typed C++ of codebreaker.source.vxd.idiom
//
// idiom.py -- re-emit a recovered VxD in the idiomatic DDK-macro source form.
//
// The byte-exact tree is correct but low-level: `db`/`cmp`/`jmp` + raw `int 20h`. Real Win95 VxD source used the
// DDK macros: the DDB is a `Declare_Virtual_Device`, the control proc is `Begin_Control_Dispatch` /
// `Control_Dispatch <Msg>, <Handler>` / `End_Control_Dispatch`, a service call is `VMMcall`/`VxDcall <name>`,
// functions are `BeginProc`/`EndProc`, segments are `VxD_LOCKED_CODE_SEG`. Those macros EXPAND to exactly the
// bytes we recover, so the idiomatic form re-assembles to the same image, `include vmm.inc` and all.
//
//     VxDDisasm D(v); D.run();
//     py::str text = emit_idiomatic(D);              // D.v is the LEFile v
//     py::str loadable = emit_idiomatic(D, true);
//
// link.py and idiom.py import each other (_label_fixup_targets / control_dispatch ...): they are one unit (idiom.cpp
// and link.cpp, link.hpp is the other half).
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * `D` is the VxDDisasm of disassembly.hpp: its rows are `Row`, its fix-ups le::Fixup.  emit_idiomatic changes D
//     (it names labels and drops instructions), as the Python does, so D is passed non-const.  `D.v` is always set
//     (disassembly.hpp keeps the LEFile), so the Python's `hasattr(D, "v")` tests are not needed.
//   * An internal fix-up with no target (a 16-bit selector) where the Python does `f["target"] & mask` raises the
//     TypeError it raised; where it does `f["obj"]` on an import fix-up (no such key) the KeyError.
//   * The sets of (object, offset) places the macros absorb (`skip`, `emittable`) are disassembly::PlaceSet: only
//     membership is read from them.
//   * `ctx` (the loadable-mode context dict) is `LoadCtx`, passed by pointer (None = nullptr).
//   * platforms.decoder(32, detail=True) (capstone) is the capstone.dll Python's package carries, loaded by path: hand
//     it to set_capstone_dll() once (a decode without it is an ImportError).  Only `_disp0` decodes.
//   * `_DISP0_CACHE` is a file-local py::dict.  The `if __name__ == "__main__"` block is not ported.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/source/vxd/disassembly.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::idiom {

using codebreaker::source::vxd::disassembly::Place;
using codebreaker::source::vxd::disassembly::PlaceSet;
using codebreaker::source::vxd::disassembly::Row;
using codebreaker::source::vxd::disassembly::VxDDisasm;
namespace le = codebreaker::formats::le;

// Names the capstone.dll to decode with (the one Python's capstone package loads).
void set_capstone_dll(const py::str& path);

// VxD device id -> the DDK .INC that defines its services (so `VxDcall <name>` resolves). Only devices whose .INC
// actually ships in INC32 are listed; a call to any other device keeps its raw `int 20h`+DWORD bytes.
extern const py::dict<int64_t, py::str> DEVICE_INC;

// INC files to include: vmm.inc always, plus one per foreign device this object calls.
py::list<py::str> device_incs(const VxDDisasm& D, int64_t oi);
// `VMMcall <name>` / `VxDcall <name>` for an INT 20h service, or None if the name is unknown (then the caller keeps the
// raw `int 20h`+DWORD bytes). Bit 0x8000 of the service word is DL_Jmp_Mask: a jmp-reference is VMMJmp/VxDJmp.
std::optional<py::str> svc_macro(int64_t dev, int64_t svc);
// `Declare_Virtual_Device` line for a recovered DDB; with `loadable` the trailing V86/PM/reference procs too.
py::str ddb_macro(const VxDDisasm& D, const le::Ddb& db, bool loadable = false);

// the VxD's own exported service table (see idiom.py): the names per index, where the raw table is, and the extra
// labels to put at shared handlers
struct ServiceTable {
    py::list<py::str> names;                              // 'RESERVED' for a null slot
    std::tuple<int64_t, int64_t, int64_t> loc;            // (obj, off, count)
    py::dict<Place, py::list<py::str>> extra;             // handler place -> the service names to emit as extra labels
};
// None when there is no table, a handler landed mid-instruction, or two services carry the identical name.
std::optional<ServiceTable> service_table(const VxDDisasm& D, const PlaceSet& emittable);

// The emitted label a direct branch targets, or None.
std::optional<py::str> _branch_label(const VxDDisasm& D, int64_t oi, int64_t a, const Row& row, const PlaceSet& emittable);

// ([(MESSAGE_NAME, handler_label)], the {(obj,off)} the macro regenerates): ([], {}) when no shape is recognised.
using Handlers = py::list<std::tuple<py::str, py::str>>;
std::tuple<Handlers, PlaceSet> control_dispatch(const VxDDisasm& D, const PlaceSet& emittable);

extern const py::dict<py::str, py::str> _SEG_MACRO;
// mnemonics whose text the period DDK ML (MASM 6.11) rejects or re-encodes -- byte-preserved
extern const py::set<py::str> _RAW_MNEMS;
extern const py::set<int64_t> _PREFIX_BYTES;
extern const py::set<py::str> _SHIFTS;

// A shift/rotate rendered with a single operand -- the implicit `,1` that capstone drops but ML requires.
bool _shift_no_count(const Row& row);
extern const py::str _XMM_MM;   // the pattern
// An instruction the 1994 MASM 6.11 predates (SSE, MMX, cmovcc/fcmovcc).
bool _ml611_unknown(const Row& row);
bool _x87_stack(const Row& row);
bool _indirect_branch(const Row& row);
bool _cap_size(const Row& row);
// True if the instruction carries a 0x67 address-size prefix.
bool _addr16(const Row& row);
// An instruction whose ORIGINAL encoding carries an explicit zero displacement (ModRM mod=01/10 and disp == 0).
bool _disp0(const Row& row);
// True if the instruction carries a 0x66 operand-size prefix.
bool _opnd16(const Row& row);
// Byte length of the on-disk VxD_Desc_Block (0x34, or the modern DDB's size).
int64_t _ddb_size(const VxDDisasm& D, const le::Ddb& db);
py::list<py::str> _all_incs(const VxDDisasm& D);
// Render a DATA span readably: printable-ASCII runs (>=4 bytes) as `db 'text'`, the rest as hex.
py::list<py::str> _db_data(const py::bytes& bs, int64_t width = 60);

extern const py::dict<int64_t, py::str> _JCC;
extern const py::str _NUM_TARGET;   // the pattern

// LOADABLE mode: the FLAT segment name + class for one LE object.
std::tuple<py::str, py::str> obj_seg(int64_t oi, int64_t flags, const std::optional<int64_t>& ddb_obj = std::nullopt);
// The emitted label an INTERNAL fixup at (oi,k) points to, or None.
std::optional<py::str> _fx_label(const VxDDisasm& D, int64_t oi, int64_t k);

// the context of a loadable emit (what the Python's ctx dict holds)
struct LoadCtx {
    py::str name;
    int64_t ddb_obj = 0, ddb_rva = 0, ddb_hi = 0;
    PlaceSet skip;
};
// The `dd` operand for a fixup target; `toff` None (a selector) raises the TypeError the Python's `toff & mask` did.
std::optional<py::str> _dd_target_expr(const VxDDisasm& D, int64_t tobj, const std::optional<int64_t>& toff, const LoadCtx* ctx);
// LOADABLE mode: an instruction's INTERNAL fixups emitted symbolically; None to fall through.
std::optional<py::list<py::str>> _loadable_insn(const VxDDisasm& D, int64_t oi, int64_t a, const Row& row, const LoadCtx* ctx = nullptr);
// code[a:b] as raw db, with `<name> label near` at each interior offset in `labels`.
py::list<py::str> _db_split_labels(const py::bytes& code, int64_t a, int64_t b, const py::dict<int64_t, py::str>& labels);
// LOADABLE mode: a data span [a,b), each INTERNAL-fixup DWORD a `dd offset <label>`.
py::list<py::str> _loadable_data(const VxDDisasm& D, int64_t oi, int64_t a, int64_t b, const LoadCtx* ctx = nullptr);

// Emit one VxD as idiomatic DDK-macro source (see the Python's docstring); `D` is changed (labels named, dropped insns).
py::str emit_idiomatic(VxDDisasm& D, bool loadable = false);

extern const py::str _HEADER;   // the file header, with {name} to format
// Write outdir/<name>/<name>.asm -- the whole VxD as one idiomatic DDK-macro source file. Returns the path written.
py::str emit_idiomatic_tree(const le::LEFile& v, const py::str& name, const py::str& outdir);
// Assemble every module's idiomatic source through the period DDK ML.EXE and report which fail. {name: assembled}.
// `only` limits to a set of lowercase module names.
py::dict<py::str, bool> verify_corpus(const py::str& srcdir, const py::str& root = py::str("."), int64_t budget = 40000,
                                      const std::optional<py::set<py::str>>& only = std::nullopt);

}  // namespace codebreaker::source::vxd::idiom
