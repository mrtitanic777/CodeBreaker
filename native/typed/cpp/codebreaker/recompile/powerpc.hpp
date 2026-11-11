// codebreaker/recompile/powerpc.hpp -- typed C++ port of codebreaker.recompile.powerpc.
//
// powerpc.py -- Xbox 360 PowerPC recompiled to C++ that runs on a PC.
//
//     python -m codebreaker recompile <image or .xex> --all -o game.cpp    every function
//     python -m codebreaker recompile <image or .xex> --at 0x820FBC58 --size 0x140 --check
//     python -m codebreaker recompile --runtime rex_ppc_min.h               the runtime header
//
//     from codebreaker.recompile import powerpc
//     cpp, called, stats = powerpc.recompile_function(body, base, address, size)
//     body, mapping, agg = powerpc.recompile_image(path)
//
// The other deliverable beside decompiling: not source to read, but the whole program, ahead of
// time, as C++ that builds and runs (recompile/project.py makes the project). Each guest function
// becomes `void sub_XXXXXXXX(PPCContext& ctx, uint8_t* base)` -- one C++ statement per guest
// instruction, on a register context and a flat 4 GB guest memory -- the method of XenonRecomp and
// of the rexglue SDK, whose code generator the builders (recompile/builders/) are derived from:
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
// Every function takes the whole context and no arguments, so any branch target can become a
// function and the branch a tail call. Memory is big-endian: every access of two bytes or more is
// byte-swapped, and volatile so the compiler keeps the guest's traffic. Indirect calls go through a
// table of function pointers by guest address; a `bctr` through a jump table becomes a C++ switch
// (jumptable.py). An instruction no builder translates becomes `PPC_UNIMPLEMENTED("...")`: a trap
// at run time, never a wrong translation. VMX128 is decoded by codebreaker/decompile/vmx128.py.
//
// Typing notes:
//   * Operand helpers answer None as std::optional; `_mem` answers (disp, base) or None. Where the Python formats an
//     operand that could be None the text is "None" (as f"{None}" is); where it does arithmetic on one, the TypeError.
//   * emit_insn takes `raw` as an int (the Python's default 0) and `jump_tables` as an optional dict
//     {bctr address: (index register, [targets])}; it returns the statements as a list of str.
//   * recompile_function's `img` is the image's bytes (a pointer, not a copy of them: null for none), `is_exec` a std::function
//     (empty: every address is code).
//     Its stats dict (fixed keys) is `Stats`, its `called` set py::set<int64_t>.
//   * recompile_image answers (C++ text, mapping table text, agg); agg's dict with fixed keys is `Agg`, its "tail entries"
//     key the field `tail_entries`, its "image" the PowerPC image. The functions are recompiled on a pool of `jobs` threads
//     (default: base/parallel workers()), the results taken in the functions' order.
//   * compiler() reads settings.json "gpp" (py::settings), else g++ on PATH; kernel_includes() derives its folder from
//     ROOT, the checkout this source sits in (six folders above it: native/typed/cpp/codebreaker/recompile/powerpc.cpp);
//     set_root() points it elsewhere (as xenon.hpp's does).
//   * main(argv) takes sys.argv[1:] and returns the exit code; it prints with py::print. Without a function address,
//     or without arguments, it prints the Python's usage text.
//   * Python ints are unbounded; int64_t is the type (an address or an immediate comes nowhere near it).
//   * not ported: the `__name__ == "__main__"` block (`sys.exit(main(sys.argv[1:]))`).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/formats/powerpc_image.hpp"
#include "py.hpp"

namespace codebreaker::recompile::powerpc {

inline constexpr int64_t WORD = 4;

// the checkout (default: six folders above powerpc.cpp)
extern py::str ROOT;
void set_root(const py::str& root);

// AltiVec (128-bit) op helpers emitted into the runtime header. Kept as a plain
// string (normal C++ braces) so the f-string runtime_header() need not escape
// them. Lane order follows the register's native layout.
extern const py::str _VEC_HELPERS;

// condition-branch mnemonic -> (C++ predicate on a cr field, is_conditional)
extern const py::dict<py::str, py::str> _COND;

// capstone's symbolic CR-bit suffix -> bit-within-field (un = unordered = SO).
extern const py::dict<py::str, int64_t> _CRFIELD_BIT;

// AltiVec ops with a 3-vector-operand (vD,vA,vB) form -> a runtime helper call.
extern const py::set<py::str> _VEC3;

std::optional<int64_t> _reg(const py::str& tok);

// A register operand where a bare 0 means the literal zero (rA of the X-form loads and stores:
// Capstone writes `stfiwx f0, 0, r11`): 0, which the builders read as "no base register".
std::optional<int64_t> _reg_or_zero(const py::str& tok);

std::optional<int64_t> _imm(const py::str& tok);

// '-0x70(r1)' -> (disp, basereg) ; 'r0'-base handled by caller.
std::optional<std::tuple<int64_t, int64_t>> _mem(const py::str& tok);

// Bare 'crN' -> CR *field* index 0-7 (compares/mcrf/mtcrf/branches).
// Symbolic 'crNlt|gt|eq|un|so' (capstone CR-bit operand) -> CR *bit* index
// 0-31 (the crand/cror/... family, which divides by 4 to recover the field).
// The two consumers use disjoint token shapes, so one helper serves both.
std::optional<int64_t> _cr(const py::str& tok);

py::list<py::str> _split(const py::str& op_str);

// f"ctx.r{n}.{field}"
py::str _rv(const std::optional<int64_t>& n, const py::str& field = py::str("u64"));

// address base register expression; r0 in an address means literal 0.
py::str _base_expr(const std::optional<int64_t>& base);

py::str _addr(int64_t disp, int64_t base);

std::optional<int64_t> _freg(const py::str& tok);
std::optional<int64_t> _vreg(const py::str& tok);

// {bctr address: (index register, [targets])}
using JumpTables = py::dict<int64_t, std::tuple<int64_t, py::list<int64_t>>>;

// Return a list of C++ statement strings for one instruction.
//
// `raw` is the 32-bit big-endian instruction word (some builders decode
// simplified forms straight out of it). `jump_tables` maps a bctr's address to
// (index_reg, [targets]) so it emits a real switch instead of an indirect call.
py::list<py::str> emit_insn(int64_t va, const py::str& mn, const py::str& op_str, const py::set<int64_t>& labels, int64_t func_lo,
                            int64_t func_hi, int64_t raw = 0, const std::optional<JumpTables>& jump_tables = std::nullopt);

// recompile_function's stats dict
struct Stats {
    int64_t total = 0;
    int64_t unimpl = 0;
    int64_t translated = 0;
    int64_t labels = 0;
};

// Emit the C++ for one guest function. Returns (cpp_text, called, stats).
//
// When `img` (the whole loaded image) is supplied, `bctr` switch tables are
// recovered from it (jumptable.py) so indirect jumps become real
// C++ switches; `is_exec(addr)` validates candidate table targets.
std::tuple<py::str, py::set<int64_t>, Stats> recompile_function(const py::bytes& body, int64_t base_va, int64_t func_va, int64_t size,
                                                                const py::bytes* img = nullptr, int64_t img_base = 0,
                                                                const std::function<bool(int64_t)>& is_exec = nullptr);

// The runtime header: enough of rex/ppc.h for the output to compile on its own.
py::str runtime_header();

// Forward-declare called guest functions so a single .cpp compiles alone.
py::str _emit_forward_decls(const py::set<int64_t>& called);

// recompile_image's agg dict
struct Agg {
    int64_t funcs = 0;
    int64_t total = 0;
    int64_t translated = 0;
    int64_t unimpl = 0;
    int64_t discovered = 0;
    int64_t tail_entries = 0;   // "tail entries"
    int64_t lo = 0;
    int64_t hi = 0;
    py::list<py::str> parts;    // each function's C++, in address order
    py::list<int64_t> decls;
    py::list<int64_t> chosen;
    py::list<int64_t> skipped;
    py::ref<formats::powerpc_image::Image> image;
};

// Find every function (discover.py) and recompile each. -> (C++ text, mapping table text, agg)
//
// A function's size is its .pdata length, else the gap to the next function (at most 16 KB).
// `skip`: addresses not to recompile -- the import stubs, whose bytes the loader replaces;
// the host provides them, and they stay declared and mapped so calls to them resolve.
// `extra`: addresses known to be called indirectly, found by a run, seeded into discovery.
std::tuple<py::str, py::str, Agg> recompile_image(const py::str& path, std::optional<int64_t> limit = std::nullopt,
                                                  std::optional<int64_t> jobs = std::nullopt,
                                                  const std::optional<py::set<int64_t>>& skip = std::nullopt,
                                                  const std::optional<py::list<int64_t>>& extra = std::nullopt);

// A C++ compiler for checking the output: settings.json `gpp`, else g++ on PATH (or None).
std::optional<py::str> compiler();

// The include folders of the rexglue kernel core (native/rexglue/kernel), in its CMake order;
// [] when it is not in the tree.
py::list<py::str> kernel_includes();

// (compiles, compiler's message) for one translation unit with the runtime header -- against
// the kernel core's PPCContext (`bridge`, as a generated project builds), else standalone. `compiles` is None
// when there is no compiler.
std::tuple<std::optional<bool>, py::str> check_compiles(const py::str& full, bool bridge = true);

// python -m codebreaker recompile <image or .xex> [--all] [--at ADDR --size N] [-o FILE]
//                                    [--limit N] [--check] [--runtime HEADER]
//
// --all without -o writes into the binary's project folder: Converted/<name>/src/<name>.cpp.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::recompile::powerpc
