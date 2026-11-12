// cb/recompile/powerpc.hpp -- Xbox 360 PowerPC recompiled to C++ that runs on a PC.
//
//     codebreaker recompile <image or .xex> --all -o game.cpp      every function
//     codebreaker recompile <image or .xex> --at 0x820FBC58 --size 0x140 --check
//     codebreaker recompile --runtime rex_ppc_min.h                 the runtime header
//
//     Recompiled r = recompile_function(dis, body, address, size);  one function
//     ImageResult all = recompile_image(program, vector_decoder);   every function of an image
//
// The other deliverable beside decompiling: not source to read, but the whole program, ahead of time, as C++ that builds and
// runs (recompile/x360project makes the project). Each guest function becomes
// `void sub_XXXXXXXX(PPCContext& ctx, uint8_t* base)` -- one C++ statement per guest instruction, on a register context and a
// flat 4 GB guest memory -- the method of XenonRecomp and of the rexglue SDK, whose code generator the builders
// (recompile/builders/) are derived from:
//
//     add rD,rA,rB          ctx.rD.u64 = ctx.rA.u64 + ctx.rB.u64;
//     lwz rD,d(rA)          ctx.rD.u64 = PPC_LOAD_U32(ctx.rA.u32 + d);
//     stwu rS,d(rA)         ea = ctx.rA.u32 + d; PPC_STORE_U32(ea, ctx.rS.u32); ctx.rA.u32 = ea;
//     cmpwi cr0,rA,imm      ctx.cr0.compare<int32_t>(ctx.rA.s32, imm, ctx.xer);
//     beq cr0,loc           if (ctx.cr0.eq) goto loc_XXXXXXXX;
//     bl target             ctx.lr = ret; sub_TARGET(ctx, base);
//     bctrl                 ctx.lr = ret; PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
//     blr                   return;
//
// Every function takes the whole context and no arguments, so any branch target can become a function and the branch a tail
// call. Memory is big-endian: every access of two bytes or more is byte-swapped, and volatile so the compiler keeps the
// guest's traffic. Indirect calls go through a table of function pointers by guest address; a `bctr` through a jump table
// becomes a C++ switch (recompile/jumptable). An instruction no builder translates becomes `PPC_UNIMPLEMENTED("...")`: a trap
// at run time, never a wrong translation -- and so does one whose operands the builders cannot read. VMX128 is decoded by
// decompile/vmx128 (the `VectorDecoder` the Disassembler is given).
//
// What the module does itself is control flow (branches, calls, the CTR loops, the switch), the atomics, the record forms
// (`add.` is `add` and a CR0 compare), a few shifts and no-op hints; everything else is a builder's (floating, arith, logical,
// system, memory, vector), tried in that order.
//
// Nothing here knows how an image is read: the caller gives the bytes (`Program`).
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/recompile/disassemble.hpp"
#include "cb/recompile/discover.hpp"
#include "cb/recompile/jumptable.hpp"

namespace cb::recompile::powerpc {

inline constexpr std::uint32_t kWord = 4;

// ---- one instruction ----------------------------------------------------------------------------------------------------

// The function an instruction is in: a branch inside [lo, hi) is a `goto`, one outside a tail call.
struct FunctionBounds {
    std::uint32_t lo = 0;
    std::uint32_t hi = 0;
};

// {bctr address: its jump table (index register, targets)}
using JumpTables = std::map<std::uint32_t, jumptable::JumpTable>;

// The C++ statements for one instruction. An instruction nothing translates, or whose operands cannot be read, gives one
// `PPC_UNIMPLEMENTED("mnemonic operands");`. `insn.word` is the instruction word (some builders decode simplified forms
// straight out of it); `jump_tables` makes a `bctr` a switch instead of an indirect call.
std::vector<std::string> emit_insn(const disassemble::Row& insn, const FunctionBounds& function, const JumpTables& jump_tables = {});

// ---- one function -------------------------------------------------------------------------------------------------------

struct Stats {
    std::size_t total = 0;          // instructions
    std::size_t unimplemented = 0;
    std::size_t translated = 0;
    std::size_t labels = 0;         // branch targets inside the function
};

struct Recompiled {
    std::string cpp;
    std::set<std::uint32_t> called;   // the guest functions it calls, tail calls included
    Stats stats;
};

// The image a function's jump tables are read from (big-endian words at `base` + offset).
struct GuestImage {
    ByteView bytes;
    std::uint32_t base = 0;
    std::function<bool(std::uint32_t)> is_exec;   // may a table's case land on this address? empty: every address is code
};

// The C++ for one guest function: its bytes (`body`, `size` of them are read, rounded down to a word), where it is. With the
// image given, `bctr` switch tables are recovered from it so indirect jumps become real C++ switches.
Recompiled recompile_function(disassemble::Disassembler& dis, ByteView body, std::uint32_t address, std::uint32_t size,
                              const GuestImage* image = nullptr);

// ---- the runtime ----------------------------------------------------------------------------------------------------------

// The runtime header: enough of rex/ppc.h for the output to compile on its own, or to bridge onto the rexglue kernel's
// PPCContext when that is on the include path.
std::string runtime_header();

// AltiVec (128-bit) op helpers emitted into the runtime header. Lane order follows the register's native layout.
std::string_view vector_helpers();

// Forward declarations of the called guest functions, so a single .cpp compiles alone.
std::string forward_declarations(const std::set<std::uint32_t>& called);

// ---- every function of an image -------------------------------------------------------------------------------------------

// An image as the recompiler reads it: what discovery reads of it, and the image laid out as loaded (the functions' bytes).
struct Program {
    discover::Image image;
    disassemble::FlatImage flat;
};

struct ImageOptions {
    std::optional<std::size_t> limit;          // only the first N functions (none: all)
    std::optional<unsigned> jobs;              // threads (none: base/parallel workers)
    std::set<std::uint32_t> skip;              // addresses not to recompile: the import stubs, whose bytes the loader replaces; the host
                                               // provides them, and they stay declared and mapped so calls to them resolve
    std::vector<std::uint32_t> extra;          // addresses known to be called indirectly, found by a run, seeded into discovery
};

struct Aggregate {
    std::size_t funcs = 0;                     // functions recompiled
    std::size_t total = 0;                     // instructions
    std::size_t translated = 0;
    std::size_t unimplemented = 0;
    std::size_t discovered = 0;                // functions discovery found
    std::size_t tail_entries = 0;              // functions added because a tail jump goes to them
    std::uint32_t lo = 0, hi = 0;              // the first and last chosen address
    std::vector<std::string> parts;            // each function's C++, in address order
    std::vector<std::uint32_t> decls;          // every function declared
    std::vector<std::uint32_t> chosen;         // every function mapped
    std::vector<std::uint32_t> skipped;        // chosen and skipped
};

struct ImageResult {
    std::string body;                          // the C++
    std::string mapping;                       // the table of {address, function}
    Aggregate agg;
};

// Find every function (recompile/discover) and recompile each. A function's size is its .pdata length, else the gap to the
// next function (at most 16 KB). The functions are recompiled on a pool of threads, each with its own Disassembler.
ImageResult recompile_image(const Program& program, const disassemble::VectorDecoder& vector, const ImageOptions& options = {});

// ---- checking the output compiles -------------------------------------------------------------------------------------------

// Where the compiler and the rexglue kernel are.
struct CompilerEnv {
    std::optional<std::filesystem::path> gpp;   // settings "gpp"
    std::filesystem::path checkout_root;        // native/rexglue/kernel is below it
    std::optional<std::string> path_variable;   // PATH (none: the environment's)
};

// A C++ compiler for checking the output: the configured one if it is a file, else g++ on PATH.
std::optional<std::filesystem::path> find_compiler(const CompilerEnv& env);

// The include folders of the rexglue kernel core (native/rexglue/kernel), in its CMake order; none when it is not in the tree.
std::vector<std::filesystem::path> kernel_includes(const std::filesystem::path& checkout_root);

struct CompileCheck {
    std::optional<bool> compiles;               // nothing when there is no compiler
    std::string message;                        // the compiler's complaint (3000 bytes at most), or why there was none
};

// Whether one translation unit compiles with the runtime header -- against the kernel core's PPCContext (`bridge`, as a
// generated project builds), else standalone.
CompileCheck check_compiles(std::string_view source, const CompilerEnv& env, bool bridge = true);

// ---- the command --------------------------------------------------------------------------------------------------------------

struct LoadedImage {
    discover::Image image;
    disassemble::ImageFacts facts;
};

struct Hooks {
    std::function<LoadedImage(const std::filesystem::path&)> load_image;
    disassemble::VectorDecoder vector;
    CompilerEnv compiler;
    // `--all` with no -o writes into the binary's project folder (Converted/<name>/src/<name>.cpp): the path it gives (making
    // the project), and what to say when the files are written.
    std::function<std::filesystem::path(const std::filesystem::path& binary)> project_output;
    std::function<void(const std::filesystem::path& binary, const std::vector<std::filesystem::path>& written, std::string_view notes)> record;
};

// `codebreaker recompile <image or .xex> [--all] [--at ADDR --size N] [-o FILE] [--limit N] [-j N] [--check] [--runtime HEADER]`.
// Returns the exit code: 0, 1 when --check finds the output does not compile, 2 for a usage error.
int main(const Hooks& hooks, std::span<const std::string> args);

}  // namespace cb::recompile::powerpc
