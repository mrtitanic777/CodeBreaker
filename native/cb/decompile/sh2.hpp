// cb/decompile/sh2.hpp -- the Hitachi SH-2's instructions (SH7604: the Saturn's two CPUs and the 32X's two; every SH-1 instruction and the
// SH-2's additions), big-endian: a disassembler and an assembler.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "mov.l   r8,@-r15" / "bf/s    L_06004010" / "mov.l   K_06004040,r1"
//     assemble(line, pc, labels)   the two bytes (big-endian) of one instruction line
//
// Every instruction is 16 bits. The syntax is GNU as's for the SH (the assembler the Saturn and 32X homebrew toolchains use: sh-elf-as),
// written the way its output is read: operands source first, `#` an immediate, `@` memory, registers r0-r15 (sp is r15), the control
// registers sr gbr vbr mach macl pr:
//
//     mov.l   @(8,r4),r0          a displacement in bytes (a multiple of the size: .w 2, .l 4)
//     mov.b   r0,@(3,gbr)         @(disp,gbr);  @(r0,gbr) and @(r0,rn) index by r0
//     mov.l   K_06004040,r1       a PC-relative load of a constant held in the code (`mov.w`, `mov.l`, `mova`): the operand is the label
//                                 of the constant, or @(disp,pc) with the displacement in bytes from the base (the address of the
//                                 instruction + 4, with the low two bits cleared for a longword load and `mova`)
//     bra     sub_06004100        a branch's operand is its target address (a label, a number or an expression)
//     bf/s    L_06004010
//     ldc     r1,sr   sts.l pr,@-r15   lds.l @r15+,pr
//
// A branch (`bf bt bra bsr braf bsrf jmp jsr rts rte` and the delayed `bf/s bt/s`) has a delay slot: the instruction after it, which runs
// before the transfer takes effect (`Insn::delay_slot`). The decoder does not move or fill one in. A word that is not an instruction of
// the SH-2 (the zero word, an FPU word) is not decoded, so data is not read as code, and what is written as an instruction assembles back
// to the same word.
//
//
// The SH-4 (SH7750: the Dreamcast's CPU) is a variant (`Variant::Sh4`): every instruction above, little-endian (the Dreamcast runs the SH-4 in
// little-endian mode, so the two bytes of a word are low first), and the SH-4's own: the FPU (`fadd fr1,fr2`, `fmov.s @r4+,fr0`, `fipr fv0,fv4`,
// `ftrv xmtrx,fv0`, `fsca fpul,dr0`, `float fpul,fr3`, `fmac fr0,fr1,fr2`), `fschg` and `frchg` (which flip FPSCR's SZ and FR), `shad` and `shld`,
// `pref @rn`, `ocbi`, `ocbp`, `ocbwb`, `movca.l r0,@rn`, `ldtlb`, `clrs`, `sets`, and the extra control registers (`ssr spc sgr dbr fpul fpscr`,
// `r0_bank`..`r7_bank`) in `ldc`, `stc`, `lds` and `sts`. The text of a word is the same whatever FPSCR holds (the single-precision names `frN`,
// `fmov.s`); the assembler also takes the other views of the same word: `drN` (even) where PR=1 makes an operation double, `fmov.d`, and `xdN`
// where SZ=1 moves a pair. Every word that decodes assembles back to itself, in either variant.
//
// Expressions are numbers (decimal, 0x hex), labels, `.` (here) and `+ - * / & | ^ ~ << >>` with parentheses, as GNU as reads them.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/decompile/labels.hpp"
#include "cb/decompile/z80.hpp"

namespace cb::decompile::sh2 {

// Which SuperH: the SH-2 (SH7604, big-endian) or the SH-4 (SH7750 in the Dreamcast's little-endian mode: the SH-2's instructions and the FPU).
enum class Variant : std::uint8_t { Sh2, Sh4 };
inline bool little_endian(Variant v) { return v == Variant::Sh4; }

// How control leaves an instruction (the Z80 module's: None, Jump, Branch, Call, Return, Indirect, Halt).
using Flow = z80::Flow;

// What an operand is; each has a text and an encoding (sh2.cpp's table).
enum class Operand : std::uint8_t {
    None,
    RegN,      // rn: the register in bits 8-11
    RegM,      // rm: bits 4-7
    R0,        // r0
    ImmS,      // #imm: 8 bits, sign-extended (mov add cmp/eq)
    ImmU,      // #imm: 8 bits, unsigned (and or xor tst trapa)
    IndN,      // @rn
    IndM,      // @rm
    PostN,     // @rn+
    PostM,     // @rm+
    PreN,      // @-rn
    DispN,     // @(disp,rn): disp = 4-bit field x size, the register in bits 8-11
    DispM,     // @(disp,rm): the register in bits 4-7
    R0IndexN,  // @(r0,rn)
    R0IndexM,  // @(r0,rm)
    GbrDisp,   // @(disp,gbr): disp = 8-bit field x size
    GbrR0,     // @(r0,gbr)
    PcDisp,    // @(disp,pc), or the label of the constant it loads
    Branch8,   // bt bf: target = pc + 4 + 2 x the signed 8-bit field
    Branch12,  // bra bsr: ... the signed 12-bit field
    Sr, Gbr, Vbr, Mach, Macl, Pr,
    // the SH-4's
    FrN,       // frn: the register in bits 8-11 (a double operation reads drN, N even, the same bits)
    FrM,       // frm: bits 4-7
    MovN,      // frn, drn or xdn of an FMOV (SZ=1 moves a pair, and bit 8 picks the bank): the text is always frn
    MovM,      // the same in bits 4-7
    DrN,       // drn: the even register in bits 9-11 (bit 8 clear)
    FvN,       // fvn: the vector fv0 fv4 fv8 fv12 in bits 10-11
    FvM,       // fvm: bits 8-9
    Xmtrx, Fpul, Fpscr, Ssr, Spc, Sgr, Dbr,
    BankM,     // rm_bank: r0_bank .. r7_bank, in bits 4-6
};

// One decoded instruction.
struct Insn {
    std::uint16_t word = 0;
    std::uint32_t pc = 0;                  // the address it runs at
    std::string mnemonic;                  // "mov.l", "bf/s", "cmp/eq", "shll2" ...
    Operand a = Operand::None, b = Operand::None;   // the operands, source first
    std::uint8_t n = 0, m = 0;             // the register fields (bits 8-11 and 4-7)
    std::uint8_t size = 0;                 // 1, 2 or 4: the width of the access or of a displacement's unit (0: none)
    std::int32_t imm = 0;                  // ImmS/ImmU: the value; Disp/GbrDisp/PcDisp: the displacement in bytes
    std::optional<std::uint32_t> target;   // where a branch, bsr or bra goes
    std::optional<std::uint32_t> literal;  // the address a PC-relative load (mov.w, mov.l) reads, or mova makes
    Flow flow = Flow::None;
    bool delay_slot = false;               // the instruction after it runs before the transfer takes effect

    std::array<std::uint8_t, 2> raw{};     // the word as the code holds it (big-endian)

    ByteView bytes() const { return {raw.data(), raw.size()}; }
    // mova: `literal` is an address, not a constant that is read.
    bool makes_address() const { return mnemonic == "mova"; }
};

// The instruction at code[at:], which runs at address `pc`, or nullopt when the bytes are not one (or are fewer than two). The word is read big-endian
// for the SH-2 and little-endian for the SH-4.
std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc, Variant variant = Variant::Sh2);
// The same from the word.
std::optional<Insn> decode_word(std::uint16_t word, std::uint32_t pc, Variant variant = Variant::Sh2);

// The instruction as source. `labels` names branch targets and the constants a PC-relative load reads; an empty name counts as no label.
std::string text(const Insn& insn, const AddressLabels& labels = {});

// The register's name ("r4", "sp" is r15 and is written "r15"), and the number of a name ("r4", "sp"; nullopt for neither).
std::string_view register_name(unsigned number);
std::optional<unsigned> register_number(std::string_view text);

// A numeric expression as the assembler reads it. ParseError for a malformed one, z80::UnknownLabel for a label not in `labels`.
std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc = 0);

// The two bytes of one instruction line. `labels` resolves names; `pc` is where the instruction will be. ParseError for text that is not
// an instruction or one whose operand is out of reach (a branch too far, a displacement that is not a multiple of the size or does not fit
// its field); z80::UnknownLabel for a label that is not there.
Bytes assemble(std::string_view line, std::uint32_t pc = 0, const NameLabels& labels = {}, Variant variant = Variant::Sh2);

}  // namespace cb::decompile::sh2
