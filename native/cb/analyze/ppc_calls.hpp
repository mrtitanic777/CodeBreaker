// cb/analyze/ppc_calls.hpp -- what a PowerPC program's calls show about its functions, from the binary alone.
//
//     ppc_calls::Module module;                       // the discovered functions as listings, the import stubs, ...
//     ppc_calls::Result r = ppc_calls::analyse(module);
//     r.arity        {address: argument registers r3.. its callers set up}   (internal functions)
//     r.imports      {name: argument registers}                            (imported functions)
//     r.clobber      {address: mask of r0, r4-r12 it may change}           (only those that keep some)
//     r.fpclobber    {address: mask of f0-f13 it may change}               (only those that keep some)
//     r.variadic     {address: fixed arguments before the variable ones}
//
// Each is evidence, never a guess:
//
//   * Arity: before a `bl`, the argument registers r3, r4, ... set within the 14 instructions back (stopping at an
//     earlier call), counted while they run on without a gap. A function's arity is the count most of its call sites
//     agree on: at least 3 sites, 60 % agreeing (85 % for 8, which a longer argument list also gives), and 1-7 (0 is
//     a pass-through as often as no arguments). An import is a stub with no body to pass through, so 0 and 8 count
//     there; it needs 3 sites and 85 %, and the printf family, whose count varies by call, is left out.
//   * Clobber: the volatile registers a function may change -- those it writes, and those every function it calls may
//     change, to a fixed point over the call graph. A call to an import, through a pointer or out of the known
//     functions may change them all. The compiler's register save and restore runs (__savegprlr_N ...) change
//     nothing the caller sees, and neither does a register reloaded from where the function stored it on its frame.
//     What is reported is what a function provably keeps: a caller may read those registers across the call.
//   * Variadic: a function that stores r8, r9 and r10 to its caller's frame before its own `stwu` is homing the
//     va_list area; its fixed arguments are the registers below the lowest one homed.
//
// These passes come from the clean-room xam decompile (tools/infer_*.py), where they turned thousands of refused
// functions into faithful C; here they run on a listing of the module's functions (recompile/disassemble) found by
// function discovery (recompile/discover), for any module. Making the Module from a file is the job of those two.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cb::analyze::ppc_calls {

inline constexpr int kLookback = 14;                          // instructions back from a call that set its arguments
inline constexpr std::uint32_t kVolatileMask = (1u << 0) | (1u << 4) | (1u << 5) | (1u << 6) | (1u << 7) | (1u << 8) |
                                               (1u << 9) | (1u << 10) | (1u << 11) | (1u << 12);   // r0, r4-r12 (r3 is the result: always changed)
inline constexpr std::uint32_t kFpVolatileMask = (1u << 14) - 1;                                  // f0-f13
inline constexpr int kVolatileRegisters[] = {0, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// One instruction of a function: its address, its mnemonic, and its operands as the listing writes them
// ("r3", "-0x10(r1)", "0x82001234").
struct Instruction {
    std::uint64_t address = 0;
    std::string mnemonic;
    std::vector<std::string> operands;
};
using Body = std::vector<Instruction>;

// What the passes read of a module.
struct Module {
    std::map<std::uint64_t, Body> functions;                       // by start: the discovered functions, in the code range
    std::unordered_map<std::uint64_t, std::string> import_names;   // import stubs and thunks: address -> the imported name
    std::set<std::uint64_t> transparent;                           // the compiler's register save and restore runs
};

struct Result {
    std::map<std::uint64_t, int> arity;
    std::map<std::string, int> imports;
    std::map<std::uint64_t, std::uint32_t> clobber;      // the volatile registers a function may change; only functions that keep some
    std::map<std::uint64_t, std::uint32_t> fpclobber;
    std::map<std::uint64_t, int> variadic;
    std::size_t functions = 0;
};

Result analyse(const Module& module);

// ---- the pieces ---------------------------------------------------------------------------------------------------

// The operands of a listing's operand text: split at commas and trimmed; none for text with nothing in it.
std::vector<std::string> split_operands(std::string_view text);

// The number of a register token `r<n>` (or, with `fp`, `f<n>` / `fr<n>`).
std::optional<int> register_number(std::string_view token, bool fp = false);

// A memory operand `disp(r<n>)`: the displacement (hex, with or without 0x, may be negative) and the base register.
struct MemoryOperand {
    std::int64_t displacement = 0;
    int base = 0;
};
std::optional<MemoryOperand> memory_operand(std::string_view token);

// The address a branch's first operand gives, when it is written as 0x....
std::optional<std::uint64_t> branch_target(const std::vector<std::string>& operands);

// The general registers an instruction writes (an over-approximation); the floating-point ones.
std::set<int> gpr_writes(std::string_view mnemonic, const std::vector<std::string>& operands);
std::set<int> fpr_writes(std::string_view mnemonic, const std::vector<std::string>& operands);

// How many argument registers (r3, r4 ...) the instructions before body[i] set up for its call.
int site_arity(const Body& body, std::size_t i);

// The count most call sites agree on, or nothing. `stub`: the callee is an import (see the top of this file).
std::optional<int> consensus(std::span<const int> arities, bool stub);

// ---- output ---------------------------------------------------------------------------------------------------------

// The one-line summary of a result.
std::string summary(const Result& r);

// The result as tab-separated lines: "kind", "address or name", "value"; one section per kind, sorted.
std::string to_tsv(const Result& r);

}  // namespace cb::analyze::ppc_calls
