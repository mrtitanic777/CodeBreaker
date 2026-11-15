// cb/decompile/describe.hpp -- take an executable apart and describe it to the decompiler.
//
//     describe32(module, image, functions, outdir, knowledge, ppc)   a PE module (x86 or PowerPC)
//     describe16(module, outdir, knowledge)                          a 16-bit Windows module (NE)
//     describe_le(module, name, outdir, knowledge)                   a virtual device driver (LE)
//     describe_dos(program, outdir)                                  a real-mode program (MZ, .COM, driver, IO.SYS)
//     describe_nes(rom, functions, outdir)                           a NES cartridge
//
// A description says which processor, where the file's bytes sit in memory, and the facts read from the binary
// rather than guessed: every function's start and name, how it returns (far or near, how many bytes of arguments it
// removes), and every import with the same for it. It is written as the text file the native decompiler reads: one
// fact to a line, tab separated (F a function, X/I an import, O a call's convention, J a switch table, R a return that
// is a jump, `bytes` where the file's bytes sit, `reg`, `arch`). A 16-bit module is a set of segments the loader
// places: each segment gets a selector (segment 1 -> 0x1000, then every 8) and so an address (selector * 0x10000).
//
// What a description asks of the rest of CodeBreaker -- the symbol tables (what an API removes from the stack, what a
// number imported by ordinal is called), the calling-convention analysis of a PE image -- it asks through `Knowledge`,
// so this module reads nothing but the binary it is given; `NoKnowledge` answers nothing (a test, or a module whose
// imports are not wanted). The switches that were environment variables are the fields of `Policy`.
//
// 16-bit instructions are decoded by Capstone (cb/platforms): `Code16` is one segment's bytes with each instruction
// decoded once, since finding functions, their returns and the stubs in front of them walks the same bytes again and
// again. A Code16 and a description that holds them belong to one thread.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include "cb/analyze/dos_functions.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core16_types.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/le.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/nes.hpp"
#include "cb/formats/pe.hpp"

namespace cb::decompile::describe {

using core16::FunctionNotes;
using core16::SegOff;
using Va = std::int64_t;

// --- the address spaces ---------------------------------------------------------------------------------------------
inline constexpr Va kFirstSelector = 0x1000;
inline constexpr Va kExternSelector = 0xE000;  // imported functions stand at kExternSelector:16*n
inline constexpr Va kImportSelector = 0xD000;  // an import used as a selector (KERNEL's __0040H ...): kImportSelector + n
inline constexpr Va kLeBase = 0x1000000;       // LE: object n at kLeBase + (n-1) * kLeStride
inline constexpr Va kLeStride = 0x1000000;
inline constexpr Va kLeSlots = 0x0F000000;     // one pointer per service called, read as that service

// the decompiler's language id of each kind (what the platform table calls its cpu)
inline constexpr std::string_view kArchNe = "x86:LE:16:Protected Mode:default";
inline constexpr std::string_view kArchLe = "x86:LE:32:default:windows";
inline constexpr std::string_view kArchDos = "x86:LE:16:Real Mode:default";
inline constexpr std::string_view kArchNes = "6502:LE:16:NES:default";

// The decompiler's language for a PE image of this machine type, or nullopt.
std::optional<std::string_view> language_of_machine(std::uint16_t machine);

constexpr Va selector(Va seg) { return kFirstSelector + 8 * (seg - 1); }
constexpr Va address(Va seg, Va off) { return (selector(seg) << 16) + off; }
// (segment, offset) of an address inside the module's segments.
SegOff where(Va va);
constexpr Va le_address(Va obj, Va off) { return kLeBase + (obj - 1) * kLeStride + off; }

// --- what a description asks of the rest of CodeBreaker -------------------------------------------------------------
class Knowledge {
public:
    virtual ~Knowledge() = default;
    // Bytes of arguments a Win16 API removes (read from the module that exports it); nullopt when not known.
    virtual std::optional<std::int64_t> win16_argument_bytes(std::string_view api) { (void)api; return std::nullopt; }
    // The same for a Win32 export (by name, or "#n" for a number) of the module with this stem ("KERNEL32").
    virtual std::optional<std::int64_t> win32_argument_bytes(std::string_view module_stem, std::string_view api) {
        (void)module_stem; (void)api;
        return std::nullopt;
    }
    // Bytes of arguments a function's declaration in the SDK headers gives.
    virtual std::optional<std::int64_t> declared_argument_bytes(std::string_view name) { (void)name; return std::nullopt; }
    // The C name an export of `module` has by its number, when it has one.
    virtual std::optional<std::string> import_name(std::string_view module, std::uint32_t ordinal) {
        (void)module; (void)ordinal;
        return std::nullopt;
    }
    // The label of a virtual device's service (the real name, else a placeholder).
    virtual std::string driver_service_label(std::uint32_t device, std::uint32_t service);
    // The names a PE module imports (what its own functions' names must not take).
    virtual std::set<std::string> imported_names(const std::filesystem::path& module) { (void)module; return {}; }
    // What the calling-convention analysis says of the functions of a PE module, by address.
    struct Conventions {
        std::set<Va> thiscall, fastcall;
    };
    virtual Conventions conventions(const std::filesystem::path& module) { (void)module; return {}; }
    // Bytes of arguments the function at `rva` of a PE module removes (read from its returns); nullopt when not found.
    virtual std::optional<std::int64_t> pe_function_pop(const std::filesystem::path& module, std::uint32_t rva) {
        (void)module; (void)rva;
        return std::nullopt;
    }
};
class NoKnowledge : public Knowledge {};

// The switches of the 16-bit describer (each was an environment variable; `from_environment` reads them).
struct Policy {
    bool caller_args = true;       // a call's arguments from the cleanup after it (CB_CALLER_ARGS=0 turns off)
    bool dx_unspecified = false;      // a callee whose callers read DX is four bytes when some way sets DX, the others unspecified (CB_DX_UNSPECIFIED=1; under test)
    bool dx_shared_bodies = false;   // DxReturns follows jumps into other functions' code and calls that never write DX (CB_DX_SHARED=1; under test)
    bool caller_results = false;   // a callee's four-byte result from its callers' reads of DX (CB_CALLER_RESULTS=1)
    bool fastcall = true;          // _fastcall functions (CB_FASTCALL=0 turns off)
    bool inline_helpers = true;    // helpers that change their caller's registers become part of it (CB_INLINE_HELPERS=0)
    bool call_overrides = true;    // `O` lines (CB_NO_CALL_OVERRIDES set turns off)
    static Policy from_environment();
};

// --- decoded 16-bit instructions ------------------------------------------------------------------------------------
enum class OperandKind : std::uint8_t { Invalid, Register, Immediate, Memory };   // Capstone's X86_OP_*

struct Operand {
    OperandKind kind = OperandKind::Invalid;
    std::uint32_t reg = 0;                         // Capstone's register ids
    std::int64_t imm = 0;
    std::uint32_t segment = 0, base = 0, index = 0;
    std::int64_t disp = 0;
    std::int32_t size = 0;                         // the operand's width in bytes
};

// One decoded instruction, as Capstone gives it: `mn` its mnemonic, `op_str` its operands as text (hex as 0x..), what
// registers it reads and writes (when Capstone can say), and which of the generic groups it is in.
struct Insn {
    std::int64_t address = 0;
    std::int64_t size = 0;
    std::string mn, op_str;
    Bytes bytes;
    std::vector<Operand> operands;
    std::vector<std::uint16_t> regs_read, regs_write;
    std::uint32_t groups = 0;                      // bit g set: in group g (kGroupJump ...)
    bool regs_ok = false;                          // regs_read/regs_write are known

    bool in_group(unsigned g) const { return (groups >> g) & 1u; }
    bool imm_first() const { return !operands.empty() && operands[0].kind == OperandKind::Immediate; }
    std::int64_t first_imm() const { return operands[0].imm; }
    std::int64_t first_size() const { return operands.empty() ? 0 : operands[0].size; }
    std::int64_t end() const { return address + size; }
};
// capstone's generic instruction groups
inline constexpr unsigned kGroupJump = 1, kGroupCall = 2, kGroupRet = 3, kGroupInt = 4, kGroupIret = 5;

// One segment's bytes with each instruction decoded once (the instruction at an offset, decoded as if 16 bytes
// followed it).
class Code16 {
public:
    explicit Code16(Bytes bytes) : bytes_(std::move(bytes)) {}
    ByteView bytes() const { return bytes_; }
    std::int64_t size() const { return static_cast<std::int64_t>(bytes_.size()); }
    // The instruction at `at`, or null when none decodes there.
    const Insn* one(std::int64_t at) const;
    // code[a:b] decoded from `a`, clamping the range to the segment (count 0: to the end).
    std::vector<Insn> decode(std::int64_t a, std::int64_t b, bool detail, std::size_t count = 0) const;

private:
    Bytes bytes_;
    mutable std::unordered_map<std::int64_t, std::unique_ptr<Insn>> seen_;   // lookup only; null: not an instruction
};

// The instructions of `code` from address `at`, to the end (what Capstone decodes before the first byte it cannot).
std::vector<Insn> decode16(ByteView code, std::int64_t at, bool detail, std::size_t count = 0);
// Capstone's name of a register id ("" when it has none).
std::string register_name16(std::uint32_t reg);

// --- what describes a module ----------------------------------------------------------------------------------------
// What is said of a module before it is decompiled. `lines` are the description itself; the file at `path` is written
// from them (`write`). The decompiler's notes of each function (decompiler.split_notes) are kept by function name.
struct Description {
    std::filesystem::path path;
    std::map<Va, std::string> names;               // {address: function name}
    std::vector<std::string> lines;
    std::map<std::string, FunctionNotes> notes;

    // The file the native decompiler reads, written from the lines (at `to`: there from now on).
    void write(const std::optional<std::filesystem::path>& to = std::nullopt);
    // The fields of each line of the given kinds (`F`, `O` ...), in order.
    std::vector<std::vector<std::string>> records(const std::vector<std::string_view>& kinds) const;
};

enum class Distance { Near, Far };
inline std::string_view distance_name(Distance d) { return d == Distance::Far ? "far" : "near"; }

// How a made-up selector or offset value stands for an import of a 16-bit module.
enum class SelectorUse { Segment, Offset };
struct SelectorImport {
    std::string symbol;
    SelectorUse use = SelectorUse::Segment;
};

// A 16-bit module (describe16).
struct NeDescription : Description {
    std::map<Va, SegOff> where_map;                      // {address: (segment, offset)}
    std::vector<std::string> imports;                    // import names by stand-in address order (kExternSelector:16*n)
    std::set<SegOff> far_called;                         // (seg, off) a far call reaches
    std::map<std::string, std::string> import_names;     // symbol -> the import's real name
    std::map<Va, SelectorImport> selector_imports;       // made-up value -> (import symbol, "segment" | "offset")
    std::map<std::int64_t, std::int64_t> code;           // segment -> bytes of code
    std::map<std::int64_t, Bytes> images;                // segment -> its bytes, relocated
    std::int64_t data_segment = 0;
    std::map<std::int64_t, Bytes> segment_bytes;         // segment -> the bytes as the loader has them (stored, then zeros)
    std::map<Va, std::int64_t> results;                  // address -> bytes it returns (results())
    std::set<Va> inlined;                                // helpers made part of their callers (addresses)
    Policy policy;

    // Is `va` in one of the described module's code segments?
    bool inside(Va va) const;
    // The decoded segment of the loader's bytes / of the relocated image (kept for the life of the description).
    Code16& loaded(std::int64_t seg) const;
    Code16& relocated(std::int64_t seg) const;

private:
    mutable std::map<std::int64_t, std::unique_ptr<Code16>> loaded_, relocated_;
};

// A virtual device driver (describe_le).
struct LeDescription : Description {
    std::map<std::int64_t, std::pair<Va, std::int64_t>> spans;   // object -> (address, length)
    std::string device;                                          // the driver's device name
};

// A real-mode program (describe_dos).
struct DosDescription : Description {
    std::map<std::int64_t, std::pair<Va, std::int64_t>> spans;   // part number -> (address, length)
    Bytes image;                                                 // the program's bytes as they sit in memory
    std::int64_t base = 0;                                       // its linear base
    formats::dos::Program program;
    std::shared_ptr<const analyze::dos_functions::Analysis> analysis;
    std::map<std::string, std::vector<analyze::dos_functions::Interrupt>> interrupts;   // function name -> its interrupt calls
};

// --- PE (and PowerPC) -----------------------------------------------------------------------------------------------
// What a PowerPC image says about itself (formats/powerpc_image): where its bytes sit, which stubs are imports, which
// register save/restore runs are part of their callers, and what run-time type information names.
struct PowerPcFacts {
    struct Span {
        Va va = 0;
        std::uint64_t file_offset = 0, length = 0;
    };
    std::vector<Span> spans;
    std::map<Va, std::string> imports, inlined, names;
};

// An import by number as it is called, and the bytes of arguments it removes.
struct NumberedImport {
    std::string module;
    std::int64_t number = 0;
    std::optional<std::int64_t> pop;
};

struct PeDescription {
    std::filesystem::path path;                           // the description file
    std::map<Va, std::string> names;                      // {va: name}
    std::set<std::string> in_ecx;                         // names of its functions that take an argument in ECX
    std::map<std::string, NumberedImport> by_number;      // the C name an import by number is called by
};

// {va: name}: an export's own name, else sub_<rva>. A name also used by an import of this module stays with the import
// (the call has to bind to it).
std::map<Va, std::string> function_names(const formats::pe::Image& image, std::span<const Va> functions,
                                         const std::set<std::string>& imported);
// {address: linker name} for the functions a module exports under a name that is not a C name (a C++ decorated name).
std::map<Va, std::string> decorated_names(const formats::pe::Image& image);
std::map<Va, std::string> decorated_names(const formats::ne::Module& module);
// {symbol: name} for a 16-bit module's imports by number: the symbol the C text uses (`USER_91`) and the real name.
std::map<std::string, std::string> import_names16(const formats::ne::Module& module, Knowledge& knowledge);
// Write the description of a PE module for the native program. `ppc` is given for a PowerPC image. UnsupportedError for a
// machine type with no language.
PeDescription describe32(const std::filesystem::path& module, const formats::pe::Image& image, std::span<const Va> functions,
                         const std::filesystem::path& outdir, Knowledge& knowledge, const PowerPcFacts* ppc = nullptr);

// --- NE: finding functions and how they return ---------------------------------------------------------------------
// Every function start that can be established, and whether it is far or near.
std::map<SegOff, Distance> find_functions(const formats::ne::Module& module);
// Entry sequences after an end that are not among `known`.
std::vector<SegOff> hidden_functions(const formats::ne::Module& module, const std::set<SegOff>& known);
// Offsets in one code segment where an entry sequence follows the end of code.
std::vector<std::int64_t> prologues_after_ends(const Code16& code);

struct Return {
    std::optional<Distance> kind;       // how it returns
    std::int64_t pops = -1;             // bytes of arguments removed; -1 when no return was met within the limit
};
// How the function at `off` returns, from its own nearest return instruction (`want` asks for one kind).
Return returns(const Code16& code, std::int64_t off, std::int64_t limit = 600, std::optional<Distance> want = std::nullopt);

// Is ins[k] a return that is a jump (the instructions straight before it push exactly what it takes off)?
bool jump_by_return(const std::vector<Insn>& ins, std::size_t k, const std::set<std::int64_t>& targets = {});
// The addresses of returns that are jumps, in every code segment.
std::vector<Va> return_jumps(const NeDescription& d);
// {(seg, off): "callee" | "caller"} -- who removes a function's arguments, as the original's own calls show.
enum class Remover { Callee, Caller };
std::map<SegOff, Remover> callers_say(const NeDescription& d);
// Bytes of arguments a thunk's caller has removed, for a thunk that puts more arguments under its return address and
// jumps on; nullopt when it is not such a thunk.
std::optional<std::int64_t> forwards(const NeDescription& d, std::int64_t seg, std::int64_t off, Distance kind);
// Bytes of arguments a function takes off the stack itself at its start, or nullopt.
std::optional<std::int64_t> popped_by_hand(const Code16& code, std::int64_t off, Distance kind);
// The bytes of arguments a function reads through BP, or nullopt when it keeps no such frame.
std::optional<std::int64_t> args_read(const Code16& code, std::int64_t start, std::int64_t end, Distance kind);
// Is this the prologue of a far function that Windows calls (exports, callbacks)?
bool windows_far_prologue(ByteView code, std::int64_t off);

// What a function passes on of its result: another function's (a near call in its segment, or unknown for a call whose
// callee is not known), or a jump on into the middle of one.
struct PassOn {
    enum class Kind { Unknown, Near, Into } kind = Kind::Unknown;
    std::int64_t offset = 0;
};
// What a function returns, by its instructions: 0, 2 or 4 bytes. `passes` and `voided`, when given, are appended to.
std::int64_t result_bytes(const Code16& code, std::int64_t start, std::int64_t end, std::vector<PassOn>* passes = nullptr,
                          std::vector<std::int64_t>* voided = nullptr);
// `J` lines: each switch jump through a table in its own code segment.
std::vector<std::string> jump_tables(const NeDescription& d);
// `O` lines: for each call through a pointer, the words the code pushed for it just before.
std::vector<std::string> call_overrides(const NeDescription& d);
// {address: bytes it returns} for every function described.
std::map<Va, std::int64_t> results(const NeDescription& d);

// the bytes of arguments the caller takes off right after the call ins[j], or 0
std::int64_t caller_cleanup(const std::vector<Insn>& ins, std::size_t j);

// Does a function hand back a DX of its own -- on every way to each of its returns?
class DxReturns {
public:
    explicit DxReturns(const NeDescription& d);
    ~DxReturns();
    // any_way: DX set on at least one way (the others leave it unspecified) instead of on every way
    bool every_way(const SegOff& place, const std::set<SegOff>& four, bool any_way = false);

    struct Impl;   // (the decoded functions it keeps; defined in describe_ne_facts.cpp)

private:
    std::unique_ptr<Impl> impl_;
};

// The functions a call to which is followed, before anything else could set it, by a read of DX. `edges`, when given,
// has (caller, callee) added.
std::set<SegOff> callers_read_dx(const NeDescription& d, std::vector<std::pair<SegOff, SegOff>>* edges = nullptr);
// Is the function at `off` a helper whose effect is on its caller's registers (a short straight run to a plain `ret`
// that leaves SI, DI or DS changed)?
bool register_helper(const Code16& code, std::int64_t off, std::optional<std::int64_t> end = std::nullopt);
// The `places` a call to which is followed, before anything sets it, by a read of AX.
std::set<SegOff> callers_read_ax(const NeDescription& d, const std::set<SegOff>& places);
// Whether the function at `off` takes arguments in AX, DX or BX (Microsoft C's _fastcall).
bool register_arguments(const Code16& code, std::int64_t off);
// The description line for a function at `va`.
std::string model_line(const NeDescription& d, Va va, const std::string& name, Distance dflt = Distance::Near);

// --- the writers ----------------------------------------------------------------------------------------------------
// Write the description (and the relocated image it points at).
NeDescription describe16(const formats::ne::Module& module, const std::filesystem::path& outdir, Knowledge& knowledge,
                         const Policy& policy = {});
// Describe one LE image to the decompiler.
LeDescription describe_le(const formats::le::Module& module, std::string_view name, const std::filesystem::path& outdir,
                          Knowledge& knowledge);
// Describe a real-mode program to the decompiler.
DosDescription describe_dos(const formats::dos::Program& program, const std::filesystem::path& outdir);
// Write the description of a NES cartridge. `functions` are the function starts of its fixed address space
// (Rom::functions). -> the path of the description and {va: name}
struct NesDescription {
    std::filesystem::path path;
    std::map<Va, std::string> names;
};
NesDescription describe_nes(const formats::nes::Rom& rom, std::span<const std::uint32_t> functions,
                            const std::filesystem::path& outdir);

}  // namespace cb::decompile::describe
