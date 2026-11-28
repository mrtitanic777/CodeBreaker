// decompile/a64.hpp -- AArch64 (ARMv8-A, the A64 instruction set): the table, a disassembler, an assembler.
//
// A64 is the 64-bit mode of the ARMv8-A processors (the Nintendo Switch's Cortex-A57 runs its software in it). Every instruction is one 32-bit little-endian word. One table of
// rows serves the disassembler, the assembler that rebuilds the source the Switch lane writes (source/console_switch), and the interpreter (emulate/a64): a row is the bits it
// fixes (a mask and a value), a mnemonic, an operand list, and what the interpreter does with it (an `Op`). The decoder takes the first row a word matches (the aliases `mov`,
// `cmp`, `ret`, `lsl #n`, `cset` ... stand before the rows they are special cases of, so a word is written as its preferred alias), the assembler tries the rows of a mnemonic in
// the same order and takes the first whose operands fit, and a word that no row matches is not an instruction here (it has no text that assembles back to it).
//
//     decode(code, at, pc)            the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)              "add x0, x1, #0x10"   "ldr x8, [x9, #8]"   "b.ne L_1040"   "stp x29, x30, [sp, #-16]!"   "ret"
//     assemble(line, pc, labels)      the 4 bytes of one instruction line
//
// The syntax is GNU as's (what the assembler of binutils and LLVM read): registers x0-x30 w0-w30 sp wsp xzr wzr (fp and lr read for x29 and x30), vector registers
// v0-v31 with arrangements (`v0.16b`), scalar b0 h0 s0 d0 q0, an immediate `#n` (decimal or 0xhex), `lsl #n` / `uxtw #n` operand modifiers, memory operands in brackets with
// the pre-index `!` or a post-index offset after them, the condition after `b.`, and the page-offset relocation `:lo12:label` where a 12-bit offset is wanted (write
// `adrp x0, label` then `add x0, x0, :lo12:label`). A branch or `adr` operand is an address (a label, or a number).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "decompile/labels.hpp"

namespace cb::decompile::a64 {

// How control leaves an instruction.
enum class Flow : std::uint8_t { None, Branch, Jump, Call, Return, Indirect, Stop };

// What the interpreter does with a row (the family of encodings; it reads the operands from the word).
enum class Op : std::uint16_t {
    None,
    Udf, Adr, Adrp, AddSubImm, LogicImm, MoveWide, Bitfield, Extract, AddSubShift, AddSubExt, AddSubCarry, LogicShift, CondCmp, CondSel, Dp3, Dp2, Dp1, FlagOp,
    B, Bl, BCond, Cbz, Tbz, Br, Blr, Ret, Eret, Svc, Hvc, Smc, Brk, Hlt,
    Hint, Clrex, Dsb, Dmb, Isb, MsrImm, Msr, Mrs, Sys, Sysl,
    LdStImm, LdStReg, LdLit, LdStPair, LdStExcl, LdStAtomic,
    Fp1Src, Fp2Src, Fp3Src, FpCmp, FpCCmp, FpCSel, FpImm, FpInt, FpFixed,
    SimdThree, SimdThreeDiff, SimdTwoMisc, SimdAcross, SimdCopy, SimdModImm, SimdShift, SimdExt, SimdPermute, SimdByElem, SimdTable, SimdLdStMulti, SimdLdStSingle,
    SimdScalarThree, SimdScalarTwoMisc, SimdScalarPair, SimdScalarCopy, SimdScalarShift, SimdScalarByElem,
};

struct Insn {
    std::uint64_t at = 0;                      // the address it sits at
    std::uint32_t word = 0;
    Op op = Op::None;
    std::uint16_t row = 0;                     // the table row it matched
    std::string_view mnemonic;                 // as written ("b.cond" rows: "b"), static storage
    Flow flow = Flow::None;
    std::optional<std::uint64_t> target;       // a branch, adr, adrp or literal load: the address it names
    static constexpr std::uint8_t size = 4;
    std::array<std::uint8_t, 4> raw{};
    ByteView bytes() const { return {raw.data(), 4}; }
};

// The instruction at code[offset], which sits at address `pc`; nullopt for a word no row matches or one cut off by the end of `code`.
std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint64_t pc);
std::optional<Insn> decode_word(std::uint32_t word, std::uint64_t pc);

using decompile::AddressLabels;
using decompile::NameLabels;

// The instruction as source; an address that has a label is written as the label. `lo12`, when not empty, is written for the instruction's 12-bit offset (an add or sub
// immediate, or a load or store's unsigned offset) as `:lo12:<lo12>`.
std::string text(const Insn& insn, const AddressLabels& labels = {}, std::string_view lo12 = {});

// The word of one instruction line. Labels are resolved from `labels`; with `lenient`, a name that is not there counts as 0 and out-of-range values are not refused (the first pass
// of an assembler needs only the size, which is always 4). Throws ParseError for a line that is not an instruction, an unknown name or a value out of range.
std::uint32_t assemble_word(std::string_view line, std::uint64_t pc, const NameLabels& labels = {}, bool lenient = false);
Bytes assemble(std::string_view line, std::uint64_t pc, const NameLabels& labels = {}, bool lenient = false);

// A numeric expression as the assembler reads it: decimal, 0xhex, 0bbinary, labels, `+ - * << >> & |`, parentheses, and `:lo12:x` (x & 0xFFF).
std::int64_t value(std::string_view expr, const NameLabels& labels, bool lenient = false);

// The system registers `mrs` / `msr` name: the name of the register encoded as op0:op1:CRn:CRm:op2 packed in 15 bits (bits 19:5 of the word), or empty.
std::string_view system_register_name(std::uint32_t packed);

// The 16 condition names in their order (eq ne cs cc mi pl vs vc hi ls ge lt gt le al nv).
std::string_view condition_name(unsigned cond);

// Is this a mnemonic of some row?
bool is_mnemonic(std::string_view mnemonic);

// The bitmask immediate (N, immr, imms) of a logical instruction as the 64-bit (or 32-bit) value it stands for, and the other way round; nullopt for an encoding / a
// value that has none (zero, all ones, not a rotated run of ones repeated).
std::optional<std::uint64_t> decode_bitmask(unsigned n, unsigned imms, unsigned immr, unsigned datasize);
struct BitmaskFields {
    unsigned n, immr, imms;
};
std::optional<BitmaskFields> encode_bitmask(std::uint64_t value, unsigned datasize);

// The value an 8-bit floating-point immediate stands for.
double expand_fp_imm8(unsigned imm8);

}  // namespace cb::decompile::a64
