// codebreaker/source/vxd/disassembly.hpp -- typed C++ of codebreaker.source.vxd.disassembly
//
// disassembly.py -- recursive-descent 32-bit disassembler + MASM emitter for Win95 VxDs.
//
// Takes an LEFile (formats.le) and reconstructs each Virtual Device Driver as annotated 32-bit
// assembly. A VxD's executable object is a MIX of code and data (the DDB, jump tables, the service
// table, string/pointer tables all live inside it), so a linear sweep disassembles the data as
// garbage. This uses RECURSIVE DESCENT instead: it seeds from the real entry points recorded in the
// DDB (control procedure, API procedures, service table) and follows control flow, discovering
// jump-table dispatchers as it goes. Everything the descent never reaches as code is emitted as data.
//
// Three VxD-specific mechanisms are decoded:
//
//   1. The DDB (Device Descriptor Block) -- the driver's identity -- becomes a Declare_Virtual_
//      Device with its fields (name, version, device id, control/API procedures, service table).
//   2. INT 20h service calls -- `int 20h` + an inline DWORD (svc lo word, device id hi word) --
//      become `VMMCall`/`VxDCall <DEVICE>.Service_<n>`; the DWORD is consumed, not disassembled.
//   3. Fixups (formats.le fixups) resolve every zeroed operand to its real target; the
//      control-dispatch `jmp [eax*4 + table]` and its table are recovered as a labelled jump.
//
//     LEFile v = LEFile::from_file("vkd.vxd");
//     py::str text = emit_asm(v);
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * `platforms.decoder(32 / 16, detail=True)` (capstone) has no C++ counterpart: the decoder is the very
//     capstone.dll Python's `capstone` package carries, loaded by path.  Hand it to set_capstone_dll() once
//     (as analyze/dos_functions does); a decode without it is an ImportError.  The .cpp includes capstone's own
//     headers (the ones that package ships).
//   * The row dicts of `insns` are the struct `Row` (fx is the fix-up found inside the instruction, if any;
//     vxdcall is (device, service, "import" | "inline") for a VxD service call).
//   * A fix-up is le::Fixup (internal or import).  An internal fix-up with `target` None (a 16-bit selector) is
//     kept as std::nullopt; where the Python carries that None into a tuple key or arithmetic, a place whose
//     offset is NONE_TARGET (INT64_MIN) stands for (obj, None): it is never an instruction, a label or a mark, and
//     the descent raises the TypeError the Python raised when it compares or masks it.
//   * The sets of (object, offset) bytes (`mark`, `data`, `entries`) are hash sets: nothing iterates them, so
//     their order cannot show.  Every dict whose order shows (insns, labels, jtables, fx, ...) is a py::dict
//     (insertion order, as in Python).
//   * `code` / `objs` are dicts by object index; the VxDDisasm keeps a reference to the LEFile it was given, which
//     must outlive it (the Python keeps the object).
//   * `disassemble_file` returns the stat dicts as the struct `FileStat`; `verify` returns `VerifyRow`s.
//   * The Python's `if __name__ == "__main__"` block (the --verify / --all command line) is not ported.
//   * `HERE` / `ROOT` / the sys.path insert are import-time plumbing and have no counterpart.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <unordered_set>

#include "codebreaker/formats/le.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::disassembly {

namespace le = codebreaker::formats::le;
using Place = le::Place;   // (object, offset)

// the place that stands for (obj, None): see the typing notes
constexpr int64_t NONE_TARGET = INT64_MIN;

struct PlaceHash {
    size_t operator()(const Place& p) const {
        return static_cast<size_t>(std::get<0>(p)) * 0x9E3779B97F4A7C15ull ^ (static_cast<size_t>(std::get<1>(p)) + 0x7F4A7C15u);
    }
};
using PlaceSet = std::unordered_set<Place, PlaceHash>;

// Names the capstone.dll to decode with (the one Python's capstone package loads).
void set_capstone_dll(const py::str& path);

// Canonical Win95 VxD device ids -> names (DDK VxD service tables). The high word of an INT 20h
// service DWORD is the device id; this names the callee module for `VxDCall`.
extern const py::dict<int64_t, py::str> VXD_DEVICES;

// DDB field offsets, relative to the DDB start (the DDB_Next dword). ddb()'s rva points here.
extern const int64_t DDB_CONTROL_PROC;
extern const int64_t DDB_V86_API_PROC;
extern const int64_t DDB_PM_API_PROC;
extern const int64_t DDB_SERVICE_TABLE;
extern const int64_t DDB_SERVICE_SIZE;

extern const py::set<py::str> _TERMS;
extern const py::set<py::str> _BRANCHES;

// [start, end) byte ranges in `code` that are ASCII string DATA the disassembler would otherwise
// decode as instructions. A run of >=8 printable bytes qualifies only if it is NUL-terminated or long
// (>=20), is alpha-majority (>=60%), and contains a vowel -- the three gates that reject genuine code
// whose bytes happen to be printable (e.g. an `[esp+disp]` compare chain decoding as `D$ ;D$$u`).
// ~100% precision on the Win95 corpus, ~86% recall (short vowel-less tags like `CD-XA001` are missed).
py::list<std::tuple<int64_t, int64_t>> _string_ranges(const py::bytes& code);

// one recovered instruction (what `insns` holds, as dicts)
struct Row {
    int64_t off = 0, size = 0;
    py::str mnem, ops;
    py::bytes raw;
    std::optional<le::Fixup> fx;                                        // the fix-up inside the instruction
    std::optional<std::tuple<int64_t, int64_t, py::str>> vxdcall;       // (device, service, via)
};

class VxDDisasm {
  public:
    explicit VxDDisasm(const le::LEFile& v);

    const le::LEFile* v;
    py::dict<Place, le::Fixup> fx;
    std::optional<le::Ddb> ddb;
    py::dict<int64_t, py::bytes> code;
    py::dict<int64_t, le::Object> objs;
    py::dict<Place, Row> insns;                                         // (oi, off) -> row
    PlaceSet mark;                                                      // bytes that are code
    PlaceSet data;                                                      // bytes KNOWN to be data (tables, DDB) -- never code
    py::dict<Place, py::str> labels;                                    // (oi, off) -> label name
    py::dict<Place, py::list<Place>> jtables;                           // (oi, base) -> [(oi, target), ...]
    std::optional<std::tuple<int64_t, int64_t, int64_t>> svctab;        // (oi, base, count) if a service table was found
    PlaceSet entries;                                                   // authoritative entry points (DDB procs + service
    //                                                                     table) -- no instruction may SPAN one

    std::optional<le::Fixup> fsite(int64_t oi, int64_t off) const;
    // True iff [a, a+sz) is entirely unclaimed -- so decoding here creates no overlap.
    bool _free(int64_t oi, int64_t a, int64_t sz) const;
    void _mark_data(int64_t oi, int64_t start, int64_t length);

    // A relocation always targets a clean address (a label the linker resolves), never the
    // middle of a real instruction. So an INTERNAL fixup target that lands INSIDE a decoded
    // instruction proves that 'instruction' is actually DATA the linear sweep mis-decoded (a
    // pointer/variable in a data region). Reclassify the whole covering instruction as data --
    // drop it from insns and mark its bytes -- so _collect_labels can place a label at the
    // target and the loadable emit preserves the region's bytes / emits `dd offset` for its
    // pointers instead of re-encoding it as an instruction. Iterate: reclassifying one span can
    // expose targets a longer mis-decode had shadowed.
    void _remark_data_from_fixups();
    // A run of consecutive INTERNAL-fixup SOURCES at 4-byte stride is a pointer table (dispatch
    // table, vtable, handler array): each source dword is a relocated pointer, i.e. DATA.
    // Reclassify the whole run as data so the walk emits it through `_loadable_data` (a `dd offset`
    // per entry = one real relocation each). A no-op when the table is already data.
    void _remark_pointer_tables();
    // A single INTERNAL fixup SOURCE dword [S,S+4) is a relocated pointer -- four contiguous DATA
    // bytes. If the linear sweep decoded code that only PARTIALLY overlaps it, reclassify every
    // instruction straddling such a dword as data and mark the dword's bytes data, so one clean
    // `dd offset` is emitted. A dword fully inside ONE instruction's operand is the normal relocation.
    void _remark_split_fixup_sources();

    // ---- recursive descent
    VxDDisasm& run();
    // Overlay real names from the DDK onto the anonymous sub_/loc_ labels: control-dispatch
    // handlers get the message they service (DEVICE_INIT, SYS_VM_INIT, ...), and the 12 core VxDs
    // with a debug .SYM get their real internal function names where a symbol's offset lands on a
    // recovered function start.
    void _apply_ddk_names();
    // Find plausible code entry points the descent missed: a fixup target or a function
    // prologue (55 8B EC) sitting in an unreached, non-DDB gap of an executable object.
    py::list<Place> _gap_seeds();
    // True when object oi is a 32-bit (BIG) segment: it is decoded as 32-bit, else as 16-bit
    // (a VxD's RCOD real-mode init, or any 16:16 segment).
    bool _is32(int64_t oi) const;
    // Cheap plausibility gate for a gap seed: a valid instruction decodes and it isn't a run
    // of NUL padding (which decodes as `add [eax],al` forever).
    bool _looks_code(int64_t oi, const py::bytes& code, int64_t t) const;
    py::list<Place> _seeds();
    void _descend(const py::list<Place>& seeds);
    // Queue a branch target. A relocated (cross-object) branch has rel32=0 and a fixup that
    // names the real destination; a plain intra-object branch uses the disassembled operand.
    void _follow(int64_t oi, const py::str& op_str, const std::optional<le::Fixup>& fx, std::vector<Place>& work);
    void _jump_table(int64_t oi, int64_t a, int64_t size, std::vector<Place>& work);
    void _collect_labels();
    // object-qualified: two objects can share an offset (both often start at 0), so the bare
    // offset is ambiguous across segments -- qualify with the object to keep labels unique.
    py::str _lname(const Place& key, const std::unordered_set<Place, PlaceHash>& calltgt) const;
};

// The operand of a direct branch ("0x401000") as a number, or None.  (The Python's _direct_target takes the
// capstone instruction; this takes its operand text.)
std::optional<int64_t> _direct_target(const py::str& op_str);
std::optional<int64_t> _direct_target_row(const Row& row);

py::str _svc_name(int64_t dev, int64_t svc, const py::str& via);

// Emit annotated 32-bit assembly for the whole VxD (accepts an LEFile).
py::str emit_asm(const le::LEFile& s_v);
// The same for a disassembly already run (emit_asm(v) = emit_asm(VxDDisasm(v).run(), v)).
py::str emit_asm(const VxDDisasm& D, const le::LEFile& s_v);

// Return the operand text of a row and whether it resolved to a label: a branch to a labelled
// location renders as that label -- for a cross-object branch (rel32=0 + fixup) the label comes
// from the fixup target -- so the listing reads as symbolic control flow rather than raw offsets.
std::tuple<py::str, bool> _render_ops(const VxDDisasm& D, int64_t oi, const Row& row);
py::str _fx_sym(const VxDDisasm& D, const le::Fixup& f);
// Emit a data span [start,end): the DDB structured, jump tables as `dd loc_X`, dword-fixup
// runs as symbol pointers, and everything else as raw bytes.
py::list<py::str> _emit_data(const VxDDisasm& D, int64_t oi, int64_t start, int64_t end);

// verify()'s per-object answer
struct VerifyRow {
    int64_t obj = 0, n = 0, code = 0, data = 0, overlap = 0, overrun = 0;
};
// Faithfulness check: the disassembly is a VIEW of the real bytes, so every executable byte
// must be accounted for exactly once -- claimed by exactly one decoded instruction (code) or by
// none (data). Overlapping instructions mean a mis-seeded entry point disassembled bytes that
// are really mid-instruction; a byte-range past vsize means over-read. Returns per-object
// {n, code, data, overlap, overrun} -- overlap==overrun==0 across the module is the proof.
py::list<VerifyRow> verify(const le::LEFile& v);

// disassemble_file()'s per-module stat dict
struct FileStat {
    py::str name;
    int64_t insns = 0;
    double cov = 0;
    int64_t labels = 0, jtables = 0;
    std::optional<py::str> device;
    std::optional<int64_t> device_id;
};
// Disassemble a VxD file (LE, LX, or a W3 bundle of several) and, if outdir is given, write
// one <module>.asm per embedded module. Returns a list of per-module stat dicts.
py::list<FileStat> disassemble_file(const py::str& path, const std::optional<py::str>& outdir = std::nullopt);

}  // namespace codebreaker::source::vxd::disassembly
