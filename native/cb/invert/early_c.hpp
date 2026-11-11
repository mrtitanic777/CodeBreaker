// cb/invert/early_c.hpp -- the inverse of the MC2 code generator: the bytes Microsoft C 2.03 / 3.00 (and Lattice C 2.12)
// emitted, back to C, by recognising the exact code-generation templates (not heuristics), with if/while control flow
// from a recursive block walker (`recover`) or from the structured control-flow graph (`recover_cfg`). Validated by
// round trip: the recovered C compiles back to the same bytes.
//
// The model itself (the register-value tracker and the text passes over what it emits) is the C++ of native/invert; this
// module is its typed API. Everything the Python kept as module settings and hooks is an `Options` value the caller
// passes to each call, so two recoveries with different settings can run side by side.
//
//     early_c::Options options;
//     options.hooks.callee_pop_bytes = [&](const early_c::CallSite& call) -> std::uint32_t { ... };
//     early_c::Recovered fn = early_c::recover_cfg(code, 0x1000, {}, options);
//     std::string c = early_c::to_c("fn", fn.frame_size, fn.params, fn.statements, fn.return_type, fn.locals);
//
// Registers a statement may name (si, di, es ...) are listed in the model's fixed order ax bx cx dx si di es: where
// the Python's order followed the hash of each name in its process (set order), the result is deterministic here.
//
// Errors: input the model cannot lift (bytes that are not the modelled compiler's code) is a FormatError; a function
// shape it recognises and does not handle is an UnsupportedError; a failed internal check is an InternalError. A hook's
// own exception propagates unchanged. NotFoundError if capstone.dll cannot be found (the model decodes with it).
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/invert/early_c_frame.hpp"

namespace cb::invert::early_c {

// The scratch registers a callee can read as arguments.
enum class Reg { Ax, Bx, Cx, Dx };

// A call instruction the walker met. `address` is its offset in the code (0-based, before `base`).
struct NearCall {
    std::uint32_t address = 0;
    std::optional<std::uint32_t> target;   // absent when the operand is not a plain offset
};
struct FarCall {
    std::uint32_t address = 0;
    std::optional<Far16> target;
};
using CallSite = std::variant<NearCall, FarCall>;

// Facts the driver knows from outside the function (a whole-program analysis). All optional: an empty function means
// "no answer".
struct Hooks {
    // The case-target offsets (0-based, in table order) of the jump-table dispatch `jmp cs:[bx+table]` at `address`.
    std::function<std::optional<std::vector<std::uint32_t>>(std::uint32_t address)> switch_targets;
    // How many bytes the callee pops on return: `ret N` with N > 0 means the Pascal convention (arguments pushed left to
    // right, N/2 words consumed); 0 is a plain `ret` (C convention) or unknown.
    std::function<std::uint32_t(const CallSite&)> callee_pop_bytes;
    // The scratch registers the callee reads before writing: arguments passed in registers. They are settled into their
    // variables before the call and the call is preceded by `USE(regs);`.
    std::function<std::vector<Reg>(const CallSite&)> callee_register_args;
};

// The model's settings, read at each call.
struct Options {
    bool mark = false;                       // byte-wide memory operands as BYTE(x), so a checker can tell 8 from 16 bits
    bool far_pointers = false;               // checker mode: es is a real register, far dereferences are FARM(es, x)
    bool dead_registers = false;             // drop register assignments nothing reads
    bool unsigned_compares = false;          // readable listings: mark unsigned comparisons
    bool fp_casts = false;                   // 8087 field operands through casts (the variant model text)
    bool eager_register_variables = false;   // MS C 3.00: a register variable updated from itself (di = di >> 3) is a statement at once
    bool frame_after_bp = false;             // MS C 3.00 and later: `sub sp,N` after `mov bp,sp` (no stack probe, -Gs) is the frame too
    bool reread_register_params = true;      // MS C 3.00: a register a parameter is loaded into is the parameter only if its slot is not read again
    bool caller_cleans = false;              // a call whose callee says nothing: the caller pops nothing (MASM_CRET)
    bool compare_switches = false;           // flow16: a chain of compares against constants is a switch (CMPSWITCH)
    bool uncapped_structures = false;        // flow16: do not cap structure nesting (CFG_NOCAP)
    std::optional<bool> returns_value;       // from a whole-program "is the result ever read by a caller"; absent: a local heuristic
    Hooks hooks;

    // The defaults with the environment's switches applied, as the Python read them: MC2INV_FAR=1 (far_pointers),
    // MC2INV_DEADREGS=1 (dead_registers), MC2INV_UNSIGNED=1 (unsigned_compares), MASM_CRET and CFG_NOCAP when set and
    // not empty (caller_cleans, uncapped_structures).
    static Options from_environment();
};

// What the recovery found out about the function besides its text.
struct Findings {
    std::vector<std::string> saved_registers;                     // si, di: the register variables MS C 3.00 saves in the prologue
    OrderedMap<std::string, std::uint32_t> register_params;       // register -> parameter index, for parameters declared `register`
    OrderedMap<std::string, std::string> fp_types;                // 8087 value -> its C type
    std::string fp_return;                                        // the C type of an 8087 return value; empty if there is none
    std::set<std::string> unsigned_longs;                         // longs that are compared unsigned
};

struct Recovered {
    std::uint32_t frame_size = 0;               // the prologue's `sub sp,N`
    std::vector<std::string> params;
    std::vector<std::string> statements;        // the body, one C line each (nested blocks indented)
    std::string return_type = "int";
    std::vector<early_c_frame::Offset> locals;  // the frame offsets of the locals the body names
    Findings findings;
};

// The function in `code`, which was loaded at `base`, by the linear block walker. `helper_operators` says which C operator
// (+ - * / %) a call to one of the compiler's arithmetic helper routines (long and floating-point arithmetic) performs, by the
// helper's 16-bit offset, base included; the driver knows them from the library. A helper not listed is `+` for the
// floating-point pattern and `*` for the long one.
Recovered recover(ByteView code, std::uint32_t base = 0, const std::unordered_map<std::uint32_t, std::string>& helper_operators = {},
                  const Options& options = {});
// The same by the structured control-flow graph (flow16), with the text passes after it.
Recovered recover_cfg(ByteView code, std::uint32_t base = 0,
                      const std::unordered_map<std::uint32_t, std::string>& helper_operators = {}, const Options& options = {});

// Live local-to-local copies `vX = vY;` in straight-line statements, as (dest, src) offsets, where src is initialised
// before and NOT overwritten after: the condition under which MC2 swaps the offsets of the two.
std::vector<early_c_frame::Copy> detect_copies(std::span<const std::string> statements);

// A 32-bit compare's ladder (the high words, then the low words without sign) folded into one comparison. `mark` as
// Options::mark.
std::vector<std::string> fold_ladders(std::span<const std::string> lines, bool mark = false);

// The declarations of the locals (gap-filled, layout-exact: see early_c_frame::emit_local_decls), one per line,
// indented four spaces.
std::string local_decls(std::uint32_t frame_size, std::span<const early_c_frame::Offset> locals,
                        const std::set<early_c_frame::Offset>& longs = {}, std::span<const early_c_frame::Copy> copies = {});

// A C function from a recovery: the signature, the parameters' declaration (all of `return_type`), the locals
// (with the copies detected in the statements, so the compiler's copy reversal lands them where they were), the body.
std::string to_c(std::string_view name, std::uint32_t frame_size, std::span<const std::string> params,
                 std::span<const std::string> statements, std::string_view return_type = "int",
                 std::span<const early_c_frame::Offset> locals = {}, const std::set<early_c_frame::Offset>& longs = {});

}  // namespace cb::invert::early_c
