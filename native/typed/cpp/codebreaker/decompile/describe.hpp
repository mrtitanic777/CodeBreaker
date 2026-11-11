// codebreaker/decompile/describe.hpp -- interface of codebreaker.decompile.describe (typed C++; implementation pending).
//
// describe.py -- take an executable apart and describe it to the decompiler.
//
//     describe32(module_path, addresses, workdir)   a PE module (x86 or PowerPC)
//     describe16(module_path, workdir)              a 16-bit Windows module (NE)
//     describe_le(image, name, workdir)             a virtual device driver (LE), one member at a time
//     describe_dos(module_path, workdir)            a real-mode program (MZ, .COM, driver, IO.SYS)
//     describe_nes(module_path, workdir)            a NES cartridge
//
// A description says which processor, where the file's bytes sit in memory, and the facts read from the binary
// rather than guessed: every function's start and name, how it returns (far or near, how many bytes of arguments it
// removes), and every import with the same for it.  A 16-bit module is a set of segments the loader places: each
// segment gets a selector (segment 1 -> 0x1000, then every 8) and so an address (selector * 0x10000).
//
// Typing notes:
//   * Description is ONE class for every kind of module.  The Python attaches attributes to the object as the
//     describer of that kind makes them (`d.code`, `d.images` ... for NE; `d.spans`, `d.device` for LE; `d.spans`,
//     `d.image`, `d.base`, `d.program`, `d.analysis`, `d.interrupts` for DOS): here they are members, empty (or
//     absent, for the optional ones) in a description of another kind.  An empty `images` is the Python's
//     `getattr(d, "images", None)` being None or {}.
//   * `where` is the module's {address: (segment, offset)} (16-bit only); `names` {address: function name}.
//     `imports` is the 16-bit module's import names in the order of their stand-in addresses (EXTERN_SELECTOR:16*n);
//     [] for the others.
//   * `notes` is {function name: FunctionNotes} (core16_types.hpp, group 4), written by the decompiler.
//   * The decoder (platforms.decoder(16, detail)) has no port: `Decoder` below is its stand-in, a thin class around
//     capstone's 16-bit x86 decoder, and `Insn` the decoded instruction with what describe.py reads of it
//     (capstone's CsInsn: mnemonic, op_str, address, size, bytes, operands, regs_access, reg_name).
//   * A 16-bit `code` argument is py::bytes (the Python also passes bytearray / memoryview slices: take a copy).
//   * An address (module-level `address(seg, off)`) is int64_t; a place "(seg, off)" is core16::SegOff.
//   * BY_NUMBER and IN_ECX are the module-level registries the callers read: extern, mutable.
//   * Description is pickled by verify/gate16/contract16 (cache): the .cpp should offer a to/from-text form for
//     `lines`+members if that cache is kept (see contract16.hpp).
//   * hidden_functions(known): `known` is a set of (segment, offset) (a dict's keys also work in the Python).
//   * forwards()/callers_read_dx() etc. take the Description; the tests (tests/test_verification.py) also pass a
//     SimpleNamespace(names, images) to _DxReturns: construct a Description with just those members.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/analyze/dos_functions.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/decompile/inline_asm.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/formats/ne.hpp"
#include "py.hpp"

namespace codebreaker::decompile::describe {

using core16::FunctionNotes;
using core16::SegOff;

// --- the 16-bit address space -------------------------------------------------------------------------------------
inline constexpr int64_t FIRST_SELECTOR = 0x1000;
inline constexpr int64_t EXTERN_SELECTOR = 0xE000;    // imported functions stand at EXTERN_SELECTOR:16*n
inline constexpr int64_t IMPORT_SELECTOR = 0xD000;    // an import used as a selector (KERNEL's __0040H ...): IMPORT_SELECTOR + n
inline constexpr int64_t LE_BASE = 0x1000000;         // LE: object n at LE_BASE + (n-1) * LE_STRIDE
inline constexpr int64_t LE_STRIDE = 0x1000000;
inline constexpr int64_t LE_SLOTS = 0x0F000000;       // one pointer per service called, read as that service

// the decompiler's language id of each kind (platforms.named(kind).cpu)
extern const py::str ARCH;       // "ne"
extern const py::str LE_ARCH;    // "le"
extern const py::str DOS_ARCH;   // "dos"

// The decompiler's language for a PE image of this machine type (its platform's), or None.   (used by tests)
std::optional<py::str> _arch(int64_t machine);

// module file -> {the C name an import by number is called by: (MODULE, number, bytes of arguments it removes or None)}
// -- what describe32 decided, for whoever declares and links those calls
extern py::dict<py::str, py::dict<py::str, std::tuple<py::str, int64_t, std::optional<int64_t>>>> BY_NUMBER;
// module file -> names of its functions that take an argument in ECX
extern py::dict<py::str, py::set<py::str>> IN_ECX;

int64_t selector(int64_t seg);
int64_t address(int64_t seg, int64_t off);
// (segment, offset) of an address inside the module's segments.
SegOff where(int64_t va);
int64_t le_address(int64_t obj, int64_t off);

// --- decoded instructions (stand-in for platforms.decoder(16, detail)) ---------------------------------------------
// One decoded 16-bit x86 instruction: capstone's CsInsn as describe.py reads it.  `operands` are filled only for a
// decoder made with detail=true.
// capstone's error: raised where the Python's `regs_access()` raises CsError (the registers of an instruction are not
// available).
struct CsError : py::Exception {
    explicit CsError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "CsError"; }
};
// The decoder is the very capstone.dll Python's `capstone` package carries, loaded by path (as analyze/dos_functions
// and source/vxd/disassembly do): hand it over once, before the first decoder16(); a use without it is an ImportError.
// (It also hands the DLL to analyze::dos_functions, which describe_dos goes through.)
void set_capstone_dll(const py::str& path);

// `operands` (and `op_size`, the size of each: capstone's op.size) are filled only for a decoder made with detail=true.
struct Insn : inline_asm::Instruction {
    py::list<int64_t> op_size;        // operands[k].size
    py::list<int64_t> op_access;      // operands[k].access (CS_AC_READ 1, CS_AC_WRITE 2): what asm_blocks reads of an operand
    py::list<int64_t> regs_read;      // regs_access()[0]
    py::list<int64_t> regs_write;     // regs_access()[1]
    py::list<int64_t> groups;         // capstone's instruction groups (CS_GRP_*), for group()
    bool regs_ok = false;             // regs_access() is available (a detail decoder, and capstone could say)
    std::string mn, os;               // mnemonic / op_str as plain text (the same as mnemonic / op_str)
    // i.regs_access(): (registers read, registers written); raises CsError when capstone cannot say
    std::tuple<py::list<int64_t>, py::list<int64_t>> regs_access() const {
        if (!regs_ok) throw CsError("Information irrelevant for this instruction (CS_ERR_DETAIL)");
        return {regs_read, regs_write};
    }
    // i.group(g): is the instruction in group `g` (CS_GRP_JUMP ...)?
    bool group(int64_t g) const {
        for (int64_t x : groups) if (x == g) return true;
        return false;
    }
};
// capstone's generic instruction groups (as capstone 5 numbers them)
inline constexpr int64_t CS_GRP_JUMP = 1;
inline constexpr int64_t CS_GRP_CALL = 2;
inline constexpr int64_t CS_GRP_RET = 3;
inline constexpr int64_t CS_GRP_INT = 4;
inline constexpr int64_t CS_GRP_IRET = 5;
class Decoder {
  public:
    // md.disasm(code, address): the instructions from the start of `code` (the Python's generator, run to the end)
    virtual py::list<Insn> disasm(const py::bytes& code, int64_t address) const = 0;
    // i.reg_name(register id)
    virtual py::str reg_name(int64_t reg) const = 0;
    virtual ~Decoder() = default;
};
// A 16-bit x86 decoder (one of each kind per thread and kept). `detail`: with each instruction's operands and registers.
std::shared_ptr<const Decoder> decoder16(bool detail = false);
// The 16-bit instruction at `at` in `code`, or None (null).  The decoded instructions of a segment are kept by the
// segment's bytes (test: the same segment gives the same object).
std::shared_ptr<const Insn> _one(const py::bytes& code, int64_t at);

// --- what describes a module ------------------------------------------------------------------------------------
// What is said of a module before it is decompiled. `lines` are the description itself -- its facts, one to a line
// (F a function, X an import, O a call's convention, bytes, reg ...); the file at `path` is written from them (write)
// for the native decompiler, which reads it.
class Description {
  public:
    py::str path;
    py::dict<int64_t, py::str> names;           // {address: function name}
    py::dict<int64_t, SegOff> where;            // {address: (segment, offset)} (NE only)
    py::list<py::str> imports;                  // NE: import names by stand-in address order
    py::list<py::str> lines;
    py::dict<py::str, FunctionNotes> notes;   // {function: its facts from the decompiler} (decompiler.split_notes)

    // --- NE (describe16)
    py::set<SegOff> far_called;                 // (seg, off) a far call reaches
    py::dict<py::str, py::str> import_names;    // symbol -> the import's real name
    py::dict<int64_t, std::tuple<py::str, py::str>> selector_imports;   // made-up value -> (import symbol, "segment" | "offset")
    py::dict<int64_t, int64_t> code;            // segment -> bytes of code
    py::dict<int64_t, py::bytes> images;        // segment -> its bytes, relocated
    int64_t data_segment = 0;
    py::dict<int64_t, py::bytes> segment_bytes; // segment -> the bytes as the loader has them (stored, then zeros)
    py::dict<int64_t, int64_t> results;         // address -> bytes it returns (results())
    py::set<int64_t> inlined;                   // helpers made part of their callers (addresses)

    // --- LE (describe_le) and DOS (describe_dos): object/part number -> (address, length)
    py::dict<int64_t, std::tuple<int64_t, int64_t>> spans;
    std::optional<py::str> device;              // LE: the driver's device name

    // --- DOS (describe_dos)
    py::bytes image;                            // the program's bytes as they sit in memory
    int64_t base = 0;                           // its linear base
    std::optional<formats::dos::Program> program;
    std::shared_ptr<analyze::dos_functions::Analysis> analysis;
    py::dict<py::str, py::list<analyze::dos_functions::Interrupt>> interrupts;   // function name -> its interrupt calls

    Description(const py::str& path, const py::dict<int64_t, py::str>& names, const py::dict<int64_t, SegOff>& where,
                const py::list<py::str>& imports);

    // The file the native decompiler reads, written from the lines (at `path`: there from now on). -> the path
    py::str write(const std::optional<py::str>& path = std::nullopt);
    // [fields of each line] of those kinds (`F`, `O` ...), in order.
    py::list<py::list<py::str>> records(const py::list<py::str>& kinds) const;
};

// Is `va` in one of the described module's code segments?
bool inside(const Description& d, int64_t va);

// --- PE (and PowerPC) ----------------------------------------------------------------------------------------------
// {va: name} -- an export's own name, else sub_<rva>. A name also used by an import stays with the import.
py::dict<int64_t, py::str> function_names(const py::str& module_path, const py::list<int64_t>& funcs);
// {address: linker name} for the functions a module exports under a name that is not a C name (a C++ decorated name).
py::dict<int64_t, py::str> decorated_names(const py::str& module_path);
// {symbol: name} for a 16-bit module's imports by number: the symbol the C text uses (`USER_91`) and the real name.
py::dict<py::str, py::str> import_names16(const formats::ne::NEFile& nef);
// Addresses of a PE module's functions (PowerPC: the .pdata table; x86: found by following the code), sorted.
py::list<int64_t> function_starts(const py::str& module_path);
// Write the description of a PE module for the native program. -> (path of the description, {va: name})
std::tuple<py::str, py::dict<int64_t, py::str>> describe32(const py::str& module_path, const py::list<int64_t>& funcs,
                                                          const py::str& outdir);

// --- NE: finding functions and how they return -------------------------------------------------------------------
// {(segment, offset): "far" | "near"} -- every function start that can be established.
py::dict<SegOff, py::str> functions(const formats::ne::NEFile& nef);
// Entry sequences after an end that are not among `known` {(segment, offset)}.
py::list<SegOff> hidden_functions(const formats::ne::NEFile& nef, const py::set<SegOff>& known);
// Offsets in one code segment where an entry sequence follows the end of code.
py::list<int64_t> prologues_after_ends(const py::bytes& code);
// Is ins[k] a return that is a jump (the instructions straight before it push exactly what it takes off)?
bool jump_by_return(const py::list<Insn>& ins, int64_t k, const py::set<int64_t>& targets = py::set<int64_t>());
// [(address of a return that is a jump)] in every code segment.
py::list<int64_t> return_jumps(const Description& d);
// How the function at `off` returns: ("far" | "near" | None, bytes of arguments removed; -1 when none met within `limit`).
std::tuple<std::optional<py::str>, int64_t> returns(const py::bytes& code, int64_t off, int64_t limit = 600,
                                                    const std::optional<py::str>& want = std::nullopt);
// {(seg, off): "callee" | "caller"} -- who removes a function's arguments, as the original's own calls show.
py::dict<SegOff, py::str> callers_say(const Description& d);
// Bytes of arguments a thunk's caller has removed, for a thunk that puts more arguments under its return address and
// jumps on, or None.
std::optional<int64_t> forwards(const Description& d, int64_t seg, int64_t off, const py::str& kind);
// Bytes of arguments a function takes off the stack itself at its start, or None.
std::optional<int64_t> popped_by_hand(const py::bytes& code, int64_t off, const py::str& kind);
// The bytes of arguments a function reads through BP, or None when it keeps no such frame.
std::optional<int64_t> args_read(const py::bytes& code, int64_t start, int64_t end, const py::str& kind);
// Is this the prologue of a far function that Windows calls (exports, callbacks): `mov ax,ds` or `push ds; pop ax`,
// `nop`, `inc bp; push bp` -- the odd BP marking a far frame?
bool windows_far_prologue(const py::bytes& code, int64_t off);

// What a function passes on of its result: another function's (an offset in its segment, or None for a call whose
// callee is not known), or a jump on into the middle of one ("into", offset).
using PassOn = std::variant<py::none_t, int64_t, std::tuple<py::str, int64_t>>;
// What a function returns, by its instructions: 0, 2 or 4 bytes.  `passes` and `voided`, when given, are appended to.
int64_t result_bytes(const py::bytes& code, int64_t start, int64_t end, py::list<PassOn>* passes = nullptr,
                     py::list<int64_t>* voided = nullptr);
// `J` lines: each switch jump through a table in its own code segment.
py::list<py::str> jump_tables(const Description& d);
// `O` lines: for each call through a pointer, the words the code pushed for it just before.
py::list<py::str> call_overrides(const Description& d);
// {address: bytes it returns} for every function described.
py::dict<int64_t, int64_t> results(const Description& d);

// the bytes of arguments the caller takes off right after the call ins[j], or 0   (used by tests)
int64_t _caller_cleanup(const py::list<Insn>& ins, int64_t j);

// Does a function hand back a DX of its own -- on every way to each of its returns? (used by tests)
class _DxReturns {
  public:
    explicit _DxReturns(const Description& d);
    bool every_way(const SegOff& place, const py::set<SegOff>& four);

  private:
    const Description* d_;
    std::shared_ptr<const Decoder> md_;
    py::dict<int64_t, py::list<int64_t>> starts_;
    // the Python's private caches and helper methods (_function, _target, _saves_ax, _gives_back_ax) stay in the .cpp
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

// {(seg, off)} of the functions a call to which is followed, before anything else could set it, by a read of DX.
// `edges`, when given, has [(caller, callee)] added.
py::set<SegOff> callers_read_dx(const Description& d, py::list<std::tuple<SegOff, SegOff>>* edges = nullptr);
// Is the function at `off` a helper whose effect is on its caller's registers (a short straight run to a plain `ret`
// that leaves SI, DI or DS changed)?
bool register_helper(const py::bytes& code, int64_t off, const std::optional<int64_t>& end = std::nullopt);
// {(seg, off)} of `places` a call to which is followed, before anything sets it, by a read of AX.
py::set<SegOff> callers_read_ax(const Description& d, const py::set<SegOff>& places);
// Whether the function at `off` takes arguments in AX, DX or BX (Microsoft C's _fastcall).
bool register_arguments(const py::bytes& code, int64_t off);
// The description line for a function at `va`.
py::str model_line(const Description& d, int64_t va, const py::str& name, const py::str& dflt = py::str("near"));

// --- the writers ----------------------------------------------------------------------------------------------------
// Write the description (and the relocated image it points at). -> Description (shared: callers keep it)
std::shared_ptr<Description> describe16(const py::str& module_path, const py::str& outdir);
// Describe one LE image to the decompiler. -> Description
std::shared_ptr<Description> describe_le(const formats::le::LEFile& lf, const py::str& name, const py::str& outdir);
// Describe a real-mode program to the decompiler. -> Description
std::shared_ptr<Description> describe_dos(const py::str& module_path, const py::str& outdir);
// Write the description of a NES cartridge. -> (path of the description, {va: name})
std::tuple<py::str, py::dict<int64_t, py::str>> describe_nes(const py::str& module_path, const py::str& outdir);

}  // namespace codebreaker::decompile::describe
