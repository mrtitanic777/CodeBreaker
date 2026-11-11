// codebreaker/formats/pe_functions.hpp -- typed C++ from codebreaker.formats.pe_functions.
//
// pe_trace.py -- the 32-bit (PE) function carve: `trace.Seg` for the PE world.
//
// `trace.Seg` carves 16-bit NE segments via objdump; this does the same job for the target's PE modules
// (e.g. KERNEL32/USER32/GDI32) using **capstone** (32-bit x86). It gives the PE side what the NE side
// already has: function starts, per-function extents, and *resolved* calls -- so a reconstruction can cite
// `0xADDR` and be checked, and the ledger can one day credit PE bytes.
//
// Function starts come from three sources, most-authoritative first:
//   1. the export table (RVAs the binary itself declares) -- ground truth;
//   2. direct `call <imm>` targets seen in a linear sweep -- real entry points;
//   3. standard prologues: `push ebp; mov ebp,esp` (55 8B EC) and the MS hot-patch
//      `mov edi,edi; push ebp; mov ebp,esp` (8B FF 55 8B EC).
// Extents run start-to-next-start (as in trace.Seg).
//
// Calls resolve via `pe.imports()` (a `call [IAT_slot]` -> `DLL!func`) and the export/known-start set (a
// `call <imm>` -> a named export or a local `sub_VA`). That is what makes the carve checkable rather than a
// raw listing.
//
// Doctrine: this is *evidence extraction*, not understanding. It says where the functions are and what they
// call; a human still reads them.
//
// Typing and runtime notes:
//   * The Python asked formats.dll (native/formats/win/pe_functions.cc) through the ctypes bridge for the
//     sweep and the listing; here the same algorithm is written in typed C++ on top of the Capstone DLL
//     (`set_capstone_dll`, the DLL Python's `capstone` package carries; a use without it is an ImportError,
//     as a missing `capstone` module is).
//   * Capstone's Python instruction object is the struct `Instruction` (address, size, mnemonic, op_str and
//     its operands: type, imm, mem base/index/disp); a decoded record of `_Insns` is the struct `Record`.
//   * `_Insns` (a mapping address -> record) is the class `Insns`; keys/items/values return vectors.
//   * the dicts with fixed keys of a carved function ({"start", "end", "name"}) are the struct `Func`.
//   * `if progress_cb:` is an empty-std::function test.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "codebreaker/formats/pe.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::pe_functions {

// The Capstone DLL (capstone.dll of Python's `capstone` package), loaded by path on first use.
void set_capstone_dll(const py::str& path);

// capstone.x86 operand types
constexpr int64_t X86_OP_IMM = 2;
constexpr int64_t X86_OP_MEM = 3;

// one operand of Capstone's x86 detail: its type, an immediate, and a memory operand's base/index/disp
struct Operand {
    int64_t type = 0;        // X86_OP_INVALID 0, X86_OP_REG 1, X86_OP_IMM 2, X86_OP_MEM 3
    int64_t imm = 0;         // operand.imm (signed)
    int64_t base = 0;        // operand.mem.base (0: none)
    int64_t index = 0;       // operand.mem.index (0: none)
    int64_t disp = 0;        // operand.mem.disp (signed)
};

// Capstone's instruction (CsInsn) with its operands
struct Instruction {
    int64_t address = 0, size = 0;
    py::str mnemonic, op_str;
    std::vector<Operand> operands;
    int64_t disp = 0;                                   // CsInsn.disp (the memory operand's displacement)
    int64_t disp_size = 0, disp_offset = 0;             // CsInsn.disp_size / disp_offset: where the displacement is in the bytes
    int64_t imm_size = 0, imm_offset = 0;               // CsInsn.imm_size / imm_offset: where the immediate is
};

// (mnemonic, op_str, size, Capstone's instruction, call target)
struct Record {
    py::str mnemonic, op_str;
    int64_t size = 0;
    Instruction ins;
    std::optional<int64_t> tgt;       // the immediate of a `call imm`
};

class PeCarve;

// {address: (mnemonic, op_str, size, Capstone's instruction, call target)} of every address the sweep decoded:
// the addresses come from the sweep, an instruction is decoded (by Capstone, as the sweep did) when it is asked for.
class Insns {
  public:
    Insns(const PeCarve* carve, const std::vector<int64_t>& addresses);
    bool contains(int64_t va) const;
    int64_t size() const;
    // the addresses, ascending (__iter__ / keys)
    std::vector<int64_t> keys() const;
    // self[va]: KeyError when the address was not decoded or lies in no section; StopIteration when Capstone
    // decodes nothing there
    Record at(int64_t va) const;
    // .get(va): None when the address was not decoded
    std::optional<Record> get(int64_t va) const;
    std::vector<std::pair<int64_t, Record>> items() const;
    std::vector<Record> values() const;

  private:
    const PeCarve* carve_;
    py::set<int64_t> addresses_;
    std::vector<int64_t> sorted_;
};

// the function PeCarve carved at `start`
struct Func {
    int64_t start = 0, end = 0;
    std::optional<py::str> name;
};

// (base, bytes) of the section holding an address; base is None (and bytes null) when there is none
struct SecOf {
    std::optional<int64_t> base;
    const py::bytes* data = nullptr;
};

class PeCarve {
  public:
    // progress_cb(bytes_swept) is called when the sweep is done (formats.dll sweeps whole)
    explicit PeCarve(const py::str& path, std::function<void(int64_t)> progress_cb = nullptr);
    PeCarve(const PeCarve&) = delete;
    PeCarve& operator=(const PeCarve&) = delete;

    std::function<void(int64_t)> _progress_cb;
    std::shared_ptr<pe::PEFile> pe;
    py::list<std::tuple<uint64_t, py::bytes, py::str>> sections;     // [(va, bytes, name)]
    py::dict<uint64_t, std::tuple<py::str, py::str>> imports;        // {iat_va: (dll, name)}
    py::dict<int64_t, py::str> exports;                              // export_va -> name
    py::set<int64_t> call_targets;
    std::shared_ptr<Insns> insn;
    py::dict<int64_t, Func> funcs;                                   // start -> function (ascending start)
    py::set<int64_t> starts;

    // -- section helpers
    SecOf _sec_of(int64_t va) const;

    // -- queries
    // A capstone call/jmp insn -> a name, or None if not a call/branch.
    std::optional<py::str> resolve_call(const Instruction& ins) const;
    // Resolved call/jmp targets inside the function carved at `start`.
    py::list<py::str> calls_in(int64_t start) const;
    py::list<std::tuple<int64_t, py::str, py::str>> disasm_func(int64_t start) const;

    // -- data cross-references
    // `resolve_call` gives the control-flow edges; these give the *data* edges: which function reads/pushes the
    // address of a string or global. A `push imm32`, `mov reg,imm32`, or absolute `[disp]` whose value lands
    // inside the image is a data reference -- the join that answers "who references this string?" without
    // which a literal cannot be traced back to its caller.
    py::list<std::tuple<int64_t, int64_t>> _image_ranges() const;
    bool _in_image(int64_t va) const;
    // {func_start: sorted[referenced data VA]}.
    //
    // Records IMM and absolute-`[disp]` operands that land in the image, except a branch's direct target (a
    // code edge, owned by resolve_call/calls_in) and recognized IAT slots (import edges, owned by resolve_call).
    const py::dict<int64_t, py::list<int64_t>>& data_refs() const;
    // Sorted function-starts that reference data address `va`.
    py::list<int64_t> xrefs_to(int64_t va) const;
    // {func_start: [(va, text), ...]} -- the strings each function references.
    // `va2text` is a {VA: string} map (see format_loaders.pe_strings.by_va).
    py::dict<int64_t, py::list<std::tuple<int64_t, py::str>>> string_refs(const py::dict<int64_t, py::str>& va2text) const;

  private:
    mutable std::optional<py::list<std::tuple<int64_t, int64_t>>> imgr_;
    mutable std::optional<py::dict<int64_t, py::list<int64_t>>> drefs_;
    mutable std::optional<py::dict<int64_t, py::list<int64_t>>> drev_;
};

// Point cores at win95 + cd into a data root, idempotently (mirrors msvc2cov._setup so a tool works with no
// `wt` wrapper).
void ensure_target();

// one PeCarve per module, kept
std::shared_ptr<PeCarve> carve(const py::str& module);

// Linearly decode the blob as 32-bit code with capstone; return [(offset, asm_text), ...] with CONTIGUOUS
// coverage (undecodable bytes become a 1-byte '(bad)' row -> db). capstone is used instead of objdump because
// objdump's linear sweep MIS-SPLITS prefixed instructions (a segment/opsize prefix like `64`/`66` gets peeled
// onto its own row), which the byte-exact reconcile then cannot realign. capstone decodes each prefixed
// instruction as one unit with the correct length, so the reconcile converges cleanly.
//
// progress_cb(bytes_done) is called when the listing is done (formats.dll lists whole).
py::list<std::tuple<int64_t, py::str>> disasm_whole32(const py::bytes& code, int64_t base = 0,
                                                      std::function<void(int64_t)> progress_cb = nullptr);

// One PE executable section, shaped like trace.Seg for asmrecon32.
class PeSeg {
  public:
    // carve: a pre-built PeCarve to REUSE (a multi-section PE otherwise re-runs the whole capstone sweep once
    // per section). progress_cb drives the per-section disassembly bar.
    explicit PeSeg(const py::str& module, std::optional<py::str> section = std::nullopt,
                   std::shared_ptr<PeCarve> carve = nullptr, std::function<void(int64_t)> progress_cb = nullptr);

    py::str module, path;
    std::shared_ptr<pe::PEFile> pe;
    uint64_t base = 0;
    int64_t va = 0;
    py::bytes code;
    py::str name;
    int64_t secva = 0;                                   // image VA of section start
    py::list<std::tuple<int64_t, py::str>> rows;
    py::dict<int64_t, int64_t> off2idx;
    py::dict<int64_t, Func> funcs;
    py::dict<int64_t, py::str> export_names;
    std::shared_ptr<PeCarve> _carve;                     // kept for call resolution/annotate
    py::set<int64_t> mask;                               // base-relocation mask, section-relative
    py::dict<int64_t, py::str> fixups;                   // resolved call sites for annotate (site_off -> "DLL!name")
};

// {start: (end, exported name or None)} -- the functions PeCarve finds, by the same rule and with the same
// answer, without keeping an object for every instruction of the module.
py::dict<int64_t, std::tuple<int64_t, std::optional<py::str>>> function_spans(const py::str& path);

}  // namespace codebreaker::formats::pe_functions
